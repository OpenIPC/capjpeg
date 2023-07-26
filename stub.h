#ifndef __STUB_H__
#define __STUB_H__

#include <stdio.h>
#include <stdint.h>

void __ctype_b(void);
void __ctype_tolower(void);

void __pthread_register_cancel(void);
void __pthread_unregister_cancel(void);
void __assert(void);
int __fgetc_unlocked(FILE *stream);

void *mmap64(void *start, size_t len, int prot, int flags, int fd, off_t off);
void *mmap(void *start, size_t len, int prot, int flags, int fd, uint32_t off);

#endif
