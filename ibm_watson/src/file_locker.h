#include <sys/file.h>
#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void handle_flock();
void unlock_file();