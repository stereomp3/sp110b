# 習題 2 : 請為 c6 編譯器加上 do ... while 語法的程式碼

```c
// 需要在前面enum加上 Do，後面的compiler 的 p加上 do
else if (tk == Do){  
    next();
    a = e + 1;
    stmt();
    if (tk == While) next(); else { printf("%d: open while expected\n", line); exit(-1); }
    if (tk == '(') next(); else { printf("%d: open paren expected\n", line); exit(-1); }
    expr(Assign);
    if (tk == ')') next(); else { printf("%d: close paren expected\n", line); exit(-1); }
    if (tk == ';') next(); else { printf("%d: close ; expected\n", line); exit(-1); }
    *++e = BNZ; *++e = (int)a; // BNZ是代表不是 0 的時候跳，這裡是跳入迴圈
  }
```