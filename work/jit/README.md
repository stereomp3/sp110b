# 習題 3 : 請用 jit 的方法直接執行一個函數的機器碼

> 執行結果:
> [![image](https://user-images.githubusercontent.com/79676439/159680798-8bfb7051-e95b-4a13-ac98-c78a58a811b4.png)](https://user-images.githubusercontent.com/79676439/159680798-8bfb7051-e95b-4a13-ac98-c78a58a811b4.png)

> 程式碼

```c
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
```