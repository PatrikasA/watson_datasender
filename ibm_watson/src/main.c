#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include "watson.h"
#include "file_locker.h"
#include "ubus_ram_handler.h"
#include "argp_handler.h"


volatile sig_atomic_t daemonize = 1;
char* program_name = "watson_datasender";

/* Signal handler - to support CTRL-C to quit */
void sigHandler(int signo) {
    signal(SIGINT, NULL);
    syslog(LOG_ALERT, "Received signal: %d\n", signo);
    daemonize = 0;
}

void handle_exit(IoTPConfig** config, IoTPDevice** device, struct ubus_context* ctx, int exit_code)
{
    disconnect_device(config, device);
    ubus_free(ctx);
    unlock_file();
	exit(exit_code);
}

int main(int argc, char *argv[])
{
    int rc = 0;
    handle_flock();
    struct argp argp = { options, parse_opt, "", "" };

    IoTPConfig* config = NULL;
    IoTPDevice* device = NULL;

    char data[300];
    struct arguments args;

    struct ubus_context* ctx = NULL;
    uint32_t id;

    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);

    ctx = ubus_connect(NULL);
	if (!ctx) {
		syslog(LOG_ERR, "Failed to connect to ubus\n");
        handle_exit(&config, &device, ctx, 1);
	}

    rc = ubus_lookup_id(ctx, "system", &id);
    if(rc != 0) {
        syslog(LOG_ERR, "Failed to lookup id\n");
        handle_exit(&config, &device, ctx, rc);
    }

    argp_parse(&argp, argc, argv, 0, 0, &args);

    rc = init(&config, &device, &args);
    if(rc != 0){
        syslog(LOG_ERR, "Error initializing. RC = %d",rc);
        handle_exit(&config, &device, ctx, rc);
    }

    struct memory_info mem; 
    while(daemonize)
    {
        rc = ubus_invoke(ctx, id, "info", NULL, memory_cb, &mem, 3000); 
        if(rc != 0){
            syslog(LOG_ERR, "ERROR: Failed to invoke ubus");
            sleep(5);
            continue;
        }       
        
        format_memory_info(mem, data, 300);

        syslog(LOG_INFO, "Send status event\n");
        rc = IoTPDevice_sendEvent(device,"status", data, "json", QoS1, NULL);
        syslog(LOG_INFO, "RC from publishEvent(): %d\n", rc);
        sleep(10);
    }
    handle_exit(&config, &device, ctx, 0);
}