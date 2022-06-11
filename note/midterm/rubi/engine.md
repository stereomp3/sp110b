## 參考資料

> 程式碼來源: [engine.c](https://github.com/embedded2015/rubi/blob/master/engine.c)





## engine.c

> 主要程式進入點，裡面分配記憶體(ntvCode、mem)，和運行所有附加的程式

* `void init()`: 初始化內存
* `void freeAddr()`:  把mem結構的內存清除
* `void dispose()`: 把記憶體釋放
* `lex(char *code)`: 把文件內容整理並放到tok結構裡面，使用for迴圈，判斷每個字符，並記錄第幾行(`tok.tok[tok.pos].nline`)，如果文件過大，
* `int xor128()`: 裡面一堆怪怪的東西，目前不知道作用 ?
* `void *funcTable[]`: 存放函數的位置，這裡跟`stdlib.c`裡面的`std_function stdfunc[]`有相關性
* `int execute(char *source)`: 拿到main傳來的文件內容，首先先經過lex，把文件內容整理並放入tok裡面
* `main(int argc, char **argv)`: 主要程式運行點，在輸入`./rubi [filename]`時，會讀取filename，然後使用`fopen`讀取文件，然後用`fread`把文件內容丟到一個pointer裡面傳給execute

```c
#include "rubi.h"
#include "parser.h"
#include "asm.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/wait.h>

struct {
    uint32_t addr[0xff];
    int count;
} mem;

static unsigned int w;
static void set_xor128() { w = 1234 + (getpid() ^ 0xFFBA9285); }

void init()
{
    long memsz = 0xFFFF + 1;
    // 分配動態記憶體給 ntvcode
    if (posix_memalign((void **) &ntvCode, memsz, memsz))  // 為了要符合函數定義，所以要轉型
        perror("posix_memalign");  // 如果不等於 0，代表出錯，打印錯誤到stderr
    // 設置記憶體權限，可讀，可修改，可用機器碼執行，這裡是快速編譯的關鍵
    if (mprotect(ntvCode, memsz, PROT_READ | PROT_WRITE | PROT_EXEC))
        perror("mprotect");
    tok.pos = ntvCount = 0;  // 設定初始位置(ntvCode[ntvCount])
    tok.size = 0xfff;
    set_xor128();
    tok.tok = calloc(sizeof(Token), tok.size);
    brks.addr = calloc(sizeof(uint32_t), 1);
    rets.addr = calloc(sizeof(uint32_t), 1);
}

static void freeAddr()
{
    if (mem.count > 0) {
        for (--mem.count; mem.count >= 0; --mem.count)
            free((void *)mem.addr[mem.count]);
        mem.count = 0;
    }
}

void dispose()
{
    free(ntvCode);
    free(brks.addr);
    free(rets.addr);
    free(tok.tok);
    freeAddr();
}

int32_t lex(char *code)  // 拿到main給的文件內容pointer src，分析裡面的語法，並存入到tok
{
    int32_t codeSize = strlen(code), line = 1;
    int32_t is_crlf = 0;

    for (int32_t i = 0; i < codeSize; i++) {
        if (tok.size <= i)  // 如果文件比預期的大，使用realloc重新分配記憶體，加大空間
            tok.tok = realloc(tok.tok, (tok.size += 512 * sizeof(Token)));
        if (isdigit(code[i])) { // number?
            for (; isdigit(code[i]); i++)  // 判斷下個是不是數字
                strncat(tok.tok[tok.pos].val, &(code[i]), 1);
            tok.tok[tok.pos].nline = line;  // 紀錄這個字符是第幾行
            i--;  // 把i數字減回來
            tok.pos++;  // 切換到下一個位置
        } else if (isalpha(code[i])) { // ident?
            char *str = tok.tok[tok.pos].val;
            // 判斷下個是不是字符
            for (; isalpha(code[i]) || isdigit(code[i]) || code[i] == '_'; i++)
                *str++ = code[i];
            tok.tok[tok.pos].nline = line;
            i--;  // 把i數字減回來
            tok.pos++;
        } else if (code[i] == ' ' || code[i] == '\t') { // space or tab?
        } else if (code[i] == '#') { // comment? 
            // 註解和空白和tab都不用紀錄，不用tok.pos++
// for (一開始先做什麼事 ;條件式 ;等每作完一次，就做什麼事){當條件成立(true)時，就重覆做的事...}
            for (i++; code[i] != '\n'; i++) { } line++;  // 值到跳行才結束，並加一行
        } else if (code[i] == '"') { // string?
            strcpy(tok.tok[tok.pos].val, "\"");
            tok.tok[tok.pos++].nline = line;
            for (i++; code[i] != '"' && code[i] != '\0'; i++)
                strncat(tok.tok[tok.pos].val, &(code[i]), 1);
            tok.tok[tok.pos].nline = line;
            if (code[i] == '\0')
                error("%d: expected expression '\"'",
                      tok.tok[tok.pos].nline);
            tok.pos++;
        } else if (code[i] == '\n' ||
                   (is_crlf = (code[i] == '\r' && code[i + 1] == '\n'))) {
            i += is_crlf;
            strcpy(tok.tok[tok.pos].val, ";");
            tok.tok[tok.pos].nline = line++;  // 只有跳行時才會 line++
            tok.pos++;
        } else {
            strncat(tok.tok[tok.pos].val, &(code[i]), 1);
            if (code[i + 1] == '=' || (code[i] == '+' && code[i + 1] == '+') ||
                (code[i] == '-' && code[i + 1] == '-'))
                strncat(tok.tok[tok.pos].val, &(code[++i]), 1);
            tok.tok[tok.pos].nline = line;
            tok.pos++;
        }
    }
    tok.tok[tok.pos].nline = line;
    tok.size = tok.pos - 1;  // pos用來巡徑，size紀錄最大位置

    return 0;
}

static void put_i32(int32_t n) { printf("%d", n); }
static void put_str(int32_t *n) { printf("%s", (char *) n); }
static void put_ln() { printf("\n"); }

static void ssleep(uint32_t t) { usleep(t * CLOCKS_PER_SEC / 1000); }

static void add_mem(int32_t addr) { mem.addr[mem.count++] = addr; }

static int xor128()  // random函數，使用xor128
{
    static uint32_t x = 123456789, y = 362436069, z = 521288629;
    uint32_t t;
    t = x ^ (x << 11);
    x = y; y = z; z = w;
    w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    return ((int32_t) w < 0) ? -(int32_t) w : (int32_t) w;
}

static void *funcTable[] = {  // function pointer 的大小為 4 bit
    put_i32, /*  0 */ put_str, /*  4 */ put_ln, /*   8 */ malloc, /* 12 */
    xor128,  /* 16 */ printf,  /* 20 */ add_mem, /* 24 */ ssleep, /* 28 */
    fopen,   /* 32 */ fprintf, /* 36 */ fclose,  /* 40 */ fgets,   /* 44 */
    free,    /* 48 */ freeAddr  /* 52 */
};

static int execute(char *source)
{
    init();  // 初始化內存
    lex(source);  // 把rubi文件內容複製到自訂結構tok
    parser();   // 解析語句
    // 設定funcTable為基底位置，使用對函數的位移來呼叫function
    ((int (*)(int *, void **)) ntvCode)(0, funcTable);
    dispose();  // 執行完畢，釋放內存
    return 0;
}

int main(int argc, char **argv)
{
    char *src;

    if (argc < 2) error("no given filename");

    FILE *fp = fopen(argv[1], "rb");  // ./rubi filename，獲取filename文件
    size_t ssz = 0;

    if (!fp) {
        perror("file not found");
        exit(0);
    }
    fseek(fp, 0, SEEK_END);  // 先設定文件到末尾，這樣下面的ftell才可以拿到正確的大小
    ssz = ftell(fp);  // 取得filename的總大小
    fseek(fp, 0, SEEK_SET);  // 文件定位在文件開頭，offset = 0
    src = calloc(sizeof(char), ssz + 2);  // 分配記憶體，這邊不知道為甚麼要加上2 XD
    fread(src, sizeof(char), ssz, fp);  // 把fp內容丟到src區域裡面
    fclose(fp);  

    return execute(src);  // 編譯程式開始執行
}
```



## q

* `*funcTable[]`: 裡面為甚麼可以直接放函式名稱?
  * 裡面放的是函數指標
* `int xor128()`、`void set_xor128()`: 這兩個是要做什麼用的，還有w是什麼東東 ???
* `((int (*)(int *, void **)) ntvCode)(0, funcTable)`: 這個到底在幹嘛?????，funcTable和`stdlib.c`的`std_function stdfunc[]`的關聯性如何建立????
* mem在哪裡用到 ??
