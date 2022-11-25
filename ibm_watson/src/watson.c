#include "watson.h"

void logCallback (int level, char * message)
{
    if ( level > 0 )
        syslog(LOG_INFO, "%s\n", message? message:"NULL");
}

void MQTTTraceCallback (int level, char * message)
{
    if ( level > 0 )
        syslog(LOG_INFO, "%s\n", message? message:"NULL");
}

/* function that sets up connection to device */
int init(IoTPConfig** config, IoTPDevice** device, struct arguments* args)
{
    int rc = 0;
    
    /* Set IoTP Client log handler */
    rc = IoTPConfig_setLogHandler(IoTPLog_FileDescriptor, stdout);
    if ( rc != 0 ) {
        syslog(LOG_ERR, "WARN: Failed to set IoTP Client log handler: rc=%d\n", rc);
        return -1;
    }

    /* Create IoTPConfig object using configuration options defined in the configuration file. */
    rc = IoTPConfig_create(config, NULL);
    if ( rc != 0 ) {
        syslog(LOG_ERR, "ERROR: Failed to initialize configuration: rc=%d\n", rc);
        return -1;
    }

    /* set config properties received from argp */
    IoTPConfig_setProperty(*config, "identity.orgId", args->organizationId);
    IoTPConfig_setProperty(*config, "identity.deviceId", args->deviceId);
    IoTPConfig_setProperty(*config, "identity.typeId", args->typeId);
    IoTPConfig_setProperty(*config, "auth.token", args->token);



    /* Create IoTPDevice object */
    rc = IoTPDevice_create(device, *config);
    if ( rc != 0 ) {
        syslog(LOG_ERR, "ERROR: Failed to configure IoTP device: rc=%d\n", rc);
        return -1;
    }

    /* Set MQTT Trace handler */
    rc = IoTPDevice_setMQTTLogHandler(*device, &MQTTTraceCallback);
    if ( rc != 0 ) {
        syslog(LOG_ERR, "WARN: Failed to set MQTT Trace handler: rc=%d\n", rc);
    }

    /* Invoke connection API IoTPDevice_connect() to connect to WIoTP. */
    rc = IoTPDevice_connect(*device);
    if ( rc != 0 ) {
        syslog(LOG_ERR, "ERROR: Failed to connect to Watson IoT Platform: rc=%d\n", rc);
        syslog(LOG_ERR, "ERROR: Returned error reason: %s\n", IOTPRC_toString(rc));
        return -1;
    }

    return rc;
}

int disconnect_device(IoTPConfig** config, IoTPDevice** device)
{
    int rc = 0;
    syslog(LOG_INFO, "Disconnecting device...\n");

    /* Disconnect device */
    if(*device != NULL){
        rc = IoTPDevice_disconnect(*device);
        if ( rc != IOTPRC_SUCCESS ) {
            syslog(LOG_ERR, "ERROR: Failed to disconnect from  Watson IoT Platform: rc=%d\n", rc);
        }
        /* Destroy client */
        IoTPDevice_destroy(*device);
    }
  
    /* Clear configuration */
    if(*config != NULL){
        rc = IoTPConfig_clear(*config);
        if( rc != IOTPRC_SUCCESS)
            syslog(LOG_ERR, "ERROR: Failed to clear configuration\n");
        }
    return rc;
}