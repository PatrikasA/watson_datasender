#include "argp_handler.h"

error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *args = state->input;

    switch(key){
        case 'o':
                strcpy(args->organizationId,arg);
                break;
        case 't':
                strcpy(args->typeId, arg);
                break;
        case 'd':
                strcpy(args->deviceId, arg);
                break;
        case 'a':
                strcpy(args->token, arg);
                break;
        default:
                return ARGP_ERR_UNKNOWN;
    }

    return 0;
}