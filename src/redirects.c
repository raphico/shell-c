#include "redirects.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int apply_redirects(cmd_ctx_t *ctx, int *save_fds) {
    redirect_t *r = ctx->redir;

    while (r) {
        int flags =
            O_WRONLY | O_CREAT; // open for write-only, create if it missing
        flags |= r->overwrite
                     ? O_TRUNC
                     : O_APPEND; // append or truncate to zero before writing

        int fd =
            open(r->filename, flags, 0666); // 0666: create file with rw-rw-rw-
        if (fd < 0) {
            perror(r->filename);
            return -1;
        }

        save_fds[r->fd] =
            dup(r->fd); // save old fd, so we can get the original output
        if (save_fds[r->fd] < 0) {
            perror("dup");
            close(fd);
            return -1;
        }

        // redirect fd to the open file
        if (dup2(fd, r->fd) < 0) {
            perror("dup");
            close(fd);
            return -1;
        }

        close(fd);
        r = r->next;
    }

    return 0;
}

void restore_fds(int *save_fds, size_t max_fd) {
    for (size_t fd = 0; fd < max_fd; fd++) {
        if (save_fds[fd] != -1) {
            dup2(save_fds[fd], fd); // put back the original
            close(save_fds[fd]);    // close temporary fd
        }
    }
}
