#include <stdio.h>
#include <signal.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include "/home/studentas/Documents/iot-c/src/wiotp/sdk/iotp_device.h"

int init(IoTPConfig** config, IoTPDevice** device, char* configFilePath);
int disconnect_device(IoTPConfig** config, IoTPDevice** device);