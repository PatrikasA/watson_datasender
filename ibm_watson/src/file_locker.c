#include "file_locker.h"

void handle_flock()
{
    int rc = 0;
    int pid_file = open("/var/run/watson_datasender.pid", O_CREAT | O_RDWR, 0666);
    rc = flock(pid_file, LOCK_EX | LOCK_NB);
    if(rc) {
        if(EWOULDBLOCK == errno)
            syslog(LOG_ERR, "Instance already running. Can only run one instance\n");
        exit(-1);
    }
}

void unlock_file()
{
    int rc = 0;
    int pid_file = open("/var/run/watson_datasender.pid", O_CREAT | O_RDWR, 0666);
    rc = flock(pid_file, LOCK_UN);
    if(rc) {
        syslog(LOG_ERR, "Failed to remove file lock\n");
    }
}