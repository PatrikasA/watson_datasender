#include <signal.h>
#include <stdlib.h>
#include "ubus_ram_handler.h"
#include "watson.h"


void getopts(int argc, char** argv);
void usage(void);

volatile sig_atomic_t daemonize = 1;
char* program_name = "watson_ram_sender";
char* configFilePath = NULL;

/* Signal handler - to support CTRL-C to quit */
void sigHandler(int signo) {
    signal(SIGINT, NULL);
    fprintf(stdout, "Received signal: %d\n", signo);
    daemonize = 0;
}

int main(int argc char *argv[])
{
    IoTPConfig* config = NULL;
    IoTPDevice* device = NULL;

    int rc = 0;
    struct ubus_context* context;

    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);

    /* check for args */
    if ( argc < 2 )
        usage();

    /* get argument options */
    getopts(argc, argv);



    /*
     * Invoke device command subscription API IoTPDevice_subscribeToCommands().
     * The arguments for this API are commandName, format, QoS
     * If you want to subscribe to all commands of any format, set commandName and format to "+"
     */
    char *commandName = "+";
    char *format = "+";
    IoTPDevice_subscribeToCommands(device, commandName, format);

    init(config, device, configFilePath);

    /* Use IoTPDevice_sendEvent() API to send device events to Watson IoT Platform. */

    /* Sample event - this sample device will send this event once in every 10 seconds. */
    char *data = "{\"d\" : {\"SensorID\": \"Test\", \"Reading\": 7 }}";

    while(daemonize)
    {
        fprintf(stdout, "Send status event\n");
        rc = IoTPDevice_sendEvent(device,"status", data, "json", QoS0, NULL);
        fprintf(stdout, "RC from publishEvent(): %d\n", rc);

        if ( testCycle > 0 ) {
            cycle += 1;
            if ( cycle >= testCycle ) {
                break;
            }
        }
        sleep(10);
    }

    disconnect_device(config, device);

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
    fprintf(stderr, "Usage: %s --config config_file_path\n", progname);
    exit(1);
}




