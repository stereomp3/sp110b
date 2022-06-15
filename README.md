# 課程:系統程式 -- 筆記、習題與報告

欄位 | 內容
-----|--------
學期 | 110 學年度下學期
學生 |  魏仲彥
學號末兩碼 | 41
教師 | [陳鍾誠](https://www.nqu.edu.tw/educsie/index.php?act=blog&code=list&ids=4)
學校科系 | [金門大學資訊工程系](https://www.nqu.edu.tw/educsie/index.php)
課程內容 | https://gitlab.com/ccc110/sp/
教材網址 | [陳鍾誠的系統程式課程](https://kinmen6.com/root/%E9%99%B3%E9%8D%BE%E8%AA%A0/%E8%AA%B2%E7%A8%8B/%E7%B3%BB%E7%B5%B1%E7%A8%8B%E5%BC%8F/README.md)
[期中作業](https://github.com/stereomp3/sp110b/tree/master/note/midterm) | 研究 [rubi 編譯器](https://github.com/embedded2015/rubi) 後寫成報告 
[期末作業](#System-Program) | [課堂筆記](#System Program)                                  
課堂加分題 | [HW1](work/HW1)、[HW2](work/HW2)、[HW3](work/jit) 





# System Program



|           week           | content                                                      | date       |
| :----------------------: | :----------------------------------------------------------- | ---------- |
|   [week01](note/01.md)   | [環境設定](note/01.md#環境設定)<br />[使用GCC執行C](note/01.md#使用GCC執行C)<br />[使用g++執行C++](note/01.md#使用g++執行C++)<br />[反組譯目的碼](note/01.md#反組譯目的碼)<br />[使用GDB進行Debug](note/01.md#使用GDB進行Debug)<br /> | 2022/02/14 |
| **[week02](note/02.md)** | [C](note/02.md#C)<ul><li>[C 常錯寫法](note/02.md#C-常錯寫法)</li><li>[動態分配空間](note/02.md#動態分配空間) </li><li>[C-陣列陷阱](note/02.md#C-陣列陷阱)</li><li>[微積分程式碼](note/02.md#微積分程式碼)</li><li>[C 觀念](note/02.md#C-觀念)</li></ul>[CPU0處理器](note/02.md#CPU0處理器)<ul><li>[CPU0 暫存器](note/02.md#CPU0-暫存器) </li><li>[CPU0 指令集](note/02.md#CPU0-指令集) </li><li>[CPU0指令格式](note/02.md#CPU0指令格式) </li><li>[CPU0 狀態暫存器](note/02.md#CPU0-狀態暫存器) </li><li>[定址模式](note/02.md#定址模式) </li><li>[CPU0的程式執行](note/02.md#CPU0的程式執行) </li></ul>[組譯器](note/02.md#組譯器)<ul><li>[映像檔](note/02.md#映像檔) </li><li>[組譯器演算法](note/02.md#組譯器演算法)</li></ul>[連結與載入](note/02.md#連結與載入)<ul><li>[簡介](note/02.md#簡介) </li><li>[載入器](note/02.md#載入器)</li></ul> | 2022/02/23 |
|   [week03](note/03.md)   | [語法理論](note/03.md#語法理論)<br />[正規表達式](note/03.md#正規表達式)<br />[C0語言](note/03.md#C0語言)<br />[程式碼解說 03-compiler](note/03.md#程式碼解說-03-compiler)<ul><li>[gen](note/03.md#00-gen) </li><li>[exp0](note/03.md#01-exp0)</li></ul> | 2022/03/02 |
|   [week04](note/04.md)   | [程式碼解說 03-compiler(續)](note/04.md#程式碼解說-03-compiler續)<ul><li>[exp0](note/04.md#程式碼解說-03-compiler續) </li><li>[lexer](note/04.md#lexer)</li><li>[03a-compiler](note/04.md#03a-compiler)</li></ul> | 2022/03/09 |
|   [week05](note/05.md)   | [C6](note/05.md#C6)<br /><ul><li>[自我編譯](note/05.md#自我編譯)</li><li>[asm](note/05.md#asm)<ul><li>sum</li><li>fib</li></ul></li><li>[遞迴下降](note/05.md#遞迴下降)</li></ul> | 2022/03/16 |
|   [week06](note/06.md)   | [gcc](note/06.md#gcc)<br />[x86組合語言](note/06.md#x86組合語言)<ul><li>hello.s</li><li>foobar.c</li><li>esp、ebp圖解</li></ul>[內嵌式組合語言](note/06.md#內嵌式組合語言)<ul><li>inline.c</li><li>jit.c快速編譯</li></ul> | 2022/03/21 |
|   [week07](note/07.md)   | [C函式庫](note/07.md#C函式庫)<br />[Makefile寫法](note/07.md#Makefile寫法) | 2022/03/30 |
|          week08          | 放假!                                                        | 2022/04/06 |
| **[week09](note/09.md)** | [作業系統](note/09.md#作業系統)<ul><li>[作業系統的五大功能模組](note/09.md#作業系統的五大功能模組)<ul><li>行程管理</li><li>記憶體管理</li><li>輸出入系統</li><li>檔案系統</li><li>使用者介面</li></ul></li><li>[行程](note/09.md#行程)<ul><li>[行程的資料共享方法](note/09.md#行程的資料共享方法)</li><li>[執行緒(Thread)](note/09.md#執行緒Thread)</li><li>[競爭狀況與臨界區間](note/09.md#競爭狀況與臨界區間)</li></ul></li><li>[多工](note/09.md#多工)</li><li>[排程](note/09.md#排程)<ul><li>[排程的方法](note/09.md#排程的方法)</li><li>[內文切換](note/09.md#內文切換)</li></ul></li><li>[記憶體管理](note/09.md#記憶體管理)<ul><li>[記憶體分配策略](note/09.md#記憶體分配策略)</li><li>[堆積空間不足時的處裡方法](note/09.md#堆積空間不足時的處裡方法)</li><li>[記憶體管理單元(MMU)](note/09.md#記憶體管理單元MMU)</li><li>[磁區結構](note/09.md#磁區結構)</li><li>[輸出入系統](note/09.md#輸出入系統)</li><li>[驅動系統](note/09.md#驅動系統)</li></ul></li></ul><li>[Linux](note/09.md#Linux)<ul><li>[Linux 行程管理](note/09.md#Linux-行程管理)</li><li>[Slab 記憶體配置器](note/09.md#Slab-記憶體配置器)</li><li>[Linux的驅動程式](note/09.md#Linux的驅動程式)</li></ul></li></ul>[Linux 指令](note/09.md#Linux-指令) | 2022/04/13 |
|   [week10](note/10.md)   | [函數式編成](note/10.md#函數式編成)<br />[pair](note/10.md#pair)<br />[map](note/10.md#map)<br />[lisp](note/10.md#lisp) | 2022/04/20 |
|   [week11](note/11.md)   | [race condition](note/11.md#race-condition)<ul><li>race.c</li><li>norace.c</li></ul>[生產者消費者問題](note/11.md#生產者消費者問題)<br />[哲學家用餐問題](note/11.md#哲學家用餐問題)<br />[fork](note/11.md#fork)<ul><li>[posix](note/11.md#posix)</li><li>[process](note/11.md#process)</li><li>[多工處裡](note/11.md#多工處裡)</li><li>[exec](note/11.md#exec)</li><li>[system](note/11.md#system)</li><li>[zombie](note/11.md#zombie)</li></ul> |            |
|   [week12](note/12.md)   | [FileSystem](note/12.md#FileSystem)<ul><li>[IO](note/12.md#IO)</li><li>[echo](note/12.md#echo)</li><li>[stderr](note/12.md#stderr)</li><li>[shell](note/12.md#shell)</li></ul>[Net](note/12.md#Net)<ul><li>[timer](note/12.md#timer)</li><li>[server](note/12.md#server)</li><li>[client](note/12.md#client)</li><li>[connetion](note/12.md#connetion)</li></ul> | 2022/05/04 |
|   [week13](note/13.md)   | [HTTPServer](note/13.md#HTTPServer)<ul><li>[Hello World!](note/13.md#Hello-World!)</li><li>[connect](note/13.md#connect)</li><li>[response](note/13.md#response)</li><li>[thread](note/13.md#thread)</li></ul>[提升程式效能](note/13.md#提升程式效能) | 2022/05/11 |
| **[week14](note/14.md)** | [Inter-Process Communication](note/14.md#掛載格式硬碟)<ul><li>[fifo](note/14.md#fifo)</li><li>[mmap](note/14.md#mmap)</li><li>[msgQ](note/14.md#msgQ)</li><li>[udp](note/14.md#udp)</li><li>[tcp](note/14.md#tcp)</li><li>[thread](note/14.md#thread)</li></ul>[LinuxNet](note/14.md#LinuxNet)<ul><li>[poll](note/14.md#poll)</li><li>[epoll](note/14.md#epoll)</li><li>[msgQ](note/14.md#msgQ)</li></ul>[特殊函數](note/14.md#特殊函數)<ul><li>strcmp</li><li>fgets</li><li>fopen</li><li>mmap</li><li>msgget</li><li>msgsnd</li><li>msgrcv</li></ul> | 2022/05/18 |
|   [week15](note/15.md)   | [mini-riscv-os](note/15.md#mini-riscv-os)<ul><li>[HelloOs](note/15.md#HelloOs)</li><li>[ContextSwitch](note/15.md#ContextSwitch)</li><li>[MultiTasking](note/15.md#MultiTasking)</li></ul> | 2022/05/25 |
|   [week16](note/16.md)   | [mini-riscv-os-續](note/16.md#mini-riscv-os-續)<ul><li>[TimerInterrupt](note/16.md#TimerInterrupt)</li><li>[Preemptive](note/16.md#Preemptive)</li></ul> | 2022/06/01 |
|   [week17](note/17.md)   | [xv6](note/17.md#xv6)<ul><li>[啟動程式](note/17.md#啟動程式)</li><li>[行程控制](note/17.md#行程控制)</li><li>[行程初始化](note/17.md#行程初始化)</li><li>[設定虛擬機](note/17.md#設定虛擬機)</li><li>[init](note/17.md#init)</li><li>[cat 指令](note/17.md#cat-指令)</li></ul> | 2022/06/08 |
|   [week18](note/18.md)   | [xv6-續](note/18.md#xv6-續)<ul><li>[cat.c](note/18.md#cat.c)</li><li>[open()](note/18.md#open)</li><li>[read()](note/18.md#read)</li><li>[inode,-superblock-與檔案系統](note/18.md#inode,-superblock-與檔案系統)</li><li>[緩衝模組-bio.c](note/18.md#緩衝模組-bio.c)</li><li>[虛擬硬碟存取-virtio.h/virtio-disk.c](note/18.md#虛擬硬碟存取-virtio.h/virtio-disk.c)</li><li>[file](note/18.md#file)</li><li>[整體的檔案系統結構](note/18.md#整體的檔案系統結構)</li><li>[xv6:-中斷-Interrupt-Trap](note/18.md#xv6:-中斷-Interrupt-Trap)<ul><li>[重要觀念](note/18.md#重要觀念)</li><li>[main.c](note/18.md#main.c)</li></ul></li><li>[xv6:-平行與鎖-lock----多核心的控管](note/18.md#xv6:-平行與鎖-lock----多核心的控管)<ul><li>[xv6:-磁碟驅動與-virtio-交互協議](note/18.md#xv6:-磁碟驅動與-virtio-交互協議)</li><li>[參考](note/18.md#參考)</li><li>[簡介](note/18.md#簡介)</li><li>[運作方式](note/18.md#運作方式)</li></ul></li><li>[xv6:-分頁表與-MMU](note/18.md#xv6:-分頁表與-MMU)<ul><li>[riscv.h](note/18.md#riscv.h)</li></ul></li><li>[Q](note/18.md#參考)</li></ul> | 2022/06/15 |

