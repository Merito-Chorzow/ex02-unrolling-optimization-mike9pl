#include <stdint.h>
extern void semihost_putstring(const char *);
extern void semihost_putnumber(unsigned);
//#include <stdio.h>

static unsigned sum8_c(const uint8_t* p, unsigned n) {
  unsigned acc=0;
  for (unsigned i=0;i<n;i++) acc += p[i];
  return acc;
}

/* TODO: Zaimplementuj wersję z unrolling ×4 (C lub ASM) */
unsigned sum8_unroll4(const uint8_t* p, unsigned n) {
  /* WERSJA STARTOWA — nieoptymalna, do podmiany */
  unsigned acc=0;
  unsigned i=0;
  for (; i+3<n; i+=4) {
    acc += p[i+0];
    acc += p[i+1];
    acc += p[i+2];
    acc += p[i+3];
  }
  for (; i<n; ++i) acc += p[i];
  return acc;
}

int main(void) {
  const unsigned S = 123; /* (nr_albumu % 1000) */
  const unsigned N = 500u + (S % 200u);
  static uint8_t buf[1024];
  for (unsigned i=0;i<N;i++) buf[i]=(uint8_t)((i*11u+5u)&0xFF);

  unsigned a = sum8_c(buf, N);
  unsigned b = sum8_unroll4(buf, N);
  //printf("ex02: sum8_c=%u sum8_unroll4=%u\n", a, b);
  semihost_putstring("ex02: sum8_c=");
  semihost_putnumber(a);
  semihost_putstring(" sum8_unroll4=");
  semihost_putnumber(b);
  semihost_putstring("\n");
  if (a==b) semihost_putstring("OK [ex02]"); else semihost_putstring("FAIL [ex02]");

  /* Prosty „pomiar” — wielokrotne wywołania (metryka czasopodobna) */
  volatile unsigned sink=0;
  for (unsigned k=0;k<100;k++) sink += sum8_unroll4(buf, N);
  semihost_putstring("ex02: metric=");
  semihost_putnumber(sink & 0xFFFF);

  return 0;
}
