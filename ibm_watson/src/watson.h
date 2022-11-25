#include <stdio.h>
#include <signal.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <iotp_device.h>

struct arguments{
    char* organizationId;
    char* deviceId;
    char* typeId;
    char* token;
};

int init(IoTPConfig** config, IoTPDevice** device, struct arguments* args);
int disconnect_device(IoTPConfig** config, IoTPDevice** device);
