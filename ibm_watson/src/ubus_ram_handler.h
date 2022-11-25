#include <stdio.h>
#include <syslog.h>
#include <libubus.h>
#include <libubox/blobmsg_json.h>

void memory_cb(struct ubus_request *req, int type, struct blob_attr *msg);