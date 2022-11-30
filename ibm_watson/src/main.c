#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <syslog.h>
#include "watson.h"
#include "file_locker.h"
#include "ubus_ram_handler.h"
#include "argp_handler.h"


volatile sig_atomic_t daemonize = 1;
char* program_name = "watson_datasender";


void sigHandler(int signo) {
    signal(SIGINT, NULL);
    syslog(LOG_ALERT, "Received signal: %d\n", signo);
    daemonize = 0;
}


int main(int argc, char *argv[])
{
    int rc = 0;
    struct argp argp = { options, parse_opt, "", "" };

    IoTPConfig* config = NULL;
    IoTPDevice* device = NULL;

    struct arguments args;

    struct ubus_context* ctx = NULL;
    uint32_t id;

    handle_flock();
    openlog(program_name, LOG_PID, LOG_DAEMON);

    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);

    ctx = ubus_connect(NULL);
	if (!ctx) {
		syslog(LOG_ERR, "Failed to connect to ubus\n");
        goto cleanup_lock;
	}


    rc = ubus_lookup_id(ctx, "system", &id);
    if(rc != 0) {
        syslog(LOG_ERR, "Failed to lookup id\n");
        goto cleanup_ubus;
    }

    argp_parse(&argp, argc, argv, 0, 0, &args);

    rc = init_watson(&config, &device, &args);
    if(rc != 0){
        syslog(LOG_ERR, "Error initializing. RC = %d",rc);
        goto cleanup_watson;
    }

    while(daemonize) {
        rc = watson_loop(ctx, id, device);
        if(rc) {
            //handle rc
            sleep(5);
        }
        sleep(10);
    }

    cleanup_watson:
        disconnect_device(&config, &device);

    cleanup_ubus:
        ubus_free(ctx);

    cleanup_lock:
        unlock_file();

    closelog();
    return rc;
}