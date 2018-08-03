#include "learnuv.h"
#include "../deps/libuv/include/uv.h"

#define BUF_SIZE 37
static const char *filename = __MAGIC_FILE__;

/* Making our lifes a bit easier by using this global, a better solution in the next exercise ;) */
static uv_fs_t open_req;
static uv_buf_t iov;

void read_cb(uv_fs_t *read_req) {
    int r = 0;
    if (read_req->result < 0) CHECK(read_req->result, "uv_fs_read callback");

    /* 4. Report the contents of the buffer */
    log_report("%s", iov.base);
    log_info("%s", iov.base);

    /* 5. Close the file descriptor */
    uv_fs_t close_req;
    r = uv_fs_close(read_req->loop, &close_req, open_req.result, NULL);
    if (r < 0) CHECK(r, "uv_fs_close");

    uv_fs_req_cleanup(&open_req);
    uv_fs_req_cleanup(read_req);
    uv_fs_req_cleanup(&close_req);
}

int main() {
    int r = 0;
    uv_loop_t *loop = uv_default_loop();

    /* 1. Open file (synchronously) */
    r = uv_fs_open(loop, &open_req, filename, O_RDONLY, S_IRUSR, NULL);
    if (r < 0) CHECK(r, "uv_fs_open");
    fprintf(stderr, "\npath %s", open_req.path);
    fprintf(stderr, "\npath %zi\n", open_req.result);

    /* 2. Create buffer and initialize it to turn it into a a uv_buf_t */
    char buf[BUF_SIZE];
    iov = uv_buf_init(buf, sizeof(buf));

    /* 3. Use the file descriptor (the .result of the open_req) to read **aynchronously** from the file into the buffer */
    uv_fs_t read_req;
    fprintf(stderr, "\niov.len %zu", iov.len);
    fprintf(stderr, "\nsizeof(buf) %zu\n", sizeof(buf));
    fprintf(stderr, "\nsizeof(iov) %zu\n", sizeof(iov));
    r = uv_fs_read(loop, &read_req, open_req.result, &iov, 5, -1, read_cb);
    if (r < 0) CHECK(r, "uv_fs_read");


    printf("\nsizeof(uv_buf_t):%zu", sizeof(uv_buf_t));
    printf("\nsizeof(size_t):%zu", sizeof(size_t));

    printf("\nsizeof(uv_buf_t*):%zu", sizeof(uv_buf_t*));

//    log_report("%s", buf);
//    log_info("%s", iov.base);
//

//    char *s = "This is the magic we are looking for!";
//    const uv_buf_t buf2[37];
    uv_buf_t* d[10];
    memcpy(d, &iov, 80);
////    d[strlen(s)] = '\0';//因为从d[0]开始复制，总长度为strlen(s)，d[strlen(s)]置为结束符
//    printf("%s", d);
//
//
//
//    char buf3[37];
//    char* d1[8];
//    memcpy(d1, buf3, 65);

    uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}
