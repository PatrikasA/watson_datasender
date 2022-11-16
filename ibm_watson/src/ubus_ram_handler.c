#include "ubus_ram_handler.h"

int rc = 0;
struct ubus_context* context = NULL;

enum {
	TOTAL_MEMORY,
	FREE_MEMORY,
	SHARED_MEMORY,
	BUFFERED_MEMORY,
	__MEMORY_MAX,
};

enum {
	MEMORY_DATA,
	__INFO_MAX,
};

struct ram_info{
    uint64_t total;
    uint64_t free;
    uint64_t shared;
    uint64_t buffered;
};

static const struct blobmsg_policy memory_policy[__MEMORY_MAX] = {
	[TOTAL_MEMORY] = { .name = "total", .type = BLOBMSG_TYPE_INT64 },
	[FREE_MEMORY] = { .name = "free", .type = BLOBMSG_TYPE_INT64 },
	[SHARED_MEMORY] = { .name = "shared", .type = BLOBMSG_TYPE_INT64 },
	[BUFFERED_MEMORY] = { .name = "buffered", .type = BLOBMSG_TYPE_INT64 },
};

static const struct blobmsg_policy info_policy[__INFO_MAX] = {
	[MEMORY_DATA] = { .name = "memory", .type = BLOBMSG_TYPE_TABLE },
};

void memory_cb(struct ubus_request *req, int type, struct blob_attr *msg) {
	struct ram_info *mem = (struct ram_info *)req->priv;
	struct blob_attr *tb[__INFO_MAX];
	struct blob_attr *memory[__MEMORY_MAX];

	blobmsg_parse(memory_policy, __MEMORY_MAX, memory, blobmsg_data(tb[MEMORY_DATA]), blob_len(tb[MEMORY_DATA]));

	if (!tb[MEMORY_DATA]) {
		fprintf(stderr, "No memory data received\n");
		rc=-1;
		return;
	}

	blobmsg_parse(memory_policy, __MEMORY_MAX, memory,
			blobmsg_data(tb[MEMORY_DATA]), blobmsg_data_len(tb[MEMORY_DATA]));

	mem -> total = blobmsg_get_u64(memory[TOTAL_MEMORY]);
	mem -> free = blobmsg_get_u64(memory[FREE_MEMORY]);
	mem -> shared = blobmsg_get_u64(memory[SHARED_MEMORY]);
	mem -> buffered = blobmsg_get_u64(memory[BUFFERED_MEMORY]);
}

