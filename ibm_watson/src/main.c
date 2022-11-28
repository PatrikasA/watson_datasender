#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <argp.h>
#include <sys/file.h>
#include "ubus_ram_handler.h"
#include "watson.h"

volatile sig_atomic_t daemonize = 1;
char* program_name = "watson_datasender";

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch(key){
        case 'o':
                strcpy(arguments->organizationId,arg);
                break;
        case 't':
                strcpy(arguments->typeId, arg);
                break;
        case 'd':
                strcpy(arguments->deviceId, arg);
                break;
        case 'a':
                strcpy(arguments->token, arg);
                break;
        default:
                return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

struct argp_option options[] = {
    {"organizationId", 'o', "organizationId", 0, "Organization ID"},
    {"typeId", 't', "typeId", 0, "Type Id"},
    {"deviceId", 'd', "deviceId", 0, "Device ID"},
    {"token", 'a', "token", 0, "Authentication token"},
    {0}
};

static struct argp argp = { options, parse_opt, "", "" };

/* Signal handler - to support CTRL-C to quit */
void sigHandler(int signo) {
    signal(SIGINT, NULL);
    syslog(LOG_ALERT, "Received signal: %d\n", signo);
    daemonize = 0;
}

void handle_flock()
{
    int rc = 0;
    int pid_file = open("/var/run/watson_datasender.pid", O_CREAT | O_RDWR, 0666);
    rc = flock(pid_file, LOCK_EX | LOCK_NB);
    if(rc) {
        if(EWOULDBLOCK == errno)
            syslog(LOG_ERR, "Instance already running. Can only run one instance\n");
        exit(-1);
    }
}

void handle_exit(IoTPConfig** config, IoTPDevice** device, ubus_context* ctx)
{
    disconnect_device(config, device);
    ubus_free(ctx);
	exit(1);
}

int main(int argc, char *argv[])
{
    int rc = 0;
    handle_flock();

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
        handle_exit(&config, &device, ctx);
	}

    rc = ubus_lookup_id(ctx, "system", &id);
    if(rc != 0) {
        syslog(LOG_ERR, "Failed to lookup id\n");
        handle_exit(&config, &device, ctx);
    }

    argp_parse(&argp, argc, argv, 0, 0, &args);

    rc = init(&config, &device, &args);
    if(rc != 0){
        syslog(LOG_ERR, "Error initializing. RC = %d",rc);
        handle_exit(&config, &device, ctx);
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
    handle_exit(&config, &device, ctx);
}

