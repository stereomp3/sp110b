## 參考資料

> 程式碼來源: [parser.c](https://github.com/embedded2015/rubi/blob/master/parser.c)、[parser.h](https://github.com/embedded2015/rubi/blob/master/parser.h)



## parser.h

```c
#ifndef RUBI_PARSER_INCLUDED
#define RUBI_PARSER_INCLUDED

#include "rubi.h"

typedef struct {
    int address, args;
    char name[0xFF];
} func_t;

int expression(int, int);

int parser();
int getString();

func_t *getFunc(char *);

// 紀錄型態，在declareVariable()裡面會使用appendVar()把型態放入，最後會由assignment()判別
typedef struct {
    char name[32];
    unsigned int id;
    int type;
    int loctype;
} Variable;  
Variable *getVar(char *);

int isassign();
int assignment();

#endif
```





## parser.c

> 程式碼主要處理rubi文件內容的script

* `int32_t getString()`: 只有在螢幕輸出的時候才回用到 (output put printf)，拿取tok裡面的文字存到strings結構裡面
* `Variable *getVar(char *name)`: 讀取變數，會在assignment()一開始存入 `Variable *v`
* `Variable *appendVar(char *name, int type)`: 判斷變數是設函式裡面的變數還是廣域變數分別存入locVar結構和gblVar結構裡面，這個function主要被`declareVariable()`呼叫
* `func_t *getFunc(char *name)`: 拿取functions裡面存的函數名稱，如果名稱有加進去，就傳回函數地址
* `func_t *appendFunc(char *name, int address, int args)`: 加入函數到functons結構
* `int32_t appendBreak()`: break時使用
* `int32_t appendReturn()`: return時使用
* `int32_t skip(char *s)`:  比對字符，如果現在位置的字符串 = s，就往下走
* `int32_t error(char *errs, ...)`:  發送錯誤消息，使用參數列表發送格式化輸出到 stdout
* `int eval(int pos, int status)`: 用來遍歷tok，status是代表在執行function或是一般程式
* `Variable *declareVariable()`: 對變數進行處理，根據不同型態傳回不一樣答案
* `int ifStmt()`: if的時候用
* `int whileStmt()`: while的時候用
* `int32_t functionStmt()`: 出現def就會觸發這個函數，會設定 `functions.inside = IN_FUNC`，表示現在是在函數裡面
* `int expression(int pos, int status)`: 判斷文件內容的結構(if、for、def、puts、$....)，根據不同的字串，做出不同的行為
* `char *replaceEscape(char *str)`: 在parser裡面會用到，跟字串處理相關
* `int32_t parser()`: 程式碼進入點，會觸發`eval(0, 0)`，然後開始運行，把整個程式碼做快速編譯
* `int32_t isassign()`: 用來判斷特殊符號(=、++、--、[、:)是否存在，如果存在就執行assignment()
* `int32_t assignment()`: 針對變數做記憶體分配處理

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "parser.h"
#include "expr.h"
#include "asm.h"

struct {
    Variable var[0xFF];
    int count;  // 紀錄連接變數的對象數量，如果等於0就銷毀變數
} gblVar;

static Variable locVar[0xFF][0xFF];
static int varSize[0xFF], varCounter;

static int nowFunc; // number of function                                              
struct {
    char *text[0xff];  // 拿取Token裡面的val
    int *addr;  // 讀取ntvCount
    int count;  // 計算strings的大小
} strings;  

struct {
    func_t func[0xff];
    int count, inside;
} functions;

#define NON 0

int32_t getString()  // 只有在螢幕輸出的時候才回用到 (output put printf)
{
    // 拿取tok裡面的文字
    strings.text[ strings.count ] =
        calloc(sizeof(char), strlen(tok.tok[tok.pos].val) + 1);
    strcpy(strings.text[strings.count], tok.tok[tok.pos++].val);
	
    *strings.addr++ = ntvCount;
    return strings.count++;  // count 代表一句話的長度
}

Variable *getVar(char *name)  // 讀取變數，會在assignment()一開始存入 Variable *v
{
    /* local variable */
    for (int i = 0; i < varCounter; i++) {
        if (!strcmp(name, locVar[nowFunc][i].name))
            return &locVar[nowFunc][i];
    }
    /* global variable */
    for (int i = 0; i < gblVar.count; i++) {
        if (!strcmp(name, gblVar.var[i].name))
            return &gblVar.var[i];
    }
    return NULL;
}

static Variable *appendVar(char *name, int type)
{
    /*
    	利用IN_FUNC和IN_GLOBAL判斷變數是設函式裡面的變數還是廣域變數
    	分別存入locVar結構和gblVar結構裡面，並記錄類型
    	declareVariable()會return這個funtion
    */
    if (functions.inside == IN_FUNC) {
        int32_t sz = 1 + ++varSize[nowFunc];
        strcpy(locVar[nowFunc][varCounter].name, name);  // 紀錄區域變量
        locVar[nowFunc][varCounter].type = type;  // 紀錄類型
        locVar[nowFunc][varCounter].id = sz;
        locVar[nowFunc][varCounter].loctype = V_LOCAL;

        return &locVar[nowFunc][varCounter++];
    } else if (functions.inside == IN_GLOBAL) {
        /* global varibale */
        strcpy(gblVar.var[gblVar.count].name, name);  // 紀錄全局變量
        gblVar.var[gblVar.count].type = type;  // 紀錄類型
        gblVar.var[gblVar.count].loctype = V_GLOBAL;
        gblVar.var[gblVar.count].id = (uint32_t) &ntvCode[ntvCount];  // 紀錄地址
        ntvCount += sizeof(int32_t); /* type */  // 因為沒有使用emit，所以要手動前進 

        return &gblVar.var[gblVar.count++];
    }
    return NULL;
}

func_t *getFunc(char *name)  // 拿取functions裡面存的函數名稱，如果名稱有加進去，就傳回函數地址
{
    for (int i = 0; i < functions.count; i++) {
        if (!strcmp(functions.func[i].name, name))
            return &functions.func[i];
    }
    return NULL;
}

static func_t *appendFunc(char *name, int address, int args)  // 加入函數到functons結構
{
    functions.func[functions.count].address = address;
    functions.func[functions.count].args = args;
    strcpy(functions.func[functions.count].name, name);
    return &functions.func[functions.count++];
}

static int32_t appendBreak()  // break時使用
{
    emit(0xe9); // jmp
    brks.addr = realloc(brks.addr, 4 * (brks.count + 1));
    brks.addr[brks.count] = ntvCount;
    emitI32(0);
    return brks.count++;
}

static int32_t appendReturn()  // return時使用
{
    relExpr(); /* get argument */
    emit(0xe9); // jmp
    rets.addr = realloc(rets.addr, 4 * (rets.count + 1));
    if (rets.addr == NULL) error("no enough memory");
    rets.addr[rets.count] = ntvCount;
    emitI32(0);
    return rets.count++;
}

int32_t skip(char *s)  // 比對字符，如果現在位置的字符串 = s，就往下走
{
    if (!strcmp(s, tok.tok[tok.pos].val)) {
        tok.pos++;
        return 1;
    }
    return 0;
}

int32_t error(char *errs, ...)  // 發送錯誤消息
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

static int eval(int pos, int status)  // 用來遍歷tok，status是代表在執行function或是一般程式
{
    while (tok.pos < tok.size)
        if (expression(pos, status)) return 1;
    return 0;
}

static Variable *declareVariable()  // 對變數進行處理，根據不同型態傳回不一樣答案
{
    int32_t npos = tok.pos;
    if (isalpha(tok.tok[tok.pos].val[0])) {
        tok.pos++;
        if (skip(":")) {
            if (skip("int")) {
                --tok.pos;
                return appendVar(tok.tok[npos].val, T_INT);
            }
            if (skip("string")) {
                --tok.pos;
                return appendVar(tok.tok[npos].val, T_STRING);
            }
            if (skip("double")) {
                --tok.pos;
                return appendVar(tok.tok[npos].val, T_DOUBLE);
            }
        } else {
            --tok.pos;
            return appendVar(tok.tok[npos].val, T_INT);
        }
    } else error("%d: can't declare variable", tok.tok[tok.pos].nline);
    return NULL;
}

static int ifStmt()
{
    uint32_t end;
    relExpr(); /* if condition */
    emit(0x83); emit(0xf8); emit(0x00);// cmp eax, 0
    emit(0x75); emit(0x05); // jne 5
    emit(0xe9); end = ntvCount; emitI32(0);// jmp
    return eval(end, NON);
}

static int whileStmt()
{
    // loopBgn = while一開始的位置, end = while末尾位置
    uint32_t loopBgn = ntvCount, end, stepBgn[2], stepOn = 0;
    relExpr(); /* condition */
    if (skip(",")) {
        stepOn = 1;
        stepBgn[0] = tok.pos;
        for (; tok.tok[tok.pos].val[0] != ';'; tok.pos++)
            /* next */;
    }
    emit(0x83); emit(0xf8); emit(0x00);// cmp eax, 0
    emit(0x75); emit(0x05); // jne 5
    emit(0xe9); end = ntvCount; emitI32(0);// jmp while end

    if (skip(":")) expression(0, BLOCK_LOOP);
    else eval(0, BLOCK_LOOP);

    if (stepOn) {
        stepBgn[1] = tok.pos;
        tok.pos = stepBgn[0];
        if (isassign()) assignment();
        tok.pos = stepBgn[1];
    }
    uint32_t n = 0xFFFFFFFF - ntvCount + loopBgn - 4;
    emit(0xe9); emitI32(n); // jmp n
    emitI32Insert(ntvCount - end - 4, end);

    for (--brks.count; brks.count >= 0; brks.count--)
        emitI32Insert(ntvCount - brks.addr[brks.count] - 4,
                      brks.addr[brks.count]);
    brks.count = 0;

    return 0;
}

static int32_t functionStmt()
{
    int32_t espBgn, argsc = 0;
    char *funcName = tok.tok[tok.pos++].val;
    nowFunc++; functions.inside = IN_FUNC;
    if (skip("(")) {
        do {
            declareVariable();
            tok.pos++;
            argsc++;
        } while(skip(","));
        if (!skip(")"))
                error("%d: expecting ')'", tok.tok[tok.pos].nline);
    }
    appendFunc(funcName, ntvCount, argsc); // append function
    emit(0x50 + EBP); // push ebp
    emit(0x89); emit(0xc0 + ESP * 8 + EBP); // mov ebp esp
    espBgn = ntvCount + 2;
    emit(0x81); emit(0xe8 + ESP); emitI32(0); // sub esp 0 ; align
    int32_t argpos[128], i;
    for(i = 0; i < argsc; i++) {
        emit(0x8b); emit(0x45); emit(0x08 + (argsc - i - 1) * sizeof(int32_t));
        emit(0x89); emit(0x44); emit(0x24);
        argpos[i] = ntvCount; emit(0x00);
    }
    eval(0, BLOCK_FUNC);

    for (--rets.count; rets.count >= 0; --rets.count) {
        emitI32Insert(ntvCount - rets.addr[rets.count] - 4,
                      rets.addr[rets.count]);
    }
    rets.count = 0;

    emit(0x81); emit(0xc0 + ESP);
    emitI32(sizeof(int32_t) * (varSize[nowFunc] + 6)); // add esp nn
    emit(0xc9); // leave
    emit(0xc3); // ret

    emitI32Insert(sizeof(int32_t) * (varSize[nowFunc] + 6), espBgn);
    for (i = 1; i <= argsc; i++)
        ntvCode[argpos[i - 1]] = 256 - sizeof(int32_t) * i +
            (((varSize[nowFunc] + 6) * sizeof(int32_t)) - 4);

    return 0;
}

int expression(int pos, int status)
{
    /*
    	判斷文件內容的結構(if、for、def、puts、$....)，根據不同的字串，做出不同的行為
    */
    int isputs = 0;

    if (skip("$")) { // global varibale?
        if (isassign()) assignment();
    } else if (skip("def")) {
        functionStmt();
    } else if (functions.inside == IN_GLOBAL &&  // 不在function內
               strcmp("def", tok.tok[tok.pos+1].val) &&  // 遇到 def
               strcmp("$", tok.tok[tok.pos+1].val) &&
               strcmp(";", tok.tok[tok.pos+1].val)) { // main function entry
        functions.inside = IN_FUNC;
        nowFunc++;
        appendFunc("main", ntvCount, 0); // append function
        emit(0x50 + EBP); // push ebp
        emit(0x89); emit(0xc0 + ESP * 8 + EBP); // mov ebp esp
        uint32_t espBgn = ntvCount + 2;
        emit(0x81); emit(0xe8 + ESP); emitI32(0); // sub esp 0
        emit(0x8b); emit(0x75); emit(0x0c); // mov esi, 0xc(%ebp)

        eval(0, NON);

        emit(0x81); emit(0xc4);
        emitI32(sizeof(int32_t) * (varSize[nowFunc] + 6)); // add %esp nn
        emit(0xc9);// leave
        emit(0xc3);// ret
        emitI32Insert(sizeof(int32_t) * (varSize[nowFunc] + 6), espBgn);
        functions.inside = IN_GLOBAL;
    } else if (isassign()) {
        assignment();
    } else if ((isputs = skip("puts")) || skip("output")) {
        do {
            int isstring = 0;
            if (skip("\"")) {
                emit(0xb8); getString(); emitI32(0x00);
                    // mov eax string_address
                isstring = 1;
            } else {
                relExpr();
            }
            emit(0x50 + EAX); // push eax
            if (isstring) {
                emit(0xff); emit(0x56); emit(4);// call *0x04(esi) putString
            } else {
                emit(0xff); emit(0x16); // call (esi) putNumber
            }
            emit(0x81); emit(0xc0 + ESP); emitI32(4); // add esp 4
        } while (skip(","));
        /* new line ? */
        if (isputs) {
            emit(0xff); emit(0x56); emit(8);// call *0x08(esi) putLN
        }
    } else if(skip("printf")) {
        if (skip("\"")) {
            emit(0xb8); getString(); emitI32(0x00); // mov eax string_address
            emit(0x89); emit(0x44); emit(0x24); emit(0x00); // mov [esp+0], eax
        }
        if (skip(",")) {
            uint32_t a = 4;
            do {
                relExpr();
                emit(0x89); emit(0x44); emit(0x24); emit(a); // mov [esp+a], eax
                a += 4;
            } while(skip(","));
        }
        emit(0xff); emit(0x56); emit(12 + 8); // call printf
    } else if (skip("for")) {
        assignment();
        if (!skip(","))
            error("%d: expecting ','", tok.tok[tok.pos].nline);
        whileStmt();
    } else if (skip("while")) {
        whileStmt();
    } else if(skip("return")) {
        appendReturn();
    } else if(skip("if")) {
        ifStmt();
    } else if(skip("else")) {
        int32_t end;
        emit(0xe9); end = ntvCount; emitI32(0);// jmp while end
        emitI32Insert(ntvCount - pos - 4, pos);
        eval(end, NON);
        return 1;
    } else if (skip("elsif")) {
        int32_t endif, end;
        emit(0xe9); endif = ntvCount; emitI32(0);// jmp while end
        emitI32Insert(ntvCount - pos - 4, pos);
        relExpr(); /* if condition */
        emit(0x83); emit(0xf8); emit(0x00);// cmp eax, 0
        emit(0x75); emit(0x05); // jne 5
        emit(0xe9); end = ntvCount; emitI32(0);// jmp while end
        eval(end, NON);
        emitI32Insert(ntvCount - endif - 4, endif);
        return 1;
    } else if (skip("break")) {
        appendBreak();
    } else if (skip("end")) {
        if (status == NON) {
            emitI32Insert(ntvCount - pos - 4, pos);
        } else if (status == BLOCK_FUNC) functions.inside = IN_GLOBAL;
        return 1;
    } else if (!skip(";")) { relExpr(); }

    return 0;
}

static char *replaceEscape(char *str)
{
    char escape[12][3] = {
        "\\a", "\a", "\\r", "\r", "\\f", "\f",
        "\\n", "\n", "\\t", "\t", "\\b", "\b"
    };
    for (int32_t i = 0; i < 12; i += 2) {
        char *pos;
        while ((pos = strstr(str, escape[i])) != NULL) {
            *pos = escape[i + 1][0];
            // ?? 把區域變數的string改變?? 外面也沒有用到，這邊是在做什麼 ??
            memmove(pos + 1, pos + 2, strlen(pos + 2) + 1);  
        }
    }
    return str;
}

int32_t parser()
{
    tok.pos = ntvCount = 0;
    strings.addr = calloc(0xFF, sizeof(int32_t));
    uint32_t main_address;
    emit(0xe9); main_address = ntvCount; emitI32(0);
    eval(0, 0);  // 開始運行expression

    uint32_t addr = getFunc("main")->address;
    emitI32Insert(addr - 5, main_address);

    // for (一開始先做什麼事 ;條件式 ;等每作完一次，就做什麼事){當條件成立(true)時，就重覆做的事...}
    // 
    for (strings.addr--; strings.count; strings.addr--) {
        emitI32Insert((uint32_t) &ntvCode[ntvCount], *strings.addr);
        replaceEscape(strings.text[--strings.count]);
        for (int32_t i = 0; strings.text[strings.count][i]; i++)
            emit(strings.text[strings.count][i]);
        emit(0); // '\0'
    }

    return 1;
}

int32_t isassign()  // 用來判斷特殊符號(=、++、--、[、:)是否存在，如果存在就執行assignment()
{
    char *val = tok.tok[tok.pos + 1].val;
    if (!strcmp(val, "=") || !strcmp(val, "++") || !strcmp(val, "--")) return 1;
    if (!strcmp(val, "[")) {
        int32_t i = tok.pos + 2, t = 1;
        while (t) {
            val = tok.tok[i].val;
            if (!strcmp(val, "[")) t++; if (!strcmp(val, "]")) t--;
            if (!strcmp(val, ";"))
                error("%d: invalid expression", tok.tok[tok.pos].nline);
            i++;
        }
        if (!strcmp(tok.tok[i].val, "=")) return 1;
    } else if (!strcmp(val, ":") && !strcmp(tok.tok[tok.pos + 3].val, "=")) {
        return 1;
    }
    return 0;
}

int32_t assignment()  // 針對變數做記憶體分配處理
{
    Variable *v = getVar(tok.tok[tok.pos].val);
    int32_t inc = 0, dec = 0, declare = 0;
    if (v == NULL) {
        declare++;
        v = declareVariable();
    }
    tok.pos++;

    if (v->loctype == V_LOCAL) {
        if (skip("[")) { // Array?
            relExpr();
            emit(0x50 + EAX); // push eax
            if (skip("]") && skip("=")) {
                relExpr();
                emit(0x8b); emit(0x4d);
                emit(256 -
                        (v->type == T_INT ? sizeof(int32_t) :
                         v->type == T_STRING ? sizeof(int32_t *) :
                         v->type == T_DOUBLE ? sizeof(double) : 4) * v->id);
                    // mov ecx [ebp-n]
                emit(0x58 + EDX); // pop edx
                if (v->type == T_INT) {
                    emit(0x89); emit(0x04); emit(0x91); // mov [ecx+edx*4], eax
                } else {
                    emit(0x89); emit(0x04); emit(0x11); // mov [ecx+edx], eax
                }
            } else if ((inc = skip("++")) || (dec = skip("--"))) {
            } else 
                error("%d: invalid assignment", tok.tok[tok.pos].nline);
        } else { // Scalar?
            if(skip("=")) {
                relExpr();
            } else if((inc = skip("++")) || (dec = skip("--"))) {
                emit(0x8b); emit(0x45);
                emit(256 -
                        (v->type == T_INT ? sizeof(int32_t) :
                         v->type == T_STRING ? sizeof(int32_t *) :
                         v->type == T_DOUBLE ? sizeof(double) : 4) * v->id);
                    // mov eax varaible
                emit(0x50 + EAX); // push eax
                if (inc) emit(0x40); // inc eax
                else if(dec) emit(0x48); // dec eax
            }
            emit(0x89); emit(0x45);
            emit(256 -
                    (v->type == T_INT ? sizeof(int32_t) :
                     v->type == T_STRING ? sizeof(int32_t *) :
                     v->type == T_DOUBLE ? sizeof(double) : 4) * v->id);
                // mov var eax
            if (inc || dec) emit(0x58 + EAX); // pop eax
        }
    } else if (v->loctype == V_GLOBAL) {
        if (declare) { // first declare for global variable?
            // assignment only int32_terger
            if (skip("=")) {
                unsigned *m = (unsigned *) v->id;
                *m = atoi(tok.tok[tok.pos++].val);
            }
        } else {
            if (skip("[")) { // Array?
                relExpr();
                emit(0x50 + EAX); // push eax
                if(skip("]") && skip("=")) {
                    relExpr();
                    emit(0x8b); emit(0x0d); emitI32(v->id);
                        // mov ecx GLOBAL_ADDR
                    emit(0x58 + EDX); // pop edx
                    if (v->type == T_INT) {
                        emit(0x89); emit(0x04); emit(0x91);
                            // mov [ecx+edx*4], eax
                    } else {
                        emit(0x89); emit(0x04); emit(0x11);
                            // mov [ecx+edx], eax
                    }
                } else error("%d: invalid assignment",
                             tok.tok[tok.pos].nline);
            } else if (skip("=")) {
                relExpr();
                emit(0xa3); emitI32(v->id); // mov GLOBAL_ADDR eax
            } else if ((inc = skip("++")) || (dec = skip("--"))) {
                emit(0xa1); emitI32(v->id); // mov eax GLOBAL_ADDR
                emit(0x50 + EAX); // push eax
                if (inc) emit(0x40); // inc eax
                else if (dec) emit(0x48); // dec eax
                emit(0xa3); emitI32(v->id); // mov GLOBAL_ADDR eax
            }
            if (inc || dec) emit(0x58 + EAX); // pop eax
        }
    }
    return 0;
}
```



## q

* getFunc
* 為甚麼parser前面要填充機器碼??
* 為甚麼不管是在`calloc()`或是`memmove()`的時候，都要把size+1 ?
* `replaceEscape()` 在 `parser()`的功能是什麼??，其實整個parser唯一知道在做什麼的好像只有eval...





`parser.c`原文註解45行拼錯字 XD