# 習題 1 : 請為編譯器加上 do ... while 語法的程式碼

```c
#include <assert.h>
#include "compiler.h"

int E();
void STMT();
void IF();
void BLOCK();

int tempIdx = 0, labelIdx = 0;

#define nextTemp() (tempIdx++)
#define nextLabel() (labelIdx++)
#define emit printf

int isNext(char *set) { //...}

int isEnd() { //...}

char *next() { //...}

char *skip(char *set) { //...}

// F = (E) | Number | Id
int F() { //...}

// E = F (op E)*
int E() { //...}

// ASSIGN = id '=' E;
void ASSIGN() { //...}

// WHILE = while (E) STMT
void WHILE() { //...}

// DO WHILE
void DO(){
  int DoBegin = nextLabel();
  int DoEnd = nextLabel();
  emit("(L%d)\n", DoBegin);
  skip("do");
  STMT();  // block call STMTS ， } to end
  skip("while");
  skip("(");
  int e = E();
  emit("if not T%d goto L%d\n", e, DoEnd);
  skip(")");
  skip(";");  // 需要過濾掉，不然一開始在main() 執行的 STMTS會觸發 ASSIGN() 裡面的 skip("=") 導致錯誤
  emit("goto L%d\n", DoBegin);
  emit("(L%d)\n", DoEnd);
}


// STMT = WHILE | BLOCK | ASSIGN
void STMT() {
  if (isNext("do")){
    DO();
  }
  else if(isNext("while"))
    WHILE();
  // else if (isNext("if"))
  //   IF();
  else if (isNext("{"))
    BLOCK();
  else
    ASSIGN();
}

// STMTS = STMT*
void STMTS() {//...}

// BLOCK = { STMTS }
void BLOCK() { //...}

// PROG = STMTS
void PROG() { //...}

void parse() {
  printf("============ parse =============\n");
  tokenIdx = 0;
  PROG();
}
```

> 執行結果

```sh
============ parse =============
t0 = 0
s = t0
t1 = 1
i = t1
(L0)
t2 = s
t3 = i
t4 = t2 + t3
s = t4
t5 = i
t6 = 1
t7 = t5 + t6
i = t7
t8 = i
t9 = 10
t10 = t8 < t9
if not T10 goto L1
goto L0
(L1)
```