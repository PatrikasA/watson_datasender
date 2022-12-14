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
	struct memory_info *mem = (struct memory_info *)req->priv;
	struct blob_attr *tb[__INFO_MAX];
	struct blob_attr *memory[__MEMORY_MAX];

	blobmsg_parse(info_policy, __INFO_MAX, tb, blob_data(msg), blob_len(msg));

	if (!tb[MEMORY_DATA]) {
		syslog(LOG_ERR, "No memory data received\n");
		rc=-1;
		return;
	}

	blobmsg_parse(memory_policy, __MEMORY_MAX, memory,
			blobmsg_data(tb[MEMORY_DATA]), blobmsg_data_len(tb[MEMORY_DATA]));

	*mem = (struct memory_info){
		.total = blobmsg_get_u64(memory[TOTAL_MEMORY]),
		.free = blobmsg_get_u64(memory[FREE_MEMORY]),
		.shared = blobmsg_get_u64(memory[SHARED_MEMORY]),
		.buffered = blobmsg_get_u64(memory[BUFFERED_MEMORY]),
	};
	
}

int format_memory_info(struct memory_info mem, char* output, int n)
{
	int rc = 0;

	rc = snprintf(output, n, "Total memory: %lld Free memory: %lld Shared memory: %lld Buffered memory: %lld",
	mem.total, mem.free, mem.shared, mem.buffered );

	return rc;
}

