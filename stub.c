
#include "stub.h"

void __pthread_register_cancel(void) {}
void __pthread_unregister_cancel(void) {}
void __assert(void) {}

int __fgetc_unlocked(FILE *stream) {
 return fgetc_unlocked(stream);
}
