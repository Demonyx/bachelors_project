#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <liburing.h>
#include <stdlib.h>
#define QUEUE_DEPTH 1
#define BLOCK_SZ    1024


struct file_info {
    off_t file_sz;
    struct iovec iovecs[];      /* Referred by readv/writev */
};

off_t get_file_size(int fd);
void output_to_console(char *buf, int len);
int get_completion_and_print(struct io_uring *ring);
int submit_read_request(char *file_path, struct io_uring *ring);