#include <stdio.h>
#include <syslog.h>
#include <libubus.h>
#include <libubox/blobmsg_json.h>

#ifndef UBUS_RAM_HANDLER_H
#define UBUS_RAM_HANDLER_H

struct memory_info{
    uint64_t total;
    uint64_t free;
    uint64_t shared;
    uint64_t buffered;
};

void memory_cb(struct ubus_request *req, int type, struct blob_attr *msg);
int format_memory_info(struct memory_info mem, char* output, int n);

#endif