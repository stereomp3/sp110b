## 參考資料

> 程式碼來源: [asm.h](https://github.com/embedded2015/rubi/blob/master/asm.h)、[rubi.h](https://github.com/embedded2015/rubi/blob/master/rubi.h)



## asm.h

這裡把機器碼直接丟入記憶體的位置裡面

```c
#ifndef RUBI_ASM_INCLUDED
#define RUBI_ASM_INCLUDED

#include <stdint.h>

// unsigned char value: 0~255(2^8-1)/0~ff
// unsigned int value: 0~65535(2^16-1)/0~ffff
unsigned char *ntvCode;  
int ntvCount;

// 定義基址暫存器 0、1、2、3、4、5、6、7
enum { EAX = 0, ECX, EDX, EBX, ESP, EBP, ESI, EDI };

static inline void emit(unsigned char val)
{
    ntvCode[ntvCount++] = (val);
}
// big-endian、little-endian
static inline void emitI32(unsigned int val)  // unsigned int ? 16byte
{
    // 使用位移運算，處理32位元的數，把它分成4部分 4*8
    // << 左位移 二進位數算術左移1位。最右邊的位置則用零填充。相當於乘法
    // >> 右位移 二進位數算術右移1位。最左邊的數值不變。相當於省略小數點的除法
    emit(val << 24 >> 24);  
    emit(val << 16 >> 24);
    emit(val <<  8 >> 24);
    emit(val <<  0 >> 24);
}
static inline void emitI32Insert(unsigned int val, int pos)
{
    // 在指定位置插入
    ntvCode[pos + 0] = (val << 24 >> 24);
    ntvCode[pos + 1] = (val << 16 >> 24);
    ntvCode[pos + 2] = (val <<  8 >> 24);
    ntvCode[pos + 3] = (val <<  0 >> 24);
}

#endif
```





## rubi.h

```c
#ifndef RUBI_INCLUDED
#define RUBI_INCLUDED

enum { IN_GLOBAL = 0, IN_FUNC };  // IN_GLOBAL 代表在function外，也是funtions.inside的預設

enum { BLOCK_LOOP = 1, BLOCK_FUNC };

// 定義結構，val用來存rubi文件的內容，因為是弱型態，所以固定32bit
// nline是存這個是第幾行，方便報錯的時候顯示
typedef struct { char val[32]; int nline; } Token;
struct {
    Token *tok;  // 多個tok紀錄所有文件內容，利用pos定位
    int size, pos;  // size用來存tok最大大小，pos用來定位(巡徑)
} tok;

enum { V_LOCAL, V_GLOBAL };

enum { T_INT, T_STRING, T_DOUBLE };

// brks 用來記錄跳出點，可以用在 break 和 whileStmt 裡面
// rets 用來記錄返回點，用在 return 和 functionStmt 裡面
struct {
    unsigned int *addr;
    int count;
} brks, rets;

int error(char *, ...);

#endif
```





## q

* unsigned int 是 16 bit，為甚麼可以塞入 32 bit 的東西然後運算 ?
* 為甚麼他要使用emitI32一次插入四個，是因為方便嗎 ??
