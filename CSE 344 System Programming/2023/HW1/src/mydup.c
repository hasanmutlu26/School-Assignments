#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int mydup(int oldfd){
    int newfd = fcntl(oldfd, F_DUPFD, 0);
    return newfd;
}

int mydup2(int oldfd, int newfd){
    if (oldfd == newfd) {
        if (fcntl(oldfd, F_GETFL) == -1) {
            errno = EBADF;
            return -1;
        }
        return oldfd;
    }

    close(newfd);

    int result = fcntl(oldfd, F_DUPFD, newfd);
    return result;
}

