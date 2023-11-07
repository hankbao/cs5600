# Homework 7

## Part 1: Chapter 20 Simulation

**Q1**: With a linear page table, you need a single register to locate the page table, assuming that hardware does the lookup upon a TLB miss. How many registers do you need to locate a two-level page table? A three-level table?

**A**: Only the PDE looking-up needs a register. Because the PFN we obtained from PDE is the physical address of the PTE, we can use the PFN to locate the PTE. So we don't need a register to locate the PTE.

---

**Q2**: Use the simulator to perform translations given random seeds 0, 1, and 2, and check your answers using the `-c` flag. How many memory references are needed to perform each lookup?

**A**: Two memory references are needed to perform each lookup excluding the final memory access.

---

**Q3**: Given your understanding of how cache memory works, how do you think memory references to the page table will behave in the cache? Will they lead to lots of cache hits (and thus fast accesses?) Or lots of misses (and thus slow accesses)?

**A**: It depends. There are many factors need to taking into account. Like cache size and its replacement policy. Also considering the locality of the page table. If the page table is small enough, it will be stored in the cache. If the page table is large, it will be stored in the main memory. If the page table is stored in the cache, it will lead to lots of cache hits. If the page table is stored in the main memory, it will lead to lots of cache misses. The memory accessing pattern should also be considered.

## Part 2: Chapter 21 Measurement

**Q1**: First, open two separate terminal connections to the _same_ machine, so that you can easily run something in one window and the other.  Now, in one window, run `vmstat 1`, which shows statistics about machine usage every second. Read the man page, the associated README, and any other information you need so that you can understand its output. Leave this window running `vmstat` for the rest of the exercises below.

Now, we will run the program `mem.c` but with very little memory usage. This can be accomplished by typing `./mem 1` (which uses only 1 MB of memory). How do the CPU usage statistics change when running `mem`? Do the numbers in the user time column make sense? How does this change when running more than one instance of `mem` at once?

**A**: The user time increased to around 50%.

```zsh
procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
 2  0 801780 163900 254884 2282788    0    0     2    24    0    0 26  1 72  0  1
 5  0 801780 163900 254884 2282788    0    0     0    72 1288 2095  2  1 96  0  0
 1  0 801780 163388 254884 2282788    0    0     0   664 1453 2283 32  1 64  1  3
 1  0 801780 162884 254884 2282788    0    0     0   192 1505 2478 53  1 45  1  2
 1  0 801780 162884 254884 2282788    0    0     0   152 1339 2120 51  1 47  1  1
 1  0 801780 162884 254884 2282788    0    0     0   168 1289 2081 52  1 46  0  1
 1  0 801780 162884 254884 2282792    0    0     0   152 1286 2067 49  1 47  0  3
 ```

 When running with `./mem 1000`, swapping happened. The system time increased during swapping out.

 ```zsh
 procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
 5  0 801780 160372 254884 2283296    0    0     2    24    0    0 26  1 72  0  1
 1  0 801780 160112 254884 2283296    0    0     0   176 1391 2283 51  1 47  0  2
 1  0 801780 160112 254884 2283296    0    0     0   152 1439 2266 50  1 47  1  2
 0  0 801780 161040 254884 2283404    0    0   108   228 1581 2721 45  2 44  1  8
 0  0 801780 161380 254884 2283424    0    0     0   676 1328 2132  1  1 97  1  2
 1  0 801780 156080 254884 2283428    0    0     0   228 1629 2610  1  2 95  0  1
 4  0 805816 123744 212548 1883784    0 4036   844  4148 2526 3131 44 33 14  1  8
 2  0 806408 120176 201500 1437208    0  592   315   636 2101 4002 35 53  7  0  4
 2  0 806572 206404 196332 1310880    0  164     0   404 1625 3075 51 11 33  1  5
 1  0 806572 206404 196340 1310936    0    0    48   144 1229 1955 41  1 45  0 14
 ```

---

**Q2**: Let’s now start looking at some of the memory statistics while running `mem`. We’ll focus on two columns: `swpd` (the amount of virtual memory used) and `free` (the amount of idle memory). Run `./mem 1024` (which allocates 1024 MB) and watch how these values change. Then kill the running program (by typing control-c) and watch again how the values change. What do you notice about the values? In particular, how does the `free` column change when the program exits? Does the amount of free memory increase by the expected amount when `mem` exits?

**A**: When running `./mem 1024`, the value of free memory decreased. After the program exits, the program will release the memory it used. So the value of free memory will increase by the expected amount.

```zsh
procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
 1  0 806572 1228708 196460 1314116    0    0     2    24    0    0 26  1 72  0  1
 0  0 806572 1228692 196460 1314228    0    0     0    20 1212 1960  1  1 97  0  1
 0  0 806572 1228440 196460 1314228    0    0     0    80 1221 1995  0  2 98  0  1
 0  0 806572 1228664 196464 1314228    0    0     0    84 1425 2420  2  1 96  0  2
 0  0 806572 1228432 196464 1314228    0    0     0    72 1167 1947  1  1 98  0  1
 0  0 806572 1228440 196464 1314228    0    0     0   140 1245 2041  1  0 96  0  2
 0  0 806572 1228448 196464 1314228    0    0     0   148 1555 2557  1  2 93  0  3
 0  0 806572 1228664 196464 1314228    0    0     0    20 1207 1963  2  1 97  0  1
 0  0 806572 1228692 196468 1314244   12    0    12    80 1497 2457 10  1 87  0  1
 0  0 806572 1228660 196476 1314252    0    0     4   148 1322 2063  4  1 94  0  2
 0  0 806572 1228156 196476 1314252    0    0     0   104 1293 2058  1  1 97  0  2
 0  0 806572 1228156 196476 1314252    0    0     0    72 1390 2290  0  2 96  0  2
 0  0 806572 1227904 196476 1314248    0    0     0    72 1276 2027  1  1 98  0  1
 0  0 806572 1227912 196476 1314248    0    0     0    96 1274 2049  2  1 95  0  2
 0  0 806572 1227904 196476 1314248    0    0     0    72 1220 2009  1  1 97  0  1
 0  0 806572 1227904 196476 1314248    0    0     0    72 1256 2021  1  0 98  0  1
 1  0 806572 1205468 196476 1314248    0    0     0   164 1472 2374  4  2 93  0  1
 1  0 806572 569660 196480 1314248    0    0     0    20 1276 1901 25 24 47  0  4
 3  0 806572 179696 196480 1314248    0    0     0   148 1317 2048 37 13 47  1  3
 1  0 806572 179428 196484 1314248    0    0     0   112 1278 1981 49  1 47  0  3
 1  0 806572 178924 196484 1314248    0    0     0    88 1273 1996 45  1 46  0  9
 1  0 806572 178672 196484 1314252    0    0     0   120 1293 1910 48  1 47  1  4
 1  0 806572 178680 196484 1314252    0    0     0    84 1201 1845 49  1 48  0  2
 3  0 806572 178276 196484 1314256    0    0     0   140 1309 2336 51  1 42  0  6
 1  0 806572 178776 196484 1314256    0    0     0   136 1447 2770 68  1 31  0  1
 1  0 806572 178480 196484 1314328    0    0     0   180 1325 2524 51  1 40  0  7
 1  0 806572 178560 196484 1314324    0    0     0   104 1308 2212 47  0 46  0  7
 1  0 806572 178560 196488 1314324    0    0     0   108 1294 1975 50  1 47  0  2
 2  0 806572 178560 196492 1314324    0    0     0   160 1364 2275 49  1 46  1  3
 1  0 806572 178560 196492 1314324    0    0     0    88 1147 1822 49  0 47  0  5
 2  0 806572 178560 196496 1314320    0    0     0   108 1222 2449 48  1 45  0  5
 2  0 806572 178560 196496 1314332    0    0     0    88 1243 2064 48  1 47  1  5
 0  0 806572 1227544 196500 1314328    0    0     0   104 1284 2174  8  3 86  0  3
 2  0 806572 1227384 196500 1314332    0    0     0   188 1246 2011  1  0 96  0  2
 0  0 806572 1227392 196500 1314332    0    0     0    76 1213 1978  1  1 98  0  1
 1  0 806572 1227392 196500 1314332    0    0     0    76 1193 1952  1  1 96  0  1
 0  0 806572 1227392 196500 1314332    0    0     0    72 1352 2232  2  1 97  0  1
```

---

**Q3**: We’ll next look at the `swap` columns (`si` and `so`), which indicate how much swapping is taking place to and from the disk. Of course, to activate these, you’ll need to run mem with large amounts of memory. First, examine how much free memory is on your Linux system (for example, by typing `cat /proc/meminfo`; type `man proc` for details on the `/proc` file system and the types of information you can find there). One of the first entries in `/proc/meminfo` is the total amount of memory in your system. Let’s assume it’s something like 8 GB of memory; if so, start by running `mem 4000` (about 4 GB) and watching the swap in/out columns. Do they ever give non-zero values? Then, try with `5000`, `6000`, etc. What happens to these values as the program enters the second loop (and beyond), as compared to the first loop? How much data (total) are swapped in and out during the second, third, and subsequent loops? (do the numbers make sense?)

**A**: It fails to run with `mem 4000`. The program got killed. But I still observed swap-in and swap-out in the `vmstat` output.

```zsh
$ cat /proc/meminfo
MemTotal:        8049116 kB
MemFree:         1225888 kB
MemAvailable:    2477276 kB
Buffers:          196544 kB
Cached:           759464 kB
SwapCached:        64276 kB
Active:          3995732 kB
Inactive:        2049100 kB
Active(anon):    3482100 kB
Inactive(anon):  1561136 kB
Active(file):     513632 kB
Inactive(file):   487964 kB
Unevictable:           0 kB
Mlocked:               0 kB
SwapTotal:       1023996 kB
SwapFree:         217424 kB
Dirty:               128 kB
Writeback:             0 kB
AnonPages:       5028400 kB
Mapped:           279460 kB
Shmem:               936 kB
KReclaimable:     555316 kB
Slab:             685440 kB
SReclaimable:     555316 kB
SUnreclaim:       130124 kB
KernelStack:        9920 kB
PageTables:        33576 kB
NFS_Unstable:          0 kB
Bounce:                0 kB
WritebackTmp:          0 kB
CommitLimit:     5048552 kB
Committed_AS:   10121584 kB
VmallocTotal:   34359738367 kB
VmallocUsed:       20800 kB
VmallocChunk:          0 kB
Percpu:             3144 kB
HardwareCorrupted:     0 kB
AnonHugePages:         0 kB
ShmemHugePages:        0 kB
ShmemPmdMapped:        0 kB
FileHugePages:         0 kB
FilePmdMapped:         0 kB
CmaTotal:              0 kB
CmaFree:               0 kB
HugePages_Total:       0
HugePages_Free:        0
HugePages_Rsvd:        0
HugePages_Surp:        0
Hugepagesize:       2048 kB
Hugetlb:               0 kB
DirectMap4k:     2514920 kB
DirectMap2M:     5769216 kB
DirectMap1G:           0 kB
```

```zsh
$ ./mem 4000
allocating 4194304000 bytes (4000.00 MB)
  number of integers in array: 1048576000
Killed
```

```zsh
procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
 3  1 806572 1225132 196572 1315004    0    0     2    24    0    0 26  1 72  0  1
 0  0 806572 1225124 196572 1315004    0    0     0    40 1224 1977  0  1 98  0  2
 3  0 806836 120532 192392 1217744    0  264    12   332 1359 2171 29 29 41  0  2
 1  1 810608 124120 118764 709072    0 3772     0  3852 2184 2445 28 55 12  2  4
 1  0 830500 129356  35464 290728    0 20404  1364 21080 6781 4804 26 47  2 19  6
 0  5 845168 102636   4388 111204    0 14668  7619 14892 5594 5543 13 26  8 45  8
 1  6 856456 102536   4580 112800    0 11312 12368 11392 4915 5903  1  8  2 84  4
 0  3 867976 102224   3460 106960    0 11756  1972 11812 4898 6023  2  6  6 81  4
 1  5 879308 102476   3144 111024    0 11328  9826 11400 5294 7025  1  8  1 87  5
 0 11 891216 102456   3140 113764    0 11908  3184 11980 4876 6194  1  2  0 92  3
 0 10 902632 103664   1500 119224    0 11416 11308 11472 4606 5842  0  4  0 91  5
 0 10 914624 103528   1500 121072    0 11992  1988 11996 4757 6316  1  3  0 94  2
 0  8 926344 103708   1504 126104    0 11720  5508 11768 4906 5886  1  2  0 96  1
 0 13 938292 101844   1512 129140    0 11948  3348 11984 5035 6615  2  4  0 93  2
 0 29 950328 103936   1492 129308    0 12036   612 12036 5320 7490  0  3  0 92  4
 0 16 961868 103904   2104 131132    0 11540  3720 11708 10208 16554  1  3  0 89  7
 2 11 973240 109088   2080 133120    0 11628  8096 11692 6054 8387  3  4  0 91  3
 0  5 984344 102496   2204 142116    0 11360  9472 11432 5132 6971  1  5  1 89  4
 1  5 995952 103632   1916 143048    0 11608  5460 11680 5084 6642  1  3  1 89  5
 0  7 1007720 102612   1936 145600    0 11768  3944 11840 4873 6373  1  4  2 89  3
 0  7 1019236 103688   2060 149824    0 11772  6164 11840 4998 6519  1  3  1 92  4
11  2 1023900 2769348   1536 119080  100 4864 140983  4984 5279 7546  5 50  0 42  3
 0  0 1023900 2722204   3836 177100    0    0 57331   120 2417 4044  1  6 80 10  2
```

---

**Q4**: Do the same experiments as above, but now watch the other statistics (such as CPU utilization, and block I/O statistics). How do they change when `mem` is running?

**A**: Block Output increases with Swap-out, and block input increases with Swap-in. The system time increases with Swap-out also.

---

**Q5**: Now let’s examine performance. Pick an input for `mem` that comfortably fits in memory (say `4000` if the amount of memory on the system is 8 GB). How long does loop 0 take (and subsequent loops 1, 2, etc.)? Now pick a size comfortably beyond the size of memory (say `12000` again assuming 8 GB of memory). How long do the loops take here? How do the bandwidth numbers compare? How different is performance when constantly swapping versus fitting everything comfortably in memory? Can you make a graph, with the size of memory used by `mem` on the x-axis, and the bandwidth of accessing said memory on the y-axis? Finally, how does the performance of the first loop compare to that of subsequent loops, for both the case where everything fits in memory and where it doesn’t?

**A**: I cannot allocate more than 2048MB memory. But in general, the loop 0 takes about twice than the loops after. The bandwidth for loop 0 is about 1/3 of the loops after. The performance is much better when everything fits in memory.

```zsh
$ ./mem 1024
allocating 1073741824 bytes (1024.00 MB)
  number of integers in array: 268435456
loop 0 in 1620.05 ms (bandwidth: 632.08 MB/s)
loop 1 in 621.11 ms (bandwidth: 1648.65 MB/s)
loop 2 in 622.76 ms (bandwidth: 1644.28 MB/s)
loop 3 in 679.27 ms (bandwidth: 1507.51 MB/s)
loop 4 in 713.97 ms (bandwidth: 1434.22 MB/s)
```

```zsh
$ ./mem 2048
allocating 2147483648 bytes (2048.00 MB)
  number of integers in array: 536870912
loop 0 in 3268.47 ms (bandwidth: 626.59 MB/s)
loop 1 in 1249.87 ms (bandwidth: 1638.56 MB/s)
loop 2 in 1227.26 ms (bandwidth: 1668.76 MB/s)
loop 3 in 1265.84 ms (bandwidth: 1617.90 MB/s)
```

```zsh
$ ./mem 3072
allocating 3221225472 bytes (3072.00 MB)
  number of integers in array: 805306368
Killed
```

---

**Q6**: Swap space isn’t infinite. You can use the tool `swapon` with the `-s` flag to see how much swap space is available. What happens if you try to run mem with increasingly large values, beyond what seems to be available in swap? At what point does the memory allocation fail?

**A**: Below are the two examples before and after I run `./mem 2048`.

```zsh
$ swapon -s
Filename				Type		Size	Used	Priority
/swapfile               file    	1023996	1019396	-2
```

```zsh
$ swapon -s
Filename				Type		Size	Used	Priority
/swapfile               file    	1023996	1015080	-2
```

## Part 3: Chapter 22 Simulation

**Q1**: Generate random addresses with the following arguments: `-s 0 -n 10`, `-s 1 -n 10`, and `-s 2 -n 10`. Change the policy from FIFO, to LRU, to OPT. Compute whether each access in said address traces are hits or misses

**A**:

```zsh
$ python paging-policy.py -s 0 -n 10 -p FIFO
ARG addresses -1
ARG addressfile
ARG numaddrs 10
ARG policy FIFO
ARG clockbits 2
ARG cachesize 3
ARG maxpage 10
ARG seed 0
ARG notrace False

Solving...

Access: 8  MISS FirstIn ->          [8] <- Lastin  Replaced:- [Hits:0 Misses:1]
Access: 7  MISS FirstIn ->       [8, 7] <- Lastin  Replaced:- [Hits:0 Misses:2]
Access: 4  MISS FirstIn ->    [8, 7, 4] <- Lastin  Replaced:- [Hits:0 Misses:3]
Access: 2  MISS FirstIn ->    [7, 4, 2] <- Lastin  Replaced:8 [Hits:0 Misses:4]
Access: 5  MISS FirstIn ->    [4, 2, 5] <- Lastin  Replaced:7 [Hits:0 Misses:5]
Access: 4  HIT  FirstIn ->    [4, 2, 5] <- Lastin  Replaced:- [Hits:1 Misses:5]
Access: 7  MISS FirstIn ->    [2, 5, 7] <- Lastin  Replaced:4 [Hits:1 Misses:6]
Access: 3  MISS FirstIn ->    [5, 7, 3] <- Lastin  Replaced:2 [Hits:1 Misses:7]
Access: 4  MISS FirstIn ->    [7, 3, 4] <- Lastin  Replaced:5 [Hits:1 Misses:8]
Access: 5  MISS FirstIn ->    [3, 4, 5] <- Lastin  Replaced:7 [Hits:1 Misses:9]

FINALSTATS hits 1   misses 9   hitrate 10.00

$ python paging-policy.py -s 0 -n 10 -p LRU
ARG addresses -1
ARG addressfile
ARG numaddrs 10
ARG policy LRU
ARG clockbits 2
ARG cachesize 3
ARG maxpage 10
ARG seed 0
ARG notrace False

Solving...

Access: 8  MISS LRU ->          [8] <- MRU Replaced:- [Hits:0 Misses:1]
Access: 7  MISS LRU ->       [8, 7] <- MRU Replaced:- [Hits:0 Misses:2]
Access: 4  MISS LRU ->    [8, 7, 4] <- MRU Replaced:- [Hits:0 Misses:3]
Access: 2  MISS LRU ->    [7, 4, 2] <- MRU Replaced:8 [Hits:0 Misses:4]
Access: 5  MISS LRU ->    [4, 2, 5] <- MRU Replaced:7 [Hits:0 Misses:5]
Access: 4  HIT  LRU ->    [2, 5, 4] <- MRU Replaced:- [Hits:1 Misses:5]
Access: 7  MISS LRU ->    [5, 4, 7] <- MRU Replaced:2 [Hits:1 Misses:6]
Access: 3  MISS LRU ->    [4, 7, 3] <- MRU Replaced:5 [Hits:1 Misses:7]
Access: 4  HIT  LRU ->    [7, 3, 4] <- MRU Replaced:- [Hits:2 Misses:7]
Access: 5  MISS LRU ->    [3, 4, 5] <- MRU Replaced:7 [Hits:2 Misses:8]

FINALSTATS hits 2   misses 8   hitrate 20.00

$ python paging-policy.py -s 0 -n 10 -p OPT
ARG addresses -1
ARG addressfile
ARG numaddrs 10
ARG policy OPT
ARG clockbits 2
ARG cachesize 3
ARG maxpage 10
ARG seed 0
ARG notrace False

Solving...

Access: 8  MISS Left  ->          [8] <- Right Replaced:- [Hits:0 Misses:1]
Access: 7  MISS Left  ->       [8, 7] <- Right Replaced:- [Hits:0 Misses:2]
Access: 4  MISS Left  ->    [8, 7, 4] <- Right Replaced:- [Hits:0 Misses:3]
Access: 2  MISS Left  ->    [7, 4, 2] <- Right Replaced:8 [Hits:0 Misses:4]
Access: 5  MISS Left  ->    [7, 4, 5] <- Right Replaced:2 [Hits:0 Misses:5]
Access: 4  HIT  Left  ->    [7, 4, 5] <- Right Replaced:- [Hits:1 Misses:5]
Access: 7  HIT  Left  ->    [7, 4, 5] <- Right Replaced:- [Hits:2 Misses:5]
Access: 3  MISS Left  ->    [4, 5, 3] <- Right Replaced:7 [Hits:2 Misses:6]
Access: 4  HIT  Left  ->    [4, 5, 3] <- Right Replaced:- [Hits:3 Misses:6]
Access: 5  HIT  Left  ->    [4, 5, 3] <- Right Replaced:- [Hits:4 Misses:6]

FINALSTATS hits 4   misses 6   hitrate 40.00
```

```zsh
$ python paging-policy.py -s 1 -n 10 -p FIFO
ARG addresses -1
ARG addressfile
ARG numaddrs 10
ARG policy FIFO
ARG clockbits 2
ARG cachesize 3
ARG maxpage 10
ARG seed 1
ARG notrace False

Solving...

Access: 1  MISS FirstIn ->          [1] <- Lastin  Replaced:- [Hits:0 Misses:1]
Access: 8  MISS FirstIn ->       [1, 8] <- Lastin  Replaced:- [Hits:0 Misses:2]
Access: 7  MISS FirstIn ->    [1, 8, 7] <- Lastin  Replaced:- [Hits:0 Misses:3]
Access: 2  MISS FirstIn ->    [8, 7, 2] <- Lastin  Replaced:1 [Hits:0 Misses:4]
Access: 4  MISS FirstIn ->    [7, 2, 4] <- Lastin  Replaced:8 [Hits:0 Misses:5]
Access: 4  HIT  FirstIn ->    [7, 2, 4] <- Lastin  Replaced:- [Hits:1 Misses:5]
Access: 6  MISS FirstIn ->    [2, 4, 6] <- Lastin  Replaced:7 [Hits:1 Misses:6]
Access: 7  MISS FirstIn ->    [4, 6, 7] <- Lastin  Replaced:2 [Hits:1 Misses:7]
Access: 0  MISS FirstIn ->    [6, 7, 0] <- Lastin  Replaced:4 [Hits:1 Misses:8]
Access: 0  HIT  FirstIn ->    [6, 7, 0] <- Lastin  Replaced:- [Hits:2 Misses:8]

FINALSTATS hits 2   misses 8   hitrate 20.00

$ python paging-policy.py -s 1 -n 10 -p LRU
ARG addresses -1
ARG addressfile
ARG numaddrs 10
ARG policy LRU
ARG clockbits 2
ARG cachesize 3
ARG maxpage 10
ARG seed 1
ARG notrace False

Solving...

Access: 1  MISS LRU ->          [1] <- MRU Replaced:- [Hits:0 Misses:1]
Access: 8  MISS LRU ->       [1, 8] <- MRU Replaced:- [Hits:0 Misses:2]
Access: 7  MISS LRU ->    [1, 8, 7] <- MRU Replaced:- [Hits:0 Misses:3]
Access: 2  MISS LRU ->    [8, 7, 2] <- MRU Replaced:1 [Hits:0 Misses:4]
Access: 4  MISS LRU ->    [7, 2, 4] <- MRU Replaced:8 [Hits:0 Misses:5]
Access: 4  HIT  LRU ->    [7, 2, 4] <- MRU Replaced:- [Hits:1 Misses:5]
Access: 6  MISS LRU ->    [2, 4, 6] <- MRU Replaced:7 [Hits:1 Misses:6]
Access: 7  MISS LRU ->    [4, 6, 7] <- MRU Replaced:2 [Hits:1 Misses:7]
Access: 0  MISS LRU ->    [6, 7, 0] <- MRU Replaced:4 [Hits:1 Misses:8]
Access: 0  HIT  LRU ->    [6, 7, 0] <- MRU Replaced:- [Hits:2 Misses:8]

FINALSTATS hits 2   misses 8   hitrate 20.00

$ python paging-policy.py -s 1 -n 10 -p OPT
ARG addresses -1
ARG addressfile
ARG numaddrs 10
ARG policy OPT
ARG clockbits 2
ARG cachesize 3
ARG maxpage 10
ARG seed 1
ARG notrace False

Solving...

Access: 1  MISS Left  ->          [1] <- Right Replaced:- [Hits:0 Misses:1]
Access: 8  MISS Left  ->       [1, 8] <- Right Replaced:- [Hits:0 Misses:2]
Access: 7  MISS Left  ->    [1, 8, 7] <- Right Replaced:- [Hits:0 Misses:3]
Access: 2  MISS Left  ->    [1, 7, 2] <- Right Replaced:8 [Hits:0 Misses:4]
Access: 4  MISS Left  ->    [1, 7, 4] <- Right Replaced:2 [Hits:0 Misses:5]
Access: 4  HIT  Left  ->    [1, 7, 4] <- Right Replaced:- [Hits:1 Misses:5]
Access: 6  MISS Left  ->    [1, 7, 6] <- Right Replaced:4 [Hits:1 Misses:6]
Access: 7  HIT  Left  ->    [1, 7, 6] <- Right Replaced:- [Hits:2 Misses:6]
Access: 0  MISS Left  ->    [1, 7, 0] <- Right Replaced:6 [Hits:2 Misses:7]
Access: 0  HIT  Left  ->    [1, 7, 0] <- Right Replaced:- [Hits:3 Misses:7]

FINALSTATS hits 3   misses 7   hitrate 30.00
```

```zsh
$ python paging-policy.py -s 2 -n 10 -p FIFO
ARG addresses -1
ARG addressfile
ARG numaddrs 10
ARG policy FIFO
ARG clockbits 2
ARG cachesize 3
ARG maxpage 10
ARG seed 2
ARG notrace False

Solving...

Access: 9  MISS FirstIn ->          [9] <- Lastin  Replaced:- [Hits:0 Misses:1]
Access: 9  HIT  FirstIn ->          [9] <- Lastin  Replaced:- [Hits:1 Misses:1]
Access: 0  MISS FirstIn ->       [9, 0] <- Lastin  Replaced:- [Hits:1 Misses:2]
Access: 0  HIT  FirstIn ->       [9, 0] <- Lastin  Replaced:- [Hits:2 Misses:2]
Access: 8  MISS FirstIn ->    [9, 0, 8] <- Lastin  Replaced:- [Hits:2 Misses:3]
Access: 7  MISS FirstIn ->    [0, 8, 7] <- Lastin  Replaced:9 [Hits:2 Misses:4]
Access: 6  MISS FirstIn ->    [8, 7, 6] <- Lastin  Replaced:0 [Hits:2 Misses:5]
Access: 3  MISS FirstIn ->    [7, 6, 3] <- Lastin  Replaced:8 [Hits:2 Misses:6]
Access: 6  HIT  FirstIn ->    [7, 6, 3] <- Lastin  Replaced:- [Hits:3 Misses:6]
Access: 6  HIT  FirstIn ->    [7, 6, 3] <- Lastin  Replaced:- [Hits:4 Misses:6]

FINALSTATS hits 4   misses 6   hitrate 40.00

$ python paging-policy.py -s 2 -n 10 -p LRU
ARG addresses -1
ARG addressfile
ARG numaddrs 10
ARG policy LRU
ARG clockbits 2
ARG cachesize 3
ARG maxpage 10
ARG seed 2
ARG notrace False

Solving...

Access: 9  MISS LRU ->          [9] <- MRU Replaced:- [Hits:0 Misses:1]
Access: 9  HIT  LRU ->          [9] <- MRU Replaced:- [Hits:1 Misses:1]
Access: 0  MISS LRU ->       [9, 0] <- MRU Replaced:- [Hits:1 Misses:2]
Access: 0  HIT  LRU ->       [9, 0] <- MRU Replaced:- [Hits:2 Misses:2]
Access: 8  MISS LRU ->    [9, 0, 8] <- MRU Replaced:- [Hits:2 Misses:3]
Access: 7  MISS LRU ->    [0, 8, 7] <- MRU Replaced:9 [Hits:2 Misses:4]
Access: 6  MISS LRU ->    [8, 7, 6] <- MRU Replaced:0 [Hits:2 Misses:5]
Access: 3  MISS LRU ->    [7, 6, 3] <- MRU Replaced:8 [Hits:2 Misses:6]
Access: 6  HIT  LRU ->    [7, 3, 6] <- MRU Replaced:- [Hits:3 Misses:6]
Access: 6  HIT  LRU ->    [7, 3, 6] <- MRU Replaced:- [Hits:4 Misses:6]

FINALSTATS hits 4   misses 6   hitrate 40.00

$ python paging-policy.py -s 2 -n 10 -p OPT
ARG addresses -1
ARG addressfile
ARG numaddrs 10
ARG policy OPT
ARG clockbits 2
ARG cachesize 3
ARG maxpage 10
ARG seed 2
ARG notrace False

Solving...

Access: 9  MISS Left  ->          [9] <- Right Replaced:- [Hits:0 Misses:1]
Access: 9  HIT  Left  ->          [9] <- Right Replaced:- [Hits:1 Misses:1]
Access: 0  MISS Left  ->       [9, 0] <- Right Replaced:- [Hits:1 Misses:2]
Access: 0  HIT  Left  ->       [9, 0] <- Right Replaced:- [Hits:2 Misses:2]
Access: 8  MISS Left  ->    [9, 0, 8] <- Right Replaced:- [Hits:2 Misses:3]
Access: 7  MISS Left  ->    [9, 0, 7] <- Right Replaced:8 [Hits:2 Misses:4]
Access: 6  MISS Left  ->    [9, 0, 6] <- Right Replaced:7 [Hits:2 Misses:5]
Access: 3  MISS Left  ->    [9, 6, 3] <- Right Replaced:0 [Hits:2 Misses:6]
Access: 6  HIT  Left  ->    [9, 6, 3] <- Right Replaced:- [Hits:3 Misses:6]
Access: 6  HIT  Left  ->    [9, 6, 3] <- Right Replaced:- [Hits:4 Misses:6]

FINALSTATS hits 4   misses 6   hitrate 40.00
```

---

**Q2**: For a cache of size 5, generate worst-case address reference streams for each of the following policies: FIFO, LRU, and MRU (worst-case reference streams cause the most misses possible. For the worst case reference streams, how much bigger of a cache is needed to improve performance dramatically and approach OPT?

**A**: For a cache of size as the same as referenced memory pages, it can improve performance to approach OPT.

```zsh
$ python paging-paging.py -C 5 -p FIFO -a 1,2,3,4,5,6,1,2,3,4
ARG addresses 1,2,3,4,5,6,1,2,3,4
ARG addressfile
ARG numaddrs 10
ARG policy FIFO
ARG clockbits 2
ARG cachesize 5
ARG maxpage 10
ARG seed 0
ARG notrace False

Solving...

Access: 1  MISS FirstIn ->          [1] <- Lastin  Replaced:- [Hits:0 Misses:1]
Access: 2  MISS FirstIn ->       [1, 2] <- Lastin  Replaced:- [Hits:0 Misses:2]
Access: 3  MISS FirstIn ->    [1, 2, 3] <- Lastin  Replaced:- [Hits:0 Misses:3]
Access: 4  MISS FirstIn -> [1, 2, 3, 4] <- Lastin  Replaced:- [Hits:0 Misses:4]
Access: 5  MISS FirstIn -> [1, 2, 3, 4, 5] <- Lastin  Replaced:- [Hits:0 Misses:5]
Access: 6  MISS FirstIn -> [2, 3, 4, 5, 6] <- Lastin  Replaced:1 [Hits:0 Misses:6]
Access: 1  MISS FirstIn -> [3, 4, 5, 6, 1] <- Lastin  Replaced:2 [Hits:0 Misses:7]
Access: 2  MISS FirstIn -> [4, 5, 6, 1, 2] <- Lastin  Replaced:3 [Hits:0 Misses:8]
Access: 3  MISS FirstIn -> [5, 6, 1, 2, 3] <- Lastin  Replaced:4 [Hits:0 Misses:9]
Access: 4  MISS FirstIn -> [6, 1, 2, 3, 4] <- Lastin  Replaced:5 [Hits:0 Misses:10]

FINALSTATS hits 0   misses 10   hitrate 0.00
```

```zsh
$ python paging-policy.py -C 5 -p LRU -a 1,2,3,4,5,6,1,2,3,4
ARG addresses 1,2,3,4,5,6,1,2,3,4
ARG addressfile
ARG numaddrs 10
ARG policy LRU
ARG clockbits 2
ARG cachesize 5
ARG maxpage 10
ARG seed 0
ARG notrace False

Solving...

Access: 1  MISS LRU ->          [1] <- MRU Replaced:- [Hits:0 Misses:1]
Access: 2  MISS LRU ->       [1, 2] <- MRU Replaced:- [Hits:0 Misses:2]
Access: 3  MISS LRU ->    [1, 2, 3] <- MRU Replaced:- [Hits:0 Misses:3]
Access: 4  MISS LRU -> [1, 2, 3, 4] <- MRU Replaced:- [Hits:0 Misses:4]
Access: 5  MISS LRU -> [1, 2, 3, 4, 5] <- MRU Replaced:- [Hits:0 Misses:5]
Access: 6  MISS LRU -> [2, 3, 4, 5, 6] <- MRU Replaced:1 [Hits:0 Misses:6]
Access: 1  MISS LRU -> [3, 4, 5, 6, 1] <- MRU Replaced:2 [Hits:0 Misses:7]
Access: 2  MISS LRU -> [4, 5, 6, 1, 2] <- MRU Replaced:3 [Hits:0 Misses:8]
Access: 3  MISS LRU -> [5, 6, 1, 2, 3] <- MRU Replaced:4 [Hits:0 Misses:9]
Access: 4  MISS LRU -> [6, 1, 2, 3, 4] <- MRU Replaced:5 [Hits:0 Misses:10]

FINALSTATS hits 0   misses 10   hitrate 0.00
```

```zsh
$ python paging-policy.py -C 5 -p MRU -a 1,2,3,4,5,6,5,6,5,6
ARG addresses 1,2,3,4,5,6,5,6,5,6
ARG addressfile
ARG numaddrs 10
ARG policy MRU
ARG clockbits 2
ARG cachesize 5
ARG maxpage 10
ARG seed 0
ARG notrace False

Solving...

Access: 1  MISS LRU ->          [1] <- MRU Replaced:- [Hits:0 Misses:1]
Access: 2  MISS LRU ->       [1, 2] <- MRU Replaced:- [Hits:0 Misses:2]
Access: 3  MISS LRU ->    [1, 2, 3] <- MRU Replaced:- [Hits:0 Misses:3]
Access: 4  MISS LRU -> [1, 2, 3, 4] <- MRU Replaced:- [Hits:0 Misses:4]
Access: 5  MISS LRU -> [1, 2, 3, 4, 5] <- MRU Replaced:- [Hits:0 Misses:5]
Access: 6  MISS LRU -> [1, 2, 3, 4, 6] <- MRU Replaced:5 [Hits:0 Misses:6]
Access: 5  MISS LRU -> [1, 2, 3, 4, 5] <- MRU Replaced:6 [Hits:0 Misses:7]
Access: 6  MISS LRU -> [1, 2, 3, 4, 6] <- MRU Replaced:5 [Hits:0 Misses:8]
Access: 5  MISS LRU -> [1, 2, 3, 4, 5] <- MRU Replaced:6 [Hits:0 Misses:9]
Access: 6  MISS LRU -> [1, 2, 3, 4, 6] <- MRU Replaced:5 [Hits:0 Misses:10]

FINALSTATS hits 0   misses 10   hitrate 0.00
```

---

**Q3**: Generate a random trace (use python or perl). How would you expect the different policies to perform on such a trace?

**A**:

```zsh
$ python paging-policy.py -C 5 -p FIFO -a 1,3,3,2,4,6,4,2,5,1,2
ARG addresses 1,3,3,2,4,6,4,2,5,1,2
ARG addressfile
ARG numaddrs 10
ARG policy FIFO
ARG clockbits 2
ARG cachesize 5
ARG maxpage 10
ARG seed 0
ARG notrace False

Solving...

Access: 1  MISS FirstIn ->          [1] <- Lastin  Replaced:- [Hits:0 Misses:1]
Access: 3  MISS FirstIn ->       [1, 3] <- Lastin  Replaced:- [Hits:0 Misses:2]
Access: 3  HIT  FirstIn ->       [1, 3] <- Lastin  Replaced:- [Hits:1 Misses:2]
Access: 2  MISS FirstIn ->    [1, 3, 2] <- Lastin  Replaced:- [Hits:1 Misses:3]
Access: 4  MISS FirstIn -> [1, 3, 2, 4] <- Lastin  Replaced:- [Hits:1 Misses:4]
Access: 6  MISS FirstIn -> [1, 3, 2, 4, 6] <- Lastin  Replaced:- [Hits:1 Misses:5]
Access: 4  HIT  FirstIn -> [1, 3, 2, 4, 6] <- Lastin  Replaced:- [Hits:2 Misses:5]
Access: 2  HIT  FirstIn -> [1, 3, 2, 4, 6] <- Lastin  Replaced:- [Hits:3 Misses:5]
Access: 5  MISS FirstIn -> [3, 2, 4, 6, 5] <- Lastin  Replaced:1 [Hits:3 Misses:6]
Access: 1  MISS FirstIn -> [2, 4, 6, 5, 1] <- Lastin  Replaced:3 [Hits:3 Misses:7]
Access: 2  HIT  FirstIn -> [2, 4, 6, 5, 1] <- Lastin  Replaced:- [Hits:4 Misses:7]

FINALSTATS hits 4   misses 7   hitrate 36.36
```

```zsh
$ python paging-policy.py -C 5 -p LRU -a 1,3,3,2,4,6,4,2,5,1,2
ARG addresses 1,3,3,2,4,6,4,2,5,1,2
ARG addressfile
ARG numaddrs 10
ARG policy LRU
ARG clockbits 2
ARG cachesize 5
ARG maxpage 10
ARG seed 0
ARG notrace False

Solving...

Access: 1  MISS LRU ->          [1] <- MRU Replaced:- [Hits:0 Misses:1]
Access: 3  MISS LRU ->       [1, 3] <- MRU Replaced:- [Hits:0 Misses:2]
Access: 3  HIT  LRU ->       [1, 3] <- MRU Replaced:- [Hits:1 Misses:2]
Access: 2  MISS LRU ->    [1, 3, 2] <- MRU Replaced:- [Hits:1 Misses:3]
Access: 4  MISS LRU -> [1, 3, 2, 4] <- MRU Replaced:- [Hits:1 Misses:4]
Access: 6  MISS LRU -> [1, 3, 2, 4, 6] <- MRU Replaced:- [Hits:1 Misses:5]
Access: 4  HIT  LRU -> [1, 3, 2, 6, 4] <- MRU Replaced:- [Hits:2 Misses:5]
Access: 2  HIT  LRU -> [1, 3, 6, 4, 2] <- MRU Replaced:- [Hits:3 Misses:5]
Access: 5  MISS LRU -> [3, 6, 4, 2, 5] <- MRU Replaced:1 [Hits:3 Misses:6]
Access: 1  MISS LRU -> [6, 4, 2, 5, 1] <- MRU Replaced:3 [Hits:3 Misses:7]
Access: 2  HIT  LRU -> [6, 4, 5, 1, 2] <- MRU Replaced:- [Hits:4 Misses:7]

FINALSTATS hits 4   misses 7   hitrate 36.36
```

```zsh
$ python paging-policy.py -C 5 -p MRU -a 1,3,3,2,4,6,4,2,5,1,2
ARG addresses 1,3,3,2,4,6,4,2,5,1,2
ARG addressfile
ARG numaddrs 10
ARG policy MRU
ARG clockbits 2
ARG cachesize 5
ARG maxpage 10
ARG seed 0
ARG notrace False

Solving...

Access: 1  MISS LRU ->          [1] <- MRU Replaced:- [Hits:0 Misses:1]
Access: 3  MISS LRU ->       [1, 3] <- MRU Replaced:- [Hits:0 Misses:2]
Access: 3  HIT  LRU ->       [1, 3] <- MRU Replaced:- [Hits:1 Misses:2]
Access: 2  MISS LRU ->    [1, 3, 2] <- MRU Replaced:- [Hits:1 Misses:3]
Access: 4  MISS LRU -> [1, 3, 2, 4] <- MRU Replaced:- [Hits:1 Misses:4]
Access: 6  MISS LRU -> [1, 3, 2, 4, 6] <- MRU Replaced:- [Hits:1 Misses:5]
Access: 4  HIT  LRU -> [1, 3, 2, 6, 4] <- MRU Replaced:- [Hits:2 Misses:5]
Access: 2  HIT  LRU -> [1, 3, 6, 4, 2] <- MRU Replaced:- [Hits:3 Misses:5]
Access: 5  MISS LRU -> [1, 3, 6, 4, 5] <- MRU Replaced:2 [Hits:3 Misses:6]
Access: 1  HIT  LRU -> [3, 6, 4, 5, 1] <- MRU Replaced:- [Hits:4 Misses:6]
Access: 2  MISS LRU -> [3, 6, 4, 5, 2] <- MRU Replaced:1 [Hits:4 Misses:7]

FINALSTATS hits 4   misses 7   hitrate 36.36
```

---

**Q4**: Now generate a trace with some locality. How can you generate such a trace? How does LRU perform on it? How much better than RAND is LRU? How does CLOCK do? How about CLOCK with different numbers of clock bits?

**A**:

```zsh
$ python paging-policy.py -C 5 -p RAND -a 1,3,3,2,4,6,4,2,5,1,2
ARG addresses 1,3,3,2,4,6,4,2,5,1,2
ARG addressfile
ARG numaddrs 10
ARG policy RAND
ARG clockbits 2
ARG cachesize 5
ARG maxpage 10
ARG seed 0
ARG notrace False

Solving...

Access: 1  MISS Left  ->          [1] <- Right Replaced:- [Hits:0 Misses:1]
Access: 3  MISS Left  ->       [1, 3] <- Right Replaced:- [Hits:0 Misses:2]
Access: 3  HIT  Left  ->       [1, 3] <- Right Replaced:- [Hits:1 Misses:2]
Access: 2  MISS Left  ->    [1, 3, 2] <- Right Replaced:- [Hits:1 Misses:3]
Access: 4  MISS Left  -> [1, 3, 2, 4] <- Right Replaced:- [Hits:1 Misses:4]
Access: 6  MISS Left  -> [1, 3, 2, 4, 6] <- Right Replaced:- [Hits:1 Misses:5]
Access: 4  HIT  Left  -> [1, 3, 2, 4, 6] <- Right Replaced:- [Hits:2 Misses:5]
Access: 2  HIT  Left  -> [1, 3, 2, 4, 6] <- Right Replaced:- [Hits:3 Misses:5]
Access: 5  MISS Left  -> [1, 3, 2, 4, 5] <- Right Replaced:6 [Hits:3 Misses:6]
Access: 1  HIT  Left  -> [1, 3, 2, 4, 5] <- Right Replaced:- [Hits:4 Misses:6]
Access: 2  HIT  Left  -> [1, 3, 2, 4, 5] <- Right Replaced:- [Hits:5 Misses:6]

FINALSTATS hits 5   misses 6   hitrate 45.45
```

```zsh
$ python paging-policy.py -C 5 -p LRU -a 1,3,3,2,4,6,4,2,5,1,2
ARG addresses 1,3,3,2,4,6,4,2,5,1,2
ARG addressfile
ARG numaddrs 10
ARG policy LRU
ARG clockbits 2
ARG cachesize 5
ARG maxpage 10
ARG seed 0
ARG notrace False

Solving...

Access: 1  MISS LRU ->          [1] <- MRU Replaced:- [Hits:0 Misses:1]
Access: 3  MISS LRU ->       [1, 3] <- MRU Replaced:- [Hits:0 Misses:2]
Access: 3  HIT  LRU ->       [1, 3] <- MRU Replaced:- [Hits:1 Misses:2]
Access: 2  MISS LRU ->    [1, 3, 2] <- MRU Replaced:- [Hits:1 Misses:3]
Access: 4  MISS LRU -> [1, 3, 2, 4] <- MRU Replaced:- [Hits:1 Misses:4]
Access: 6  MISS LRU -> [1, 3, 2, 4, 6] <- MRU Replaced:- [Hits:1 Misses:5]
Access: 4  HIT  LRU -> [1, 3, 2, 6, 4] <- MRU Replaced:- [Hits:2 Misses:5]
Access: 2  HIT  LRU -> [1, 3, 6, 4, 2] <- MRU Replaced:- [Hits:3 Misses:5]
Access: 5  MISS LRU -> [3, 6, 4, 2, 5] <- MRU Replaced:1 [Hits:3 Misses:6]
Access: 1  MISS LRU -> [6, 4, 2, 5, 1] <- MRU Replaced:3 [Hits:3 Misses:7]
Access: 2  HIT  LRU -> [6, 4, 5, 1, 2] <- MRU Replaced:- [Hits:4 Misses:7]

FINALSTATS hits 4   misses 7   hitrate 36.36
```

```zsh
$ python paging-policy.py -C 5 -p CLOCK -a 1,3,3,2,4,6,4,2,5,1,2
ARG addresses 1,3,3,2,4,6,4,2,5,1,2
ARG addressfile
ARG numaddrs 10
ARG policy CLOCK
ARG clockbits 2
ARG cachesize 5
ARG maxpage 10
ARG seed 0
ARG notrace False

Solving...

Access: 1  MISS Left  ->          [1] <- Right Replaced:- [Hits:0 Misses:1]
Access: 3  MISS Left  ->       [1, 3] <- Right Replaced:- [Hits:0 Misses:2]
Access: 3  HIT  Left  ->       [1, 3] <- Right Replaced:- [Hits:1 Misses:2]
Access: 2  MISS Left  ->    [1, 3, 2] <- Right Replaced:- [Hits:1 Misses:3]
Access: 4  MISS Left  -> [1, 3, 2, 4] <- Right Replaced:- [Hits:1 Misses:4]
Access: 6  MISS Left  -> [1, 3, 2, 4, 6] <- Right Replaced:- [Hits:1 Misses:5]
Access: 4  HIT  Left  -> [1, 3, 2, 4, 6] <- Right Replaced:- [Hits:2 Misses:5]
Access: 2  HIT  Left  -> [1, 3, 2, 4, 6] <- Right Replaced:- [Hits:3 Misses:5]
Access: 5  MISS Left  -> [1, 3, 4, 6, 5] <- Right Replaced:2 [Hits:3 Misses:6]
Access: 1  HIT  Left  -> [1, 3, 4, 6, 5] <- Right Replaced:- [Hits:4 Misses:6]
Access: 2  MISS Left  -> [1, 3, 4, 5, 2] <- Right Replaced:6 [Hits:4 Misses:7]

FINALSTATS hits 4   misses 7   hitrate 36.36
```

```zsh
$ python paging-policy.py -C 5 -p CLOCK -a 1,3,3,2,4,6,4,2,5,1,2 -b 10
ARG addresses 1,3,3,2,4,6,4,2,5,1,2
ARG addressfile
ARG numaddrs 10
ARG policy CLOCK
ARG clockbits 10
ARG cachesize 5
ARG maxpage 10
ARG seed 0
ARG notrace False

Solving...

Access: 1  MISS Left  ->          [1] <- Right Replaced:- [Hits:0 Misses:1]
Access: 3  MISS Left  ->       [1, 3] <- Right Replaced:- [Hits:0 Misses:2]
Access: 3  HIT  Left  ->       [1, 3] <- Right Replaced:- [Hits:1 Misses:2]
Access: 2  MISS Left  ->    [1, 3, 2] <- Right Replaced:- [Hits:1 Misses:3]
Access: 4  MISS Left  -> [1, 3, 2, 4] <- Right Replaced:- [Hits:1 Misses:4]
Access: 6  MISS Left  -> [1, 3, 2, 4, 6] <- Right Replaced:- [Hits:1 Misses:5]
Access: 4  HIT  Left  -> [1, 3, 2, 4, 6] <- Right Replaced:- [Hits:2 Misses:5]
Access: 2  HIT  Left  -> [1, 3, 2, 4, 6] <- Right Replaced:- [Hits:3 Misses:5]
Access: 5  MISS Left  -> [1, 3, 4, 6, 5] <- Right Replaced:2 [Hits:3 Misses:6]
Access: 1  HIT  Left  -> [1, 3, 4, 6, 5] <- Right Replaced:- [Hits:4 Misses:6]
Access: 2  MISS Left  -> [1, 3, 4, 5, 2] <- Right Replaced:6 [Hits:4 Misses:7]

FINALSTATS hits 4   misses 7   hitrate 36.36
```
