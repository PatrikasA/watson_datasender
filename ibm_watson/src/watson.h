#include <stdio.h>
#include <signal.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <iotp_device.h>

struct arguments{
    char organizationId[30];
    char deviceId[30];
    char typeId[30];
    char token[30];
};

int init(IoTPConfig** config, IoTPDevice** device, struct arguments* args);
int disconnect_device(IoTPConfig** config, IoTPDevice** device);
