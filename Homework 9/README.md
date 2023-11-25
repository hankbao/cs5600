# Homework 9

## Part 1: Chapter 29 Code

**Q1**: We’ll start by redoing the measurements within this chapter. Use the call `gettimeofday()` to measure time within your program.  How accurate is this timer? What is the smallest interval it can measure? Gain confidence in its workings, as we will need it in all subsequent questions. You can also look into other timers, such as the cycle counter available on x86 via the `rdtsc` instruction.

**A**: Although the `gettimeofday()` provides time in microseconds, its accuracy can depend on the system and its hardware. It's generally considered to have microsecond resolution, but the actual precision can be much lower. The `rdtsc` instruction has one clock cycle resolution.

---

**Q2**: Now, build a simple concurrent counter and measure how long it takes to increment the counter many times as the number of threads increases. How many CPUs are available on the system you are using? Does this number impact your measurements at all?

**A**: Please see the code in [simple_counter.c](./simple_counter.c). There are 10 CPU cores available on my system. The number of CPU cores does impact the measurements. The more CPU cores, the faster the counter increments. With more CPU cores, threads can run in a more parallel fashion.

```zsh
$ sysctl -n hw.logicalcpu
10
```

---

**Q3**: Next, build a version of the approximate counter. Once again, measure its performance as the number of threads varies, as well as the threshold. Do the numbers match what you see in the chapter?

**A**: Please see the code in [approximate_conter.c](./approximate_counter.c). The numbers do match what I see in the chapter.

---

**Q4**: Build a version of a linked list that uses hand-over-hand locking [MS04], as cited in the chapter. You should read the paper first to understand how it works, and then implement it. Measure its performance. When does a hand-over-hand list work better than a standard list as shown in the chapter?

**A**: Please see the code in [hand_over_hand.c](./hand_over_hand.c). In practice, it is hard to make such a structure faster than the simple single lock approach, as the overheads of acquiring and releasing locks for each node of a list traversal is prohibitive. Also, the insertion is not protected with a list-level lock.

---

**Q5**: Pick your favorite data structure, such as a B-tree or other slightly more interesting structure. Implement it, and start with a simple locking strategy such as a single lock. Measure its performance as the number of concurrent threads increases.

**A**: Please see the code in [hash_table.c](./hash_table.c).

---

**Q6**: Finally, think of a more interesting locking strategy for this favorite data structure of yours. Implement it, and measure its performance. How does it compare to the straightforward locking approach?

**A**: We could also deploy the hand-over-hand locking strategy for the hash table. It may have the performance gain in theory.

## Part 2: Chapter 30 Code

## Part 3: Chapter 31 Code
