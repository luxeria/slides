/*
 * Description: run various statx(2) tests
 *
 */
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <linux/stat.h>

#include "liburing.h"

#if defined(__x86_64)
static int do_statx(int dfd, const char *path, int flags, unsigned mask,
		    struct statx *statxbuf)
{
	return syscall(332, dfd, path, flags, mask, statxbuf);
}
#endif

static int create_file(const char *file, size_t size)
{
	ssize_t ret;
	char *buf;
	int fd;

	buf = malloc(size);
	memset(buf, 0xaa, size);

	fd = open(file, O_WRONLY | O_CREAT, 0644);
	if (fd < 0) {
		perror("open file");
		return 1;
	}
	ret = write(fd, buf, size);
	close(fd);
	return ret != size;
}

static int test_statx(struct io_uring *ring, const char *path)
{
	struct io_uring_cqe *cqe;
	struct io_uring_sqe *sqe;
	struct statx x1;
#if defined(__x86_64)
	struct statx x2;
#endif
	int ret;

	sqe = io_uring_get_sqe(ring);
	if (!sqe) {
		fprintf(stderr, "get sqe failed\n");
		goto err;
	}
	io_uring_prep_statx(sqe, -1, path, 0, STATX_ALL, &x1);

	ret = io_uring_submit(ring);
	if (ret <= 0) {
		fprintf(stderr, "sqe submit failed: %d\n", ret);
		goto err;
	}

	ret = io_uring_wait_cqe(ring, &cqe);
	if (ret < 0) {
		fprintf(stderr, "wait completion %d\n", ret);
		goto err;
	}
	ret = cqe->res;
	io_uring_cqe_seen(ring, cqe);
	if (ret)
		return ret;
#if defined(__x86_64)
	ret = do_statx(-1, path, 0, STATX_ALL, &x2);
	if (ret < 0)
		return -1;
	if (memcmp(&x1, &x2, sizeof(x1))) {
		fprintf(stderr, "Miscompare between io_uring and statx\n");
		goto err;
	}
#endif
	return 0;
err:
	return -1;
}

int main(int argc, char *argv[])
{
	struct io_uring ring;
	const char *fname;
	int ret;

	ret = io_uring_queue_init(8, &ring, 0);
	if (ret) {
		fprintf(stderr, "ring setup failed\n");
		return 1;
	}

	if (argc > 1) {
		fname = argv[1];
	} else {
		fname = "/tmp/.statx";
		if (create_file(fname, 4096)) {
			fprintf(stderr, "file create failed\n");
			return 1;
		}
	}

	ret = test_statx(&ring, fname);
	if (ret) {
		if (ret == -EINVAL) {
			fprintf(stdout, "statx not supported, skipping\n");
			goto done;
		}
		fprintf(stderr, "test_statx failed: %d\n", ret);
		goto err;
	}
done:
	if (fname != argv[1])
		unlink(fname);
	return 0;
err:
	if (fname != argv[1])
		unlink(fname);
	return 1;
}
