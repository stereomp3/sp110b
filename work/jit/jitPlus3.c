// 修改自 -- https://github.com/spencertipping/jit-tutorial/blob/master/jitproto.c
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/mman.h>

typedef int(*fn)(int);

int main() {
  char *memory = mmap(NULL,             // address
                      4096,             // size
                      PROT_READ | PROT_WRITE | PROT_EXEC,
                      MAP_PRIVATE | MAP_ANONYMOUS,
                      -1,               // fd (not used here)
                      0);               // offset (not used here)
  assert(memory != NULL);
  char code[] = {
    0x8d,0x47,0x03,0xc3
  };

  memcpy(memory, code, 4);
  fn f = (fn) memory;

  printf("plus3(5) = %d\n", (*f)(5));
  munmap(f, 4096);
  return 0;
}
