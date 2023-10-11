#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <liburing.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

#define QUEUE_DEPTH 1
#define BLOCK_SZ 1024

// How much space should be reserved for metadata?
// Decide on magic numbers to start metadata section

struct io_data {
  int read;
  off_t first_offset, offset;
  size_t first_len;
  struct iovec iov;
};

struct metadata {
  off_t size;
  struct iovec iovecs[]; /* Referred by readv/writev */
};

static int setupContext(unsigned entries, struct io_uring *ring);
static void queuePreparedData(struct io_uring *ring, struct io_data *data);
static int queueDataRead(struct io_uring *ring, off_t size, off_t offset);
static void queueDataWrite(struct io_uring *ring, struct io_data *data);
static int getFileSize(int fd, off_t *size);

void readMetadata();
void writeMetadata();

void checkMetadataValidity();

int write();
void read();

// off_t get_file_size(int fd);
// void output_to_console(char *buf, int len);
// int get_completion_and_print(struct io_uring *ring);
// int submit_read_request(char *file_path, struct io_uring *ring);