## 參考資料

* https://tw.gitbook.net/c_standard_library/
* https://www.runoob.com/cprogramming/c-tutorial.html
* https://openhome.cc/Gossip/CGossip/MallocFree.html
* https://icodding.blogspot.com/2018/05/c-define.html
* https://opensourcedoc.com/c-programming/struct/
* https://eee4017.gitbooks.io/unix/content/memory.html
* https://blog.csdn.net/TsingHua666/article/details/80507231
* https://blog.csdn.net/Roland_Sun/article/details/33728955



## head

c語言可能包含很多head檔案，所以可以透過下面的方式來避免重複的情況

```
#ifndef _HELLO_H_
#define _HELLO_H_

...
// 文件內容
...

#endif
```



## inline

inline函數: 可以把短但常用的程式碼內嵌到主函數裡，可以加快程式碼的運行，組合語言可以不用跳來跳去，這個是建議編譯器，所以加上這個不一定可以實現內嵌的功能。

```c
inline int square(int x) { return x*x; }
```



## struct

```c
struct test         // 定義一個結構體
{
    char key[10];       // 結構體成員：key
    char name[20];      // 結構體成員：name
    int age;            // 結構體成員：age
};

typedef struct          // 定義一個結構體型別：DATA ，使用匿名訪問，比較省空間
{
    char key[10];       
    char name[20];      
    int age;            
}DATA;
    
DATA data;              // 宣告一個結構體變數
DATA *pdata;            // 宣告一個指向結構體的指標
    
// 訪問資料操作如下：
data.age = 24;          // 結構體變數通過點運算子( . )訪問
pdata->age = 24;        // 指向結構體的指標通過箭頭運算子( -> )訪問
*data->age = 24         // 其實也可以這樣寫
```







## stdlib.h

`malloc(size)`: 分配heap的動態記憶體，通常會搭配`sizeof()`使用，用完記得要使用free()釋放記憶體

* declaration: `void *malloc(size_t size)`

* return:

  * 如果成功，返回一個pointer分配的內存
  * 如果發生錯誤，返回NULL

* example

  ```c
  #include <stdlib.h> 
  int *p = malloc(100)  // 分配空間，該位置的值是亂數
  *p = 200;  // 在分配的位置給值
  free(p)
  ```

  



`calloc(size, sizeof(type))`: 用法和malloc差不多

* declaration: `void *calloc(size_t nitems, size_t size)`

  * `nitems`: 分配元素數量
  * `size`: 分配元素大小

* return:

  * 如果成功，返回一個pointer分配的內存
  * 如果發生錯誤，返回NULL

* example

  ```c
  #include <stdlib.h> 
  int *p = calloc(1000, sizeof(int));  // 分配空間，該位置的值是亂數
  *p = 200;  // 分配的位置給值
  free(p)
  ```



`realloc(ptr, sizeof(type))`: 重新分配新的記憶體空間地址

* declaration: `void *realloc(void *ptr, size_t size)`

* return:

  * 如果成功，返回一個pointer分配的內存
  * 如果發生錯誤，返回NULL

* example

  ```c
  #include <stdlib.h> 
  int *p = calloc(1000, sizeof(int));  // 分配空間，該位置的值是亂數
  *p = 200;  // 分配的位置給值
  free(p)
  ```



`posix_memalign(memptr, alignment, size)`: 動態配置以alignment(必須是2的次方)對齊的記憶體

* declaration: `int posix_memalign(void **memptr, size_t alignment, size_t size);`

  *      `memptr`: 分配好的内存空间的第一個地址
   *      `alignment`: 對齊邊界，Linux中，32位系统是8bit，64位系统是16bit
   *      `size`: 指定分配size大小的内存

* return:

  * 如果成功，函數返回0，會把分配的內存放到memptr裡面，內存是size大小的動態內存，裡面內存空間是alignment的倍數(必須是2的次方)。
  * 如果發生錯誤，內存不會分配，會返回下面兩種情況
    * EINVAL：參數不是2的冪，或者不是void指針的倍數。
    * ENOMEM：沒有足夠的內存去滿足函數的請求。
  * 要注意的是，對於這個函數，errno不會被設置，只能通過返回值得到。

* example

  ```c
  #include <stdlib.h> 
  long memsz = 0xFFFF + 1;
  unsigned char *ntvCode;
  posix_memalign((void **) &ntvCode, memsz, memsz);  // 資料對齊，並給出記憶體
  free(ntvCode);  // 釋放記憶體
  ```



`atoi(str)`: 把指向的字符串，轉換成`int`型態

* declaration: `int atoi(const char *str)`

* return:

  * 如果成功，返回轉換後的長整數
  * 如果失敗，返回0

* example

  ```c
  #include <stdlib.h> 
  #include <string.h>
  char str[20]; int val;
  strcpy(str, "123456");
  val = atoi(str);  // str = "123456", val = 123456
  strcpy(str, "abc");
  val = atoi(str);  // str = "abc", val = 0
  ```



## string.h

`strcpy(dest, src)`: 比較str1和str2的大小

* declaration: `char *strcpy(char *dest, const char *src)`

* return:

  * 返回一個指向目標字符串dest

* example

  ```c
  #include <string.h>
  char src[40];
  char dest[100];
  
  memset(dest, '\0', sizeof(dest));
  strcpy(src, "This is gitbook.net");
  strcpy(dest, src);  // 把dest內容也會變成 "This is gitbook.net"
  ```
  
  

`strcmp(str1, str2)`: 比較str1和str2的大小(ASCII)

* declaration: `int strcmp(const char *str1, const char *str2)`
* return:
  * 傳回值 < 0: `str1 < str2`
  * 傳回值 > 0: `str1 > str2`
  * 傳回 == 0: `str1 == str2`



`strncat(dest, src)`: 比較str1和str2的大小

* declaration: `char *strncat(char *dest, const char *src, size_t n)`

  * `dest`: 指向目標`char[]`，裡面的空間需要可以容納追加後的字符串，包括額外的空字符。
  * `src`: 要追加的字符串。
  * `n`: 要追加的最大字符數。

* return:

  * 返回一個指向目標字符串dest

* example

  ```c
  #include <string.h>
  char src[50], dest[50];
  strcpy(src,  "This is source");
  strcpy(dest, "This is destination");
  strncat(dest, src, 15); // 把dest內容變成 "This is destinationThis is source"
  ```




`strstr(haystack, needle)`: 在haystack裡面找子字符串needle

* declaration: `char *strstr(const char *haystack, const char *needle)`

  * `haystack`: 被找的字符串。
  * `needle`: 要找的較短字符串

* return:

  * 返回在 haystack 中第一次出現 needle 字符串的位置，如果沒有找到就返回NULL

* example

  ```c
  #include <string.h>
  char src[50], dest[50];
  const char haystack[20] = "RUNOOB";
  const char needle[10] = "NOOB";
  char *ret;
  ret = strstr(haystack, needle); // "NOOB"
  ```



`memmove(str1, str2, n)`: 從 str2 複製 n 個字符到 str1，但是在重疊內存塊這方面，`memmove()` 是比 `memcpy()` 更安全的方法。如果目標區域和源區域有重疊的話，`memmove()` 能夠保證 str1 在被覆蓋之前將重疊區域的字節拷貝到目標區域中，複製後源區域的內容會被更改。如果目標區域與源區域沒有重疊，則和 memcpy() 函數功能相同。

* declaration: `void *memmove(void *str1, const void *str2, size_t n)`

  * `str1`: 指向用於存儲複製內容的目標數組，類型強制轉換為 void* 指針。
  * `str2`: 指向要復制的數據源，類型強制轉換為 void* 指針。
  * `n`: 要被複製的字節數

* return:

  * 返回一個指向 str 儲存區域的 pointer

* example

  ```c
  #include <string.h>
  const char dest[] = "oldstring";
  const char src[]  = "newstring";
  memmove(dest, src, 9);  // dest裡面的字符串變成newstring
  ```





## sys/mman.h



`mprotect()`: 可以用來修改一段指定內存區域的保護屬性。

* declaration: `int mprotect(const void *addr, size_t len, int prot)`

  * `addr`: 的内存空间的第一個地址
  * `len`: 需要修改保護屬性的內存區大小
  * `prot`: 修改的值
    * PROT_READ：該區段可以被讀取
    * PROT_WRITE：該區段可以被寫入
    * PROT_EXEC：該區段可以被當作機器碼執行
    * PROT_NONE：根本無法被存取

* return:

  * 如果成功，返回0
  * 如果失敗，返回-1，並設置errno，有下面4種情況
    * EACCES: 該內存不能設置為相應權限。這是可能發生的，比如，如果你 mmap(2) 映射一個文件為只讀的，接著使用 `mprotect()` 標誌為 PROT_WRITE。
    * EINVAL: addr 不是一個有效的指針，指向的不是某個內存頁的開頭。
    * ENOMEM: 內核內部的結構體無法分配。
    * ENOMEM: 進程的地址空間在區間 [addr, addr+len] 範圍內是無效，或者有一個或多個內存頁沒有映射。如果調用進程內存訪問行為侵犯了這些設置的保護屬性，內核會為該進程產生 SIGSEGV （Segmentation fault，段錯誤）信號，並且終止該進程。

* example

  ```c
  #include <sys/mman.h>
  long memsz = 0xFFFF + 1;
  unsigned char *ntvCode;
  posix_memalign((void **) &ntvCode, memsz, memsz);  // 資料對齊，並給出記憶體
  mprotect(ntvCode, memsz, PROT_READ | PROT_WRITE | PROT_EXEC);
  free(ntvCode);  // 釋放記憶體
  ```

  

## stdio.h



`perror(str)`: 打印錯誤訊息到stderr

* declaration: `void perror(const char *str)`
* return: 沒有回傳值



`fopen(str, n, file)`: 讀取 file 裡面的 n 個字元，存放到str(通常是陣列)裡面

* declaration: `char * fgets ( char * str , int n , FILE * stream )`

* return:

  * 如果讀取成功，返回值等於str (文件內容)
  * 如果讀取字元為到n個就讀到文件結束，就回傳空指標，並把內容存放到到str
  * 如果發生錯誤，返回空指標

* example

  ```c
  #include <stdio.h>
  FILE *fp = fopen("filename", "rb");  // create file pointer object
  fclose(fp);  // close file
  ```



`fseek(fp, offset, fromwhere)`: 重定位文件的讀寫pointer

* declaration: `int fseek(FILE *stream, long int offset, int whence)`

  * `stream`: 這是一個文件對象的標識流的指針。

  * `offset`: 相對於whence的文件偏移量。

  * `whence`: 位置偏移量添加。它指定由下列常數之一

    | SEEK_SET | Beginning of file                     |
    | -------- | ------------------------------------- |
    | SEEK_CUR | Current position of the file yiibaier |
    | SEEK_END | End of file                           |

* return:

  * 如果讀取成功，返回0
  * 如果發生錯誤，返回非0值

* example

  ```c
  #include <stdio.h>
  FILE *fp = fopen("filename", "rb");
  fseek(fp, 0, SEEK_SET);
  ```



`ftell(fp)`: 經常搭配`fseek()`使用，在隨機讀取文件時，由於文件位置會頻繁的前後移動，可以使用`fseek()`後再使用`ftell`，就可以很容易確認文件當前位置，使用`ftell()`也可以獲取文件長度

* declaration: `long int ftell(FILE *stream)`

  * `stream`: 這是一個文件對象的標識流的指針。

* return:

  * 如果讀取成功，返回當前位置識別符號的值
  * 如果發生錯誤，返回 -1L，全局變量 errno 設置為一個正值

* example

  ```c
  #include <stdio.h>
  FILE *fp = fopen("filename", "rb");
  fseek(fp, 0, SEEK_END);  // 設定文件到末尾
  int len = ftell(fp);  // len 可以取得filename的總大小(bit)
  fclose(fp);
  ```



`fread(ptr, size, nmemb, *stream)`: 讀取文件數據，再把數據丟到ptr所指向的地方

* declaration: `size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)`

  * `ptr`: 指向帶有最小尺寸 size*nmemb 字節的內存塊的指針。
  * `size`: 要讀取的每個元素的大小，以字節為單位。
  * `nmemb`: 元素的個數，每個元素的大小為 size 字節。
  * `stream`: 這是指向 FILE 對象的指針，該 FILE 對象指定了一個輸入流。

* return:

  * 如果讀取成功，會返回讀取元素的總數(以 `size_t` 類型返回)。
  * 如果讀取總數與 `nmemb` 參數不同，或是其他錯誤，就會回傳0。

* example

  ```c
  #include <stdio.h>
  FILE *fp = fopen("filename", "rb");
  fseek(fp, 0, SEEK_END);
  size_t ssz = ftell(fp);
  char *src = calloc(sizeof(char), ssz + 2);  // 設定pointer區域
  fread(src, sizeof(char), ssz, fp);
  fclose(fp);
  
  ```
  
  

## stdarg.h

`va_start(ap, last_arg)`: 讀取文件數據，再把數據丟到ptr所指向的地方

* declaration: `void va_start(va_list ap, last_arg)`

  * `ap`: 這是va_list的對象，將持有va_arg的額外的參數來檢索所需的信息。
  * `last_arg`: 這是最後一個已知的固定參數傳遞給函數。

* return:

  * 如果讀取成功，會返回讀取元素的總數(以 `size_t` 類型返回)。
  * 如果讀取總數與 `nmemb` 參數不同，或是其他錯誤，就會回傳0。

* example

  ```c
  #include<stdarg.h>
  #include<stdio.h>
  int32_t error(int, ...);
  
  int main(void)
  {
      // 1: can't declare variable
    	error("%d: can't declare variable", 1);
     	return 0;
  }
  
  int32_t error(char *errs, ...)
  {
      va_list args;
      va_start(args, errs);
      printf("error: ");
      vprintf(errs, args);  // 使用參數列表發送格式化輸出到 stdout
      puts("");
      va_end(args);
      exit(0);
      return 0;
  }
  
  ```

  

