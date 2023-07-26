#include "stub.h"

void __ctype_b(void) {}
void __ctype_tolower(void) {}

void __pthread_register_cancel(void) {}
void __pthread_unregister_cancel(void) {}
void __assert(void) {}
int __fgetc_unlocked(FILE *stream) {
    return fgetc_unlocked(stream);
}

void *mmap64(void *start, size_t len, int prot, int flags, int fd, off_t off);
void *mmap(void *start, size_t len, int prot, int flags, int fd, uint32_t off) {
    return mmap64(start, len, prot, flags, fd, off);
}
