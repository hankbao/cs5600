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

**Q1**: Our first question focuses on main-two-cvs-while.c (the working solution). First, study the code. Do you think you have an understanding of what should happen when you run the program?

**A**:

---

**Q2**: Run with one producer and one consumer, and have the producer produce a few values. Start with a buffer (size 1), and then increase it. How does the behavior of the code change with larger buffers?  (or does it?) What would you predict `num_full` to be with different buffer sizes (e.g., `-m 10`) and different numbers of produced items (e.g., `-l 100`), when you change the consumer sleep string from default (no sleep) to `-C 0,0,0,0,0,0,1`?

**A**:

---

**Q3**: If possible, run the code on different systems (e.g., a Mac and Linux).  Do you see different behavior across these systems?

**A**:

---

**Q4**: Let’s look at some timings. How long do you think the following execution, with one producer, three consumers, a single-entry shared buffer, and each consumer pausing at point `c3` for a second, will take? `./main-two-cvs-while -p 1 -c 3 -m 1 -C 0,0,0,1,0,0,0:0,0,0,1,0,0,0:0,0,0,1,0,0,0 -l 10 -v -t`

**A**:

---

**Q5**: Now change the size of the shared buffer to 3 (`-m 3`). Will this make any difference in the total time?

**A**:

---

**Q6**: Now change the location of the sleep to `c6` (this models a consumer taking something off the queue and then doing something with it), again using a single-entry buffer. What time do you predict in this case? `./main-two-cvs-while -p 1 -c 3 -m 1 -C 0,0,0,0,0,0,1:0,0,0,0,0,0,1:0,0,0,0,0,0,1 -l 10 -v -t`

**A**:

---

**Q7**: Finally, change the buffer size to 3 again (`-m 3`). What time do you predict now?

**A**:

---

**Q8**: Now let’s look at `main-one-cv-while.c`. Can you configure a sleep string, assuming a single producer, one consumer, and a buffer of size 1, to cause a problem with this code?

**A**:

---

**Q9**: Now change the number of consumers to two. Can you construct sleep strings for the producer and the consumers so as to cause a problem in the code?

**A**:

---

**Q10**: Now examine `main-two-cvs-if.c`. Can you cause a problem to happen in this code? Again consider the case where there is only one consumer, and then the case where there is more than one.

**A**:

---

**Q11**: Finally, examine `main-two-cvs-while-extra-unlock.c`. What problem arises when you release the lock before doing a put or a get? Can you reliably cause such a problem to happen, given the sleep strings? What bad thing can happen?

**A**:

## Part 3: Chapter 31 Code

**Q1**: The first problem is just to implement and test a solution to the **fork/join** problem, as described in the text. Even though this solution is described in the text, the act of typing it in on your own is worthwhile; even Bach would rewrite Vivaldi, allowing one soon-to-be master to learn from an existing one. See `fork-join.c` for details. Add the call `sleep(1)` to the child to ensure it is working.

**A**:

---

**Q2**: Let’s now generalize this a bit by investigating the **rendezvous problem**.  The problem is as follows: you have two threads, each of which are about to enter the rendezvous point in the code. Neither should exit this part of the code before the other enters it. Consider using two semaphores for this task, and see `rendezvous.c` for details.

**A**:

---

**Q3**: Now go one step further by implementing a general solution to **barrier synchronization**. Assume there are two points in a sequential piece of code, called *P1* and *P2*. Putting a barrier between *P1* and *P2* guarantees that all threads will execute *P1* before any one thread executes *P2*. Your task: write the code to implement a `barrier()` function that can be used in this manner. It is safe to assume you know *N* (the total number of threads in the running program) and that all *N* threads will try to enter the barrier. Again, you should likely use two semaphores to achieve the solution, and some other integers to count things. See `barrier.c` for details.

**A**:

---

**Q4**: Now let’s solve the **reader-writer problem**, also as described in the text. In this first take, don’t worry about starvation. See the code in `reader-writer.c` for details. Add `sleep()` calls to your code to demonstrate it works as you expect. Can you show the existence of the starvation problem?

**A**:

---

**Q5**: Let’s look at the reader-writer problem again, but this time, worry about starvation. How can you ensure that all readers and writers eventually make progress? See `reader-writer-nostarve.c` for details.

**A**:

---

**Q6**: Use semaphores to build a **no-starve mutex**, in which any thread that tries to acquire the mutex will eventually obtain it. See the code in `mutex-nostarve.c` for more information.

**A**:

---

**Q7**: Liked these problems? See Downey’s free text for more just like them. And don’t forget, have fun! But, you always do when you write code, no?

**A**:
