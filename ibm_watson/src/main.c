#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include "ubus_ram_handler.h"
#include "watson.h"


void getopts(int argc, char** argv);
void usage(void);

volatile sig_atomic_t daemonize = 1;
int useEnv = 0;
int testCycle = 0;
char* program_name = "watson_ram_sender";
char* configFilePath = NULL;


/* Signal handler - to support CTRL-C to quit */
void sigHandler(int signo) {
    signal(SIGINT, NULL);
    fprintf(stdout, "Received signal: %d\n", signo);
    daemonize = 0;
}

int main(int argc, char *argv[])
{
    IoTPConfig* config = NULL;
    IoTPDevice* device = NULL;

    int rc = 0;
    int cycle = 0;
    char buffer[300];

    struct ubus_context* ctx;
    uint32_t id;

    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);

    /* connect to ubus */
    ctx = ubus_connect(NULL);
	if (!ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
		exit(1);
	}

    rc = ubus_lookup_id(ctx, "system", &id);
    if(rc != 0) {
        fprintf(stderr, "Failed to lookup id\n");
        exit(1);
    }


    /* check for args */
    if ( argc < 2 )
        usage();

    /* get argument options */
    getopts(argc, argv);

    rc = init(&config, &device, configFilePath);

    while(daemonize)
    {
        rc = ubus_invoke(ctx, id, "info", NULL, memory_cb, &buffer, 3000);
        fprintf(stdout, "Send status event\n");
        rc = IoTPDevice_sendEvent(device,"status", buffer, "json", QoS0, NULL);
        fprintf(stdout, "RC from publishEvent(): %d\n", rc);

        if ( testCycle > 0 ) {
            cycle += 1;
            if ( cycle >= testCycle ) {
                break;
            }
        }
        sleep(10);
    }

    disconnect_device(&config, &device);

    return 0;
}

void getopts(int argc, char** argv)
{
    int count = 1;

    while (count < argc)
    {
        if (strcmp(argv[count], "--config") == 0)
        {
            if (++count < argc)
                configFilePath = argv[count];
            else
                usage();
        }
        if (strcmp(argv[count], "--useEnv") == 0) {
            useEnv = 1;
        }
        if (strcmp(argv[count], "--testCycle") == 0) {
            if (++count < argc)
                testCycle = atoi(argv[count]);
            else
                usage();
        }
        count++;
    }
}

void usage(void) 
{
    fprintf(stderr, "Usage: %s --config config_file_path\n", program_name);
    exit(1);
}




