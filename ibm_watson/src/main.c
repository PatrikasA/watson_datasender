#include <signal.h>
#include <stdlib.h>
#include "ubus_ram_handler.h"
#include "iotp_device.h"

volatile sig_atomic_t daemonize = 1;
char* program_name = "watson_ram_sender";

/* Signal handler - to support CTRL-C to quit */
void sigHandler(int signo) {
    signal(SIGINT, NULL);
    fprintf(stdout, "Received signal: %d\n", signo);
    daemonize = 0;
}

int main()
{
    IoTPConfig* config = NULL;
    IoTPDevice* device = NULL;

    int rc = 0;
    struct ubuss_context* context;

    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);

    /* check for args */
    if ( argc < 2 )
        usage();

    /* Set signal handlers */
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);

    /* get argument options */
    getopts(argc, argv);

    /* Set IoTP Client log handler */
    rc = IoTPConfig_setLogHandler(IoTPLog_FileDescriptor, stdout);
    if ( rc != 0 ) {
        fprintf(stderr, "WARN: Failed to set IoTP Client log handler: rc=%d\n", rc);
        exit(1);
    }

    /* Create IoTPConfig object using configuration options defined in the configuration file. */
    rc = IoTPConfig_create(&config, configFilePath);
    if ( rc != 0 ) {
        fprintf(stderr, "ERROR: Failed to initialize configuration: rc=%d\n", rc);
        exit(1);
    }

    /* read additional config from environment */
    if ( useEnv == 1 ) {
        IoTPConfig_readEnvironment(config);
    } 

    /* Create IoTPDevice object */
    rc = IoTPDevice_create(&device, config);
    if ( rc != 0 ) {
        fprintf(stderr, "ERROR: Failed to configure IoTP device: rc=%d\n", rc);
        exit(1);
    }

    /* Set MQTT Trace handler */
    rc = IoTPDevice_setMQTTLogHandler(device, &MQTTTraceCallback);
    if ( rc != 0 ) {
        fprintf(stderr, "WARN: Failed to set MQTT Trace handler: rc=%d\n", rc);
    }

    /* Invoke connection API IoTPDevice_connect() to connect to WIoTP. */
    rc = IoTPDevice_connect(device);
    if ( rc != 0 ) {
        fprintf(stderr, "ERROR: Failed to connect to Watson IoT Platform: rc=%d\n", rc);
        fprintf(stderr, "ERROR: Returned error reason: %s\n", IOTPRC_toString(rc));
        exit(1);
    }

    /*
     * Set device command callback using API IoTPDevice_setCommandsHandler().
     * Refer to deviceCommandCallback() function DEV_NOTES for details on
     * how to process device commands received from WIoTP.
     */
    IoTPDevice_setCommandsHandler(device, deviceCommandCallback);

    /*
     * Invoke device command subscription API IoTPDevice_subscribeToCommands().
     * The arguments for this API are commandName, format, QoS
     * If you want to subscribe to all commands of any format, set commandName and format to "+"
     */
    char *commandName = "+";
    char *format = "+";
    IoTPDevice_subscribeToCommands(device, commandName, format);


    /* Use IoTPDevice_sendEvent() API to send device events to Watson IoT Platform. */

    /* Sample event - this sample device will send this event once in every 10 seconds. */
    char *data = "{\"d\" : {\"SensorID\": \"Test\", \"Reading\": 7 }}";

    while(!interrupt)
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

    fprintf(stdout, "Publish event cycle is complete.\n");

    /* Disconnect device */
    rc = IoTPDevice_disconnect(device);
    if ( rc != IOTPRC_SUCCESS ) {
        fprintf(stderr, "ERROR: Failed to disconnect from  Watson IoT Platform: rc=%d\n", rc);
        exit(1);
    }

    /* Destroy client */
    IoTPDevice_destroy(device);

    /* Clear configuration */
    IoTPConfig_clear(config);

    return 0;
}




