# Homework 4

## Part 1:  Chapter 13 Code

**Q2**: How much memory is in your system? How much is free?

**A**: 7860 MB total, 244 MB free.

```zsh
$ free -m
              total        used        free      shared  buff/cache   available
Mem:           7860        5102         256           0        2502        2542
Swap:           999         859         140
```

---

**Q3**: Create a little program called `memory-user.c`.

**A**: Please see [memory-user.c](./memory-user.c) in this directory.

---

**Q4**: How do the memory usage totals change when your program is running? How about when you kill the `memory-user` program? Do th numbers match your expectations? Try this for different amounts of memory usage. What happens when you use really large amounts of memory?

**A**: The memory usage totals increase as the program runs, and decrease when the program is killed. The numbers don't always match my expectations. With increasing memory been using, the free memory becomes less at first. Then it stays at around 120M, and the buff/cache starts to decrease. When using really large amounts of memory, the program is killed by the kernel.

```zsh
$ free -m
              total        used        free      shared  buff/cache   available
Mem:           7860        5102         256           0        2502        2542
Swap:           999         859         140

$ ./memory-user -a 100
memory-user: pid is 13561

$ free -m
              total        used        free      shared  buff/cache   available
Mem:           7860        5205         155           0        2499        2439
Swap:           999         859         140

$ ./memory-user -a 300
memory-user: pid is 13760

$ free -m
              total        used        free      shared  buff/cache   available
Mem:           7860        5398         129           0        2332        2240
Swap:           999         860         139

$ ./memory-user -a 500
memory-user: pid is 13992

$ free -m
              total        used        free      shared  buff/cache   available
Mem:           7860        5590         127           0        2141        2039
Swap:           999         861         138

$ ./memory-user -a 800
memory-user: pid is 14167

$ free -m
              total        used        free      shared  buff/cache   available
Mem:           7860        5879         136           0        1844        1740
Swap:           999         863         136

$ ./memory-user -a 999999999
memory-user: pid is 14646
Failed to allocate memory.
```

---

**Q6**: First run `ps auxw` to see a list of all processes; then, pick an interesting one, such as a browser.

**A**:

```zsh
$ ps auxw
USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root         1  0.0  0.0 225652  7584 ?        Ss    2021   8:38 /lib/systemd/systemd --system --deserialize 41
root         2  0.0  0.0      0     0 ?        S     2021   0:02 [kthreadd]
root         3  0.0  0.0      0     0 ?        I<    2021   0:00 [rcu_gp]
root         4  0.0  0.0      0     0 ?        I<    2021   0:00 [rcu_par_gp]
root         6  0.0  0.0      0     0 ?        I<    2021   0:00 [kworker/0:0H-kb]
root         9  0.0  0.0      0     0 ?        I<    2021   0:00 [mm_percpu_wq]
root        10  0.0  0.0      0     0 ?        S     2021  36:10 [ksoftirqd/0]
root        11  0.1  0.0      0     0 ?        I     2021 1045:01 [rcu_sched]
root        12  0.0  0.0      0     0 ?        S     2021   4:06 [migration/0]
root        13  0.0  0.0      0     0 ?        S     2021   0:00 [cpuhp/0]
root        14  0.0  0.0      0     0 ?        S     2021   0:00 [cpuhp/1]
...
sa       16156  102  1.2 106916 103916 pts/1   R+   06:52   0:15 ./memory-user -a 100
```

---

**Q7**: Now run `pmap` on some of these processes, using various flags (like -X) to reveal many details about the process. What do you see? How many different entities make up a modern address space, as opposed to our simple conception of code/stack/heap?

**A**: With `-x` flag, I can see `Kbytes`, `RSS` and `Dirty` of each memory region. With `-X` flag, I can see more details of each memory region, including `KernelPageSize`, `MMUPageSize`, `Shared_Clean`, `Shared_Dirty`, `Private_Clean`, `Private_Dirty`, `Referenced`, `Anonymous`, `LazyFree`, `AnonHugePages`, `ShmemPmdMapped`, `FilePmdMapped`, `Shared_Hugetlb`, `Private_Hugetlb`, `Swap`, `SwapPss`, `Locked`, `THPeligible`, `ProtectionKey`, `Mapping`. With `-XX` flag, I can see even more details of each memory region, including `VmFlagsMapping`.

```zsh
$ pmap 16156
16156:   ./memory-user -a 100
00005578c3ce3000      4K r-x-- musr
00005578c3ee4000      4K r---- musr
00005578c3ee5000      4K rw--- musr
00005578c51ea000    132K rw---   [ anon ]
00007fa967a13000 102404K rw---   [ anon ]
00007fa96de14000   1948K r-x-- libc-2.27.so
00007fa96dffb000   2048K ----- libc-2.27.so
00007fa96e1fb000     16K r---- libc-2.27.so
00007fa96e1ff000      8K rw--- libc-2.27.so
00007fa96e201000     16K rw---   [ anon ]
00007fa96e205000    164K r-x-- ld-2.27.so
00007fa96e420000      8K rw---   [ anon ]
00007fa96e42e000      4K r---- ld-2.27.so
00007fa96e42f000      4K rw--- ld-2.27.so
00007fa96e430000      4K rw---   [ anon ]
00007ffe3585c000    132K rw---   [ stack ]
00007ffe359c5000     12K r----   [ anon ]
00007ffe359c8000      4K r-x--   [ anon ]
ffffffffff600000      4K --x--   [ anon ]
 total           106920K

$ pmap -x 16156
16156:   ./memory-user -a 100
Address           Kbytes     RSS   Dirty Mode  Mapping
00005578c3ce3000       4       4       0 r-x-- musr
00005578c3ce3000       0       0       0 r-x-- musr
00005578c3ee4000       4       4       4 r---- musr
00005578c3ee4000       0       0       0 r---- musr
00005578c3ee5000       4       4       4 rw--- musr
00005578c3ee5000       0       0       0 rw--- musr
00005578c51ea000     132       4       4 rw---   [ anon ]
00005578c51ea000       0       0       0 rw---   [ anon ]
00007fa967a13000  102404  102404  102404 rw---   [ anon ]
00007fa967a13000       0       0       0 rw---   [ anon ]
00007fa96de14000    1948    1308       0 r-x-- libc-2.27.so
00007fa96de14000       0       0       0 r-x-- libc-2.27.so
00007fa96dffb000    2048       0       0 ----- libc-2.27.so
00007fa96dffb000       0       0       0 ----- libc-2.27.so
00007fa96e1fb000      16      16      16 r---- libc-2.27.so
00007fa96e1fb000       0       0       0 r---- libc-2.27.so
00007fa96e1ff000       8       8       8 rw--- libc-2.27.so
00007fa96e1ff000       0       0       0 rw--- libc-2.27.so
00007fa96e201000      16      12      12 rw---   [ anon ]
00007fa96e201000       0       0       0 rw---   [ anon ]
00007fa96e205000     164     164       0 r-x-- ld-2.27.so
00007fa96e205000       0       0       0 r-x-- ld-2.27.so
00007fa96e420000       8       8       8 rw---   [ anon ]
00007fa96e420000       0       0       0 rw---   [ anon ]
00007fa96e42e000       4       4       4 r---- ld-2.27.so
00007fa96e42e000       0       0       0 r---- ld-2.27.so
00007fa96e42f000       4       4       4 rw--- ld-2.27.so
00007fa96e42f000       0       0       0 rw--- ld-2.27.so
00007fa96e430000       4       4       4 rw---   [ anon ]
00007fa96e430000       0       0       0 rw---   [ anon ]
00007ffe3585c000     132      12      12 rw---   [ stack ]
00007ffe3585c000       0       0       0 rw---   [ stack ]
00007ffe359c5000      12       0       0 r----   [ anon ]
00007ffe359c5000       0       0       0 r----   [ anon ]
00007ffe359c8000       4       4       0 r-x--   [ anon ]
00007ffe359c8000       0       0       0 r-x--   [ anon ]
ffffffffff600000       4       0       0 --x--   [ anon ]
ffffffffff600000       0       0       0 --x--   [ anon ]
---------------- ------- ------- -------
total kB          106920  103964  102484

$ pmap -X 16156
16156:   ./memory-user -a 100
         Address Perm   Offset Device  Inode   Size    Rss    Pss Referenced Anonymous LazyFree ShmemPmdMapped FilePmdMapped Shared_Hugetlb Private_Hugetlb Swap SwapPss Locked THPeligible ProtectionKey Mapping
    5578c3ce3000 r-xp 00000000 103:01 513825      4      4      4          4         0        0              0             0              0               0    0       0      0           0             0 musr
    5578c3ee4000 r--p 00001000 103:01 513825      4      4      4          4         4        0              0             0              0               0    0       0      0           0             0 musr
    5578c3ee5000 rw-p 00002000 103:01 513825      4      4      4          4         4        0              0             0              0               0    0       0      0           0             0 musr
    5578c51ea000 rw-p 00000000  00:00      0    132      4      4          4         4        0              0             0              0               0    0       0      0           0             0 [heap]
    7fa967a13000 rw-p 00000000  00:00      0 102404 102404 102404     102404    102404        0              0             0              0               0    0       0      0           0             0
    7fa96de14000 r-xp 00000000 103:01  10783   1948   1308     55       1308         0        0              0             0              0               0    0       0      0           0             0 libc-2.27.so
    7fa96dffb000 ---p 001e7000 103:01  10783   2048      0      0          0         0        0              0             0              0               0    0       0      0           0             0 libc-2.27.so
    7fa96e1fb000 r--p 001e7000 103:01  10783     16     16     16         16        16        0              0             0              0               0    0       0      0           0             0 libc-2.27.so
    7fa96e1ff000 rw-p 001eb000 103:01  10783      8      8      8          8         8        0              0             0              0               0    0       0      0           0             0 libc-2.27.so
    7fa96e201000 rw-p 00000000  00:00      0     16     12     12         12        12        0              0             0              0               0    0       0      0           0             0
    7fa96e205000 r-xp 00000000 103:01  10779    164    164      6        164         0        0              0             0              0               0    0       0      0           0             0 ld-2.27.so
    7fa96e420000 rw-p 00000000  00:00      0      8      8      8          8         8        0              0             0              0               0    0       0      0           0             0
    7fa96e42e000 r--p 00029000 103:01  10779      4      4      4          4         4        0              0             0              0               0    0       0      0           0             0 ld-2.27.so
    7fa96e42f000 rw-p 0002a000 103:01  10779      4      4      4          4         4        0              0             0              0               0    0       0      0           0             0 ld-2.27.so
    7fa96e430000 rw-p 00000000  00:00      0      4      4      4          4         4        0              0             0              0               0    0       0      0           0             0
    7ffe3585c000 rw-p 00000000  00:00      0    132     12     12         12        12        0              0             0              0               0    0       0      0           0             0 [stack]
    7ffe359c5000 r--p 00000000  00:00      0     12      0      0          0         0        0              0             0              0               0    0       0      0           0             0 [vvar]
    7ffe359c8000 r-xp 00000000  00:00      0      4      4      0          4         0        0              0             0              0               0    0       0      0           0             0 [vdso]
ffffffffff600000 --xp 00000000  00:00      0      4      0      0          0         0        0              0             0              0               0    0       0      0           0             0 [vsyscall]
                                             ====== ====== ====== ========== ========= ======== ============== ============= ============== =============== ==== ======= ====== =========== =============
                                             106920 103964 102549     103964    102484        0              0             0              0               0    0       0      0           0             0 KB

$ pmap -XX 16156
16156:   ./memory-user -a 100
         Address Perm   Offset Device  Inode   Size KernelPageSize MMUPageSize    Rss    Pss Shared_Clean Shared_Dirty Private_Clean Private_Dirty Referenced Anonymous LazyFree AnonHugePages ShmemPmdMapped FilePmdMapped Shared_Hugetlb Private_Hugetlb Swap SwapPss Locked THPeligible ProtectionKey                   VmFlagsMapping
    5578c3ce3000 r-xp 00000000 103:01 513825      4              4           4      4      4            0            0             4             0          4         0        0             0              0             0              0               0    0       0      0           0             0    rd ex mr mw me dw sd  musr
    5578c3ee4000 r--p 00001000 103:01 513825      4              4           4      4      4            0            0             0             4          4         4        0             0              0             0              0               0    0       0      0           0             0    rd mr mw me dw ac sd  musr
    5578c3ee5000 rw-p 00002000 103:01 513825      4              4           4      4      4            0            0             0             4          4         4        0             0              0             0              0               0    0       0      0           0             0 rd wr mr mw me dw ac sd  musr
    5578c51ea000 rw-p 00000000  00:00      0    132              4           4      4      4            0            0             0             4          4         4        0             0              0             0              0               0    0       0      0           0             0    rd wr mr mw me ac sd  [heap]
    7fa967a13000 rw-p 00000000  00:00      0 102404              4           4 102404 102404            0            0             0        102404     102404    102404        0             0              0             0              0               0    0       0      0           0             0    rd wr mr mw me ac sd
    7fa96de14000 r-xp 00000000 103:01  10783   1948              4           4   1308     54         1308            0             0             0       1308         0        0             0              0             0              0               0    0       0      0           0             0       rd ex mr mw me sd  libc-2.27.so
    7fa96dffb000 ---p 001e7000 103:01  10783   2048              4           4      0      0            0            0             0             0          0         0        0             0              0             0              0               0    0       0      0           0             0             mr mw me sd  libc-2.27.so
    7fa96e1fb000 r--p 001e7000 103:01  10783     16              4           4     16     16            0            0             0            16         16        16        0             0              0             0              0               0    0       0      0           0             0       rd mr mw me ac sd  libc-2.27.so
    7fa96e1ff000 rw-p 001eb000 103:01  10783      8              4           4      8      8            0            0             0             8          8         8        0             0              0             0              0               0    0       0      0           0             0    rd wr mr mw me ac sd  libc-2.27.so
    7fa96e201000 rw-p 00000000  00:00      0     16              4           4     12     12            0            0             0            12         12        12        0             0              0             0              0               0    0       0      0           0             0    rd wr mr mw me ac sd
    7fa96e205000 r-xp 00000000 103:01  10779    164              4           4    164      6          164            0             0             0        164         0        0             0              0             0              0               0    0       0      0           0             0    rd ex mr mw me dw sd  ld-2.27.so
    7fa96e420000 rw-p 00000000  00:00      0      8              4           4      8      8            0            0             0             8          8         8        0             0              0             0              0               0    0       0      0           0             0    rd wr mr mw me ac sd
    7fa96e42e000 r--p 00029000 103:01  10779      4              4           4      4      4            0            0             0             4          4         4        0             0              0             0              0               0    0       0      0           0             0    rd mr mw me dw ac sd  ld-2.27.so
    7fa96e42f000 rw-p 0002a000 103:01  10779      4              4           4      4      4            0            0             0             4          4         4        0             0              0             0              0               0    0       0      0           0             0 rd wr mr mw me dw ac sd  ld-2.27.so
    7fa96e430000 rw-p 00000000  00:00      0      4              4           4      4      4            0            0             0             4          4         4        0             0              0             0              0               0    0       0      0           0             0    rd wr mr mw me ac sd
    7ffe3585c000 rw-p 00000000  00:00      0    132              4           4     12     12            0            0             0            12         12        12        0             0              0             0              0               0    0       0      0           0             0    rd wr mr mw me gd ac  [stack]
    7ffe359c5000 r--p 00000000  00:00      0     12              4           4      0      0            0            0             0             0          0         0        0             0              0             0              0               0    0       0      0           0             0    rd mr pf io de dd sd  [vvar]
    7ffe359c8000 r-xp 00000000  00:00      0      4              4           4      4      0            4            0             0             0          4         0        0             0              0             0              0               0    0       0      0           0             0    rd ex mr mw me de sd  [vdso]
ffffffffff600000 --xp 00000000  00:00      0      4              4           4      0      0            0            0             0             0          0         0        0             0              0             0              0               0    0       0      0           0             0                      ex  [vsyscall]
                                             ====== ============== =========== ====== ====== ============ ============ ============= ============= ========== ========= ======== ============= ============== ============= ============== =============== ==== ======= ====== =========== =============
                                             106920             76          76 103964 102548         1476            0             4        102484     103964    102484        0             0              0             0              0               0    0       0      0           0             0 KB
```

---

**Q8**: Finally, let's run `pmap` on your `memory-user` program, with different amounts of memory. What do you see? Does the output from `pmap` match your expectations?

**A**: With increasing of memory been using, the `pmap` output shows that the `anon` memory increases. The output matches my expectations.

```zsh
$ pmap 16156
16156:   ./memory-user -a 100
00005578c3ce3000      4K r-x-- musr
00005578c3ee4000      4K r---- musr
00005578c3ee5000      4K rw--- musr
00005578c51ea000    132K rw---   [ anon ]
00007fa967a13000 102404K rw---   [ anon ]
00007fa96de14000   1948K r-x-- libc-2.27.so
00007fa96dffb000   2048K ----- libc-2.27.so
00007fa96e1fb000     16K r---- libc-2.27.so
00007fa96e1ff000      8K rw--- libc-2.27.so
00007fa96e201000     16K rw---   [ anon ]
00007fa96e205000    164K r-x-- ld-2.27.so
00007fa96e420000      8K rw---   [ anon ]
00007fa96e42e000      4K r---- ld-2.27.so
00007fa96e42f000      4K rw--- ld-2.27.so
00007fa96e430000      4K rw---   [ anon ]
00007ffe3585c000    132K rw---   [ stack ]
00007ffe359c5000     12K r----   [ anon ]
00007ffe359c8000      4K r-x--   [ anon ]
ffffffffff600000      4K --x--   [ anon ]
 total           106920K

$ pmap 16783
16783:   ./memory-user -a 200
000055ee765b0000      4K r-x-- musr
000055ee767b1000      4K r---- musr
000055ee767b2000      4K rw--- musr
000055ee777a7000    132K rw---   [ anon ]
00007fdd91380000 204804K rw---   [ anon ]
00007fdd9db81000   1948K r-x-- libc-2.27.so
00007fdd9dd68000   2048K ----- libc-2.27.so
00007fdd9df68000     16K r---- libc-2.27.so
00007fdd9df6c000      8K rw--- libc-2.27.so
00007fdd9df6e000     16K rw---   [ anon ]
00007fdd9df72000    164K r-x-- ld-2.27.so
00007fdd9e18d000      8K rw---   [ anon ]
00007fdd9e19b000      4K r---- ld-2.27.so
00007fdd9e19c000      4K rw--- ld-2.27.so
00007fdd9e19d000      4K rw---   [ anon ]
00007ffedc796000    132K rw---   [ stack ]
00007ffedc7f3000     12K r----   [ anon ]
00007ffedc7f6000      4K r-x--   [ anon ]
ffffffffff600000      4K --x--   [ anon ]
 total           209320K

$ pmap 16870
16870:   ./memory-user -a 500
0000555e545fd000      4K r-x-- musr
0000555e547fe000      4K r---- musr
0000555e547ff000      4K rw--- musr
0000555e560c5000    132K rw---   [ anon ]
00007f7c53cf2000 512004K rw---   [ anon ]
00007f7c730f3000   1948K r-x-- libc-2.27.so
00007f7c732da000   2048K ----- libc-2.27.so
00007f7c734da000     16K r---- libc-2.27.so
00007f7c734de000      8K rw--- libc-2.27.so
00007f7c734e0000     16K rw---   [ anon ]
00007f7c734e4000    164K r-x-- ld-2.27.so
00007f7c736ff000      8K rw---   [ anon ]
00007f7c7370d000      4K r---- ld-2.27.so
00007f7c7370e000      4K rw--- ld-2.27.so
00007f7c7370f000      4K rw---   [ anon ]
00007ffc95300000    132K rw---   [ stack ]
00007ffc9537e000     12K r----   [ anon ]
00007ffc95381000      4K r-x--   [ anon ]
ffffffffff600000      4K --x--   [ anon ]
 total           516520K
```
