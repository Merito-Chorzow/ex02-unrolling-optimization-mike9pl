/* src/debug.c — semihosting helpers
 *
 * Provides:
 *   void semihost_putstring(const char *s);    // prints a NUL-terminated string
 *   void semihost_putnumber(unsigned value);   // prints an unsigned in decimal
 *
 * Uses semihosting SYS_WRITE0 (operation 4) via BKPT 0xAB.
 */

#include <stdint.h>

/* semihost call wrapper */
static inline int semihost_call(int op, void *ptr) {
    register int r0 asm("r0") = op;
    register void *r1 asm("r1") = ptr;
    asm volatile ("bkpt 0xAB" : "+r"(r0) : "r"(r1) : "memory");
    return r0;
}

/* Print a NUL-terminated string to the host (SYS_WRITE0) */
void semihost_putstring(const char *s) {
    semihost_call(4, (void *)s); /* SYS_WRITE0 */
}

/* Print an unsigned decimal value (no stdlib). */
void semihost_putnumber(unsigned value) {
    char buf[12]; /* 10 digits max for 32-bit unsigned + optional '\0' */
    int pos = 0;

    if (value == 0) {
        buf[pos++] = '0';
    } else {
        /* fill digits in reverse into a small temp buffer */
        char rev[11];
        int ri = 0;
        unsigned v = value;
        while (v > 0) {
            rev[ri++] = '0' + (v % 10);
            v /= 10;
        }
        /* copy reversed digits into final buffer in correct order */
        while (ri > 0) {
            buf[pos++] = rev[--ri];
        }
    }

    /* NUL-terminate and call semihost */
    buf[pos] = '\0';
    semihost_call(4, buf); /* SYS_WRITE0 */
}