#include <argp.h>
#include "watson.h"

#ifndef ARGP_HANDLER_H
#define ARGP_HANDLER_H

static struct argp_option options[] = {
    {"organizationId", 'o', "organizationId", 0, "Organization ID"},
    {"typeId", 't', "typeId", 0, "Type Id"},
    {"deviceId", 'd', "deviceId", 0, "Device ID"},
    {"token", 'a', "token", 0, "Authentication token"},
    {0}
};

error_t parse_opt(int key, char *arg, struct argp_state *state);
#endif