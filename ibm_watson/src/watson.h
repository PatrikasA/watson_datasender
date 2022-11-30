#include <stdio.h>
#include <signal.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <iotp_device.h>
#include "ubus_ram_handler.h"

#ifndef WATSON_H
#define WATSON_H

struct arguments{
    char organizationId[30];
    char deviceId[30];
    char typeId[30];
    char token[30];
};

int init_watson(IoTPConfig** config, IoTPDevice** device, struct arguments* args);
int disconnect_device(IoTPConfig** config, IoTPDevice** device);
int watson_loop(struct ubus_context* ctx, uint32_t id, IoTPDevice* device);

#endif
