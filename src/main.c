#include <stdint.h>
extern void semihost_putstring(const char *);
extern void semihost_putnumber(unsigned);
//#include <stdio.h>

static unsigned sum8_c(const uint8_t* p, unsigned n) {
  unsigned acc=0;
  for (unsigned i=0;i<n;i++) acc += p[i];
  return acc;
}

//unrolling ×4, jedna akumulacja, obsługa przekroczenia limitu bez drugiej pętli
unsigned sum8_unroll4(const uint8_t* p, unsigned n) {
  unsigned acc = 0;
  unsigned i = 0;
  // Wyłącza dwa najmłodsze bity n, pozostawiając wyższe bity bez zmian.
  // Zaokrągla n w dół do najbliższej wielokrotności 4.
  unsigned limit = n & ~3u;
  for (; i < limit; i += 4) {
    acc += p[i + 0];
    acc += p[i + 1];
    acc += p[i + 2];
    acc += p[i + 3];
  }
  // przekroczenie limitu: 0–3 elementy
  // zamiast drugiej pętli, rozwiń ręcznie
  // każdorazowo sprawdź, czy i < n
  // przykład: n=6 → limit=4 → i=4 → 2 elementy przekroczyły
  // przykład: n=7 → limit=4 → i=4 → 3 elementy przekroczyły
  // przykład: n=8 → limit=8 → i=8 → 0 elementów przekroczyło
  // przykład: n=5 → limit=4 → i=4 → 1 element przekroczył
  if (i < n) acc += p[i++];
  if (i < n) acc += p[i++];
  if (i < n) acc += p[i++];

  return acc;
}

int main(void) {
  const unsigned nr_albumu = 115413u;
  const unsigned S = nr_albumu % 1000u;
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
  if (a==b) semihost_putstring("OK [ex02]\n"); else semihost_putstring("FAIL [ex02]\n");

  /* Prosty „pomiar” — wielokrotne wywołania (metryka czasopodobna) */
  volatile unsigned sink=0;
  for (unsigned k=0;k<100;k++) sink += sum8_unroll4(buf, N);
  semihost_putstring("ex02: metric=");
  semihost_putnumber(sink & 0xFFFF);
  semihost_putstring("\n");

  return 0;
}
