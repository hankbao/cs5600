# Homework 8

## Part 1: Chapter 26 Simulation

**Q1**: Let’s examine a simple program, “loop.s”. First, just read and understand it. Then, run it with these arguments (`./x86.py -p loop.s -t 1 -i 100 -R dx`) This specifies a single thread, an interrupt every 100 instructions, and tracing of register `%dx`. What will `%dx` be during the run? Use the `-c` flag to check your answers; the answers, on the left, show the value of the register (or memory value) _after_ the instruction on the right has run.

**A**: %dx will be -1.

```zsh
$ python x86.py -p loop.s -t 1 -i 100 -R dx
ARG seed 0
ARG numthreads 1
ARG program loop.s
ARG interrupt frequency 100
ARG interrupt randomness False
ARG argv
ARG load address 1000
ARG memsize 128
ARG memtrace
ARG regtrace dx
ARG cctrace False
ARG printstats False
ARG verbose False

   dx          Thread 0
    0
   -1   1000 sub  $1,%dx
   -1   1001 test $0,%dx
   -1   1002 jgte .top
   -1   1003 halt
```

---

**Q2**: Same code, different flags: (`./x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx`) This specifies two threads, and initializes each %dx to 3. What values will `%dx` see? Run with `-c` to check. Does the presence of multiple threads affect your calculations? Is there a race in this code?

**A**: No. Because the interrupt frequency is 100, the first thread will run until it reaches the halt instruction, so there is no race condition. Also that the sub and test instructions will be executed atomically.

```zsh
$ python3 x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx
ARG seed 0
ARG numthreads 2
ARG program loop.s
ARG interrupt frequency 100
ARG interrupt randomness False
ARG argv dx=3,dx=3
ARG load address 1000
ARG memsize 128
ARG memtrace
ARG regtrace dx
ARG cctrace False
ARG printstats False
ARG verbose False

   dx          Thread 0                Thread 1
    3
    2   1000 sub  $1,%dx
    2   1001 test $0,%dx
    2   1002 jgte .top
    1   1000 sub  $1,%dx
    1   1001 test $0,%dx
    1   1002 jgte .top
    0   1000 sub  $1,%dx
    0   1001 test $0,%dx
    0   1002 jgte .top
   -1   1000 sub  $1,%dx
   -1   1001 test $0,%dx
   -1   1002 jgte .top
   -1   1003 halt
    3   ----- Halt;Switch -----  ----- Halt;Switch -----
    2                            1000 sub  $1,%dx
    2                            1001 test $0,%dx
    2                            1002 jgte .top
    1                            1000 sub  $1,%dx
    1                            1001 test $0,%dx
    1                            1002 jgte .top
    0                            1000 sub  $1,%dx
    0                            1001 test $0,%dx
    0                            1002 jgte .top
   -1                            1000 sub  $1,%dx
   -1                            1001 test $0,%dx
   -1                            1002 jgte .top
   -1                            1003 halt
```

---

**Q3**: Run this: `./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx` This makes the interrupt interval small/random; use different seeds (`-s`) to see different interleavings. Does the interrupt frequency change anything?

**A**: The smaller interrupt frequency makes the two threads interleaved. But the sub and test instructions are atomic operations, and each thread accesses its own %dx. So no race condition here.

```zsh
$ python3 x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx -s 1
ARG seed 1
ARG numthreads 2
ARG program loop.s
ARG interrupt frequency 3
ARG interrupt randomness True
ARG argv dx=3,dx=3
ARG load address 1000
ARG memsize 128
ARG memtrace
ARG regtrace dx
ARG cctrace False
ARG printstats False
ARG verbose False

   dx          Thread 0                Thread 1
    3
    2   1000 sub  $1,%dx
    3   ------ Interrupt ------  ------ Interrupt ------
    2                            1000 sub  $1,%dx
    2                            1001 test $0,%dx
    2                            1002 jgte .top
    2   ------ Interrupt ------  ------ Interrupt ------
    2   1001 test $0,%dx
    2   1002 jgte .top
    1   1000 sub  $1,%dx
    2   ------ Interrupt ------  ------ Interrupt ------
    1                            1000 sub  $1,%dx
    1   ------ Interrupt ------  ------ Interrupt ------
    1   1001 test $0,%dx
    1   1002 jgte .top
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1001 test $0,%dx
    1                            1002 jgte .top
    1   ------ Interrupt ------  ------ Interrupt ------
    0   1000 sub  $1,%dx
    0   1001 test $0,%dx
    1   ------ Interrupt ------  ------ Interrupt ------
    0                            1000 sub  $1,%dx
    0                            1001 test $0,%dx
    0                            1002 jgte .top
    0   ------ Interrupt ------  ------ Interrupt ------
    0   1002 jgte .top
    0   ------ Interrupt ------  ------ Interrupt ------
   -1                            1000 sub  $1,%dx
    0   ------ Interrupt ------  ------ Interrupt ------
   -1   1000 sub  $1,%dx
   -1   1001 test $0,%dx
   -1   1002 jgte .top
   -1   ------ Interrupt ------  ------ Interrupt ------
   -1                            1001 test $0,%dx
   -1                            1002 jgte .top
   -1   ------ Interrupt ------  ------ Interrupt ------
   -1   1003 halt
   -1   ----- Halt;Switch -----  ----- Halt;Switch -----
   -1                            1003 halt
```

```zsh
$ python3 x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx -s 2
ARG seed 2
ARG numthreads 2
ARG program loop.s
ARG interrupt frequency 3
ARG interrupt randomness True
ARG argv dx=3,dx=3
ARG load address 1000
ARG memsize 128
ARG memtrace
ARG regtrace dx
ARG cctrace False
ARG printstats False
ARG verbose False

   dx          Thread 0                Thread 1
    3
    2   1000 sub  $1,%dx
    2   1001 test $0,%dx
    2   1002 jgte .top
    3   ------ Interrupt ------  ------ Interrupt ------
    2                            1000 sub  $1,%dx
    2                            1001 test $0,%dx
    2                            1002 jgte .top
    2   ------ Interrupt ------  ------ Interrupt ------
    1   1000 sub  $1,%dx
    2   ------ Interrupt ------  ------ Interrupt ------
    1                            1000 sub  $1,%dx
    1   ------ Interrupt ------  ------ Interrupt ------
    1   1001 test $0,%dx
    1   1002 jgte .top
    0   1000 sub  $1,%dx
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1001 test $0,%dx
    1                            1002 jgte .top
    0                            1000 sub  $1,%dx
    0   ------ Interrupt ------  ------ Interrupt ------
    0   1001 test $0,%dx
    0   1002 jgte .top
   -1   1000 sub  $1,%dx
    0   ------ Interrupt ------  ------ Interrupt ------
    0                            1001 test $0,%dx
   -1   ------ Interrupt ------  ------ Interrupt ------
   -1   1001 test $0,%dx
   -1   1002 jgte .top
    0   ------ Interrupt ------  ------ Interrupt ------
    0                            1002 jgte .top
   -1                            1000 sub  $1,%dx
   -1   ------ Interrupt ------  ------ Interrupt ------
   -1   1003 halt
   -1   ----- Halt;Switch -----  ----- Halt;Switch -----
   -1                            1001 test $0,%dx
   -1   ------ Interrupt ------  ------ Interrupt ------
   -1                            1002 jgte .top
   -1   ------ Interrupt ------  ------ Interrupt ------
   -1                            1003 halt
```

```zsh
$ python3 x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx -s 3
ARG seed 3
ARG numthreads 2
ARG program loop.s
ARG interrupt frequency 3
ARG interrupt randomness True
ARG argv dx=3,dx=3
ARG load address 1000
ARG memsize 128
ARG memtrace
ARG regtrace dx
ARG cctrace False
ARG printstats False
ARG verbose False

   dx          Thread 0                Thread 1
    3
    2   1000 sub  $1,%dx
    3   ------ Interrupt ------  ------ Interrupt ------
    2                            1000 sub  $1,%dx
    2                            1001 test $0,%dx
    2   ------ Interrupt ------  ------ Interrupt ------
    2   1001 test $0,%dx
    2   1002 jgte .top
    2   ------ Interrupt ------  ------ Interrupt ------
    2                            1002 jgte .top
    1                            1000 sub  $1,%dx
    2   ------ Interrupt ------  ------ Interrupt ------
    1   1000 sub  $1,%dx
    1   1001 test $0,%dx
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1001 test $0,%dx
    1   ------ Interrupt ------  ------ Interrupt ------
    1   1002 jgte .top
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1002 jgte .top
    0                            1000 sub  $1,%dx
    0                            1001 test $0,%dx
    1   ------ Interrupt ------  ------ Interrupt ------
    0   1000 sub  $1,%dx
    0   ------ Interrupt ------  ------ Interrupt ------
    0                            1002 jgte .top
    0   ------ Interrupt ------  ------ Interrupt ------
    0   1001 test $0,%dx
    0   1002 jgte .top
   -1   1000 sub  $1,%dx
    0   ------ Interrupt ------  ------ Interrupt ------
   -1                            1000 sub  $1,%dx
   -1                            1001 test $0,%dx
   -1   ------ Interrupt ------  ------ Interrupt ------
   -1   1001 test $0,%dx
   -1   1002 jgte .top
   -1   1003 halt
   -1   ----- Halt;Switch -----  ----- Halt;Switch -----
   -1   ------ Interrupt ------  ------ Interrupt ------
   -1                            1002 jgte .top
   -1                            1003 halt
```

---

**Q4**: Now, a different program, `looping-race-nolock.s`, which accesses a shared variable located at address 2000; we’ll call this variable `value`. Run it with a single thread to confirm your understanding: `./x86.py -p looping-race-nolock.s -t 1 -M 2000` What is `value` (i.e., at memory address 2000) throughout the run? Use `-c` to check.

**A**: 0 to 1.

```zsh
$ python3 x86.py -p looping-race-nolock.s -t 1 -M 2000
ARG seed 0
ARG numthreads 1
ARG program looping-race-nolock.s
ARG interrupt frequency 50
ARG interrupt randomness False
ARG argv
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace
ARG cctrace False
ARG printstats False
ARG verbose False

 2000          Thread 0
    0
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   1006 halt
```

---

**Q5**: Run with multiple iterations/threads: `./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000` Why does each thread loop three times? What is final value of `value`?

**A**: Final value is 6.

```zsh
$ python3 x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000
ARG seed 0
ARG numthreads 2
ARG program looping-race-nolock.s
ARG interrupt frequency 50
ARG interrupt randomness False
ARG argv bx=3
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace
ARG cctrace False
ARG printstats False
ARG verbose False

 2000          Thread 0                Thread 1
    0
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   1000 mov 2000, %ax
    1   1001 add $1, %ax
    2   1002 mov %ax, 2000
    2   1003 sub  $1, %bx
    2   1004 test $0, %bx
    2   1005 jgt .top
    2   1000 mov 2000, %ax
    2   1001 add $1, %ax
    3   1002 mov %ax, 2000
    3   1003 sub  $1, %bx
    3   1004 test $0, %bx
    3   1005 jgt .top
    3   1006 halt
    3   ----- Halt;Switch -----  ----- Halt;Switch -----
    3                            1000 mov 2000, %ax
    3                            1001 add $1, %ax
    4                            1002 mov %ax, 2000
    4                            1003 sub  $1, %bx
    4                            1004 test $0, %bx
    4                            1005 jgt .top
    4                            1000 mov 2000, %ax
    4                            1001 add $1, %ax
    5                            1002 mov %ax, 2000
    5                            1003 sub  $1, %bx
    5                            1004 test $0, %bx
    5                            1005 jgt .top
    5                            1000 mov 2000, %ax
    5                            1001 add $1, %ax
    6                            1002 mov %ax, 2000
    6                            1003 sub  $1, %bx
    6                            1004 test $0, %bx
    6                            1005 jgt .top
    6                            1006 halt
```

---

**Q6**:  Run with random interrupt intervals: `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0` with different seeds (`-s 1, -s 2,` etc.) Can you tell by looking at the thread interleaving what the final value of `value` will be? Does the timing of the interrupt matter? Where can it safely occur? Where not? In other words, where is the critical section exactly?

**A**: The timing of the interrupt matters. The critical section is from line 6-8.

```zsh
$ python3 x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0
ARG seed 0
ARG numthreads 2
ARG program looping-race-nolock.s
ARG interrupt frequency 4
ARG interrupt randomness True
ARG argv
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace
ARG cctrace False
ARG printstats False
ARG verbose False

 2000          Thread 0                Thread 1
    0
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1000 mov 2000, %ax
    1                            1001 add $1, %ax
    2                            1002 mov %ax, 2000
    2                            1003 sub  $1, %bx
    2   ------ Interrupt ------  ------ Interrupt ------
    2   1004 test $0, %bx
    2   1005 jgt .top
    2   ------ Interrupt ------  ------ Interrupt ------
    2                            1004 test $0, %bx
    2                            1005 jgt .top
    2   ------ Interrupt ------  ------ Interrupt ------
    2   1006 halt
    2   ----- Halt;Switch -----  ----- Halt;Switch -----
    2                            1006 halt
```

```zsh
$ python3 x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 1
ARG seed 1
ARG numthreads 2
ARG program looping-race-nolock.s
ARG interrupt frequency 4
ARG interrupt randomness True
ARG argv
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace
ARG cctrace False
ARG printstats False
ARG verbose False

 2000          Thread 0                Thread 1
    0
    0   1000 mov 2000, %ax
    0   ------ Interrupt ------  ------ Interrupt ------
    0                            1000 mov 2000, %ax
    0                            1001 add $1, %ax
    1                            1002 mov %ax, 2000
    1                            1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------
    1   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   1004 test $0, %bx
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1004 test $0, %bx
    1                            1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------
    1   1005 jgt .top
    1   1006 halt
    1   ----- Halt;Switch -----  ----- Halt;Switch -----
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1006 halt
```

```zsh
$ python3 x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 2
ARG seed 2
ARG numthreads 2
ARG program looping-race-nolock.s
ARG interrupt frequency 4
ARG interrupt randomness True
ARG argv
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace
ARG cctrace False
ARG printstats False
ARG verbose False

 2000          Thread 0                Thread 1
    0
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1000 mov 2000, %ax
    1                            1001 add $1, %ax
    2                            1002 mov %ax, 2000
    2                            1003 sub  $1, %bx
    2   ------ Interrupt ------  ------ Interrupt ------
    2   1004 test $0, %bx
    2   ------ Interrupt ------  ------ Interrupt ------
    2                            1004 test $0, %bx
    2   ------ Interrupt ------  ------ Interrupt ------
    2   1005 jgt .top
    2   1006 halt
    2   ----- Halt;Switch -----  ----- Halt;Switch -----
    2                            1005 jgt .top
    2                            1006 halt
```

---

**Q7**: Now examine fixed interrupt intervals: `./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1` What will the final value of the shared variable `value` be? What about when you change `-i 2, -i 3`, etc.? For which interrupt intervals does the program give the “correct” answer?

**A**: 1, 1, 2 for -i 1, -i 2 and -i 3. The program gives the "correct" answer with `-i 3`

```zsh
$ python3 x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1
ARG seed 0
ARG numthreads 2
ARG program looping-race-nolock.s
ARG interrupt frequency 1
ARG interrupt randomness False
ARG argv bx=1
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace
ARG cctrace False
ARG printstats False
ARG verbose False

 2000          Thread 0                Thread 1
    0
    0   1000 mov 2000, %ax
    0   ------ Interrupt ------  ------ Interrupt ------
    0                            1000 mov 2000, %ax
    0   ------ Interrupt ------  ------ Interrupt ------
    0   1001 add $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------
    0                            1001 add $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------
    1   1002 mov %ax, 2000
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1002 mov %ax, 2000
    1   ------ Interrupt ------  ------ Interrupt ------
    1   1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------
    1   1004 test $0, %bx
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1004 test $0, %bx
    1   ------ Interrupt ------  ------ Interrupt ------
    1   1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------
    1   1006 halt
    1   ----- Halt;Switch -----  ----- Halt;Switch -----
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1006 halt
```

```zsh
$ python3 x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 2
ARG seed 0
ARG numthreads 2
ARG program looping-race-nolock.s
ARG interrupt frequency 2
ARG interrupt randomness False
ARG argv bx=1
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace
ARG cctrace False
ARG printstats False
ARG verbose False

 2000          Thread 0                Thread 1
    0
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------
    0                            1000 mov 2000, %ax
    0                            1001 add $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1002 mov %ax, 2000
    1                            1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1004 test $0, %bx
    1                            1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------
    1   1006 halt
    1   ----- Halt;Switch -----  ----- Halt;Switch -----
    1                            1006 halt
```

```zsh
$ python3 x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 3
ARG seed 0
ARG numthreads 2
ARG program looping-race-nolock.s
ARG interrupt frequency 3
ARG interrupt randomness False
ARG argv bx=1
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace
ARG cctrace False
ARG printstats False
ARG verbose False

 2000          Thread 0                Thread 1
    0
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1000 mov 2000, %ax
    1                            1001 add $1, %ax
    2                            1002 mov %ax, 2000
    2   ------ Interrupt ------  ------ Interrupt ------
    2   1003 sub  $1, %bx
    2   1004 test $0, %bx
    2   1005 jgt .top
    2   ------ Interrupt ------  ------ Interrupt ------
    2                            1003 sub  $1, %bx
    2                            1004 test $0, %bx
    2                            1005 jgt .top
    2   ------ Interrupt ------  ------ Interrupt ------
    2   1006 halt
    2   ----- Halt;Switch -----  ----- Halt;Switch -----
    2                            1006 halt
```

---

**Q8**: Run the same for more loops (e.g., set `-a bx=100`). What interrupt intervals (`-i`) lead to a correct outcome? Which intervals are surprising?

**A**: The `-i 3` lead to a correct outcome as previously.

---

**Q9**: One last program: `wait-for-me.s`. Run: `./x86.py -p wait-for-me.s -a ax=1,ax=0 -R ax -M 2000` This sets the `%ax` register to 1 for thread 0, and 0 for thread 1, and watches `%ax` and memory location 2000. How should the code behave? How is the value at location 2000 being used by the threads? What will its final value be?

**A**: One thread will be the signaller and the other will be the waiter. The final value will be 1 for memory location 2000. The value at location 2000 is used as a flag to indicate that a signal is fired.

```zsh
$ python3 x86.py -p wait-for-me.s -a ax=1,ax=0 -R ax -M 2000
ARG seed 0
ARG numthreads 2
ARG program wait-for-me.s
ARG interrupt frequency 50
ARG interrupt randomness False
ARG argv ax=1,ax=0
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace ax
ARG cctrace False
ARG printstats False
ARG verbose False

 2000      ax          Thread 0                Thread 1
    0       1
    0       1   1000 test $1, %ax
    0       1   1001 je .signaller
    1       1   1006 mov  $1, 2000
    1       1   1007 halt
    1       0   ----- Halt;Switch -----  ----- Halt;Switch -----
    1       0                            1000 test $1, %ax
    1       0                            1001 je .signaller
    1       0                            1002 mov  2000, %cx
    1       0                            1003 test $1, %cx
    1       0                            1004 jne .waiter
    1       0                            1005 halt
```

---

**Q10**: Now switch the inputs: `./x86.py -p wait-for-me.s -a ax=0,ax=1 -R ax -M 2000` How do the threads behave? What is thread 0 doing? How would changing the interrupt interval (e.g., `-i 1000`, or perhaps to use random intervals) change the trace outcome? Is the program efficiently using the CPU?

**A**: The waiter was waiting with a spin loop until the signaller set the flag. CPU resources are wasted. With `-i 1000`, the thread waits even longer.

```zsh
$ python3 x86.py -p wait-for-me.s -a ax=0,ax=1 -R ax -M 2000
ARG seed 0
ARG numthreads 2
ARG program wait-for-me.s
ARG interrupt frequency 50
ARG interrupt randomness False
ARG argv ax=0,ax=1
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace ax
ARG cctrace False
ARG printstats False
ARG verbose False

 2000      ax          Thread 0                Thread 1
    0       0
    0       0   1000 test $1, %ax
    0       0   1001 je .signaller
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       1   ------ Interrupt ------  ------ Interrupt ------
    0       1                            1000 test $1, %ax
    0       1                            1001 je .signaller
    1       1                            1006 mov  $1, 2000
    1       1                            1007 halt
    1       0   ----- Halt;Switch -----  ----- Halt;Switch -----
    1       0   1002 mov  2000, %cx
    1       0   1003 test $1, %cx
    1       0   1004 jne .waiter
    1       0   1005 halt
```

## Part 2: Chapter 27 Code

**Q1**: First build `main-race.c`. Examine the code so you can see the (hopefully obvious) data race in the code. Now run `helgrind` (by typing `valgrind --tool=helgrind main-race`) to see how it reports the race. Does it point to the right lines of code? What other information does it give to you?

**A**: Yes, it does. It shows the possible data races in the code with addresses and line numbers.

```zsh
$ valgrind --tool=helgrind main-race
==3365== Helgrind, a thread error detector
==3365== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==3365== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==3365== Command: ./main-race
==3365==
==3365== ---Thread-Announcement------------------------------------------
==3365==
==3365== Thread #1 is the program's root thread
==3365==
==3365== ---Thread-Announcement------------------------------------------
==3365==
==3365== Thread #2 was created
==3365==    at 0x518460E: clone (clone.S:71)
==3365==    by 0x4E4BEC4: create_thread (createthread.c:100)
==3365==    by 0x4E4BEC4: pthread_create@@GLIBC_2.2.5 (pthread_create.c:797)
==3365==    by 0x4C38A27: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==3365==    by 0x1087E2: main (main-race.c:14)
==3365==
==3365== ----------------------------------------------------------------
==3365==
==3365== Possible data race during read of size 4 at 0x309014 by thread #1
==3365== Locks held: none
==3365==    at 0x108806: main (main-race.c:15)
==3365==
==3365== This conflicts with a previous write of size 4 by thread #2
==3365== Locks held: none
==3365==    at 0x10879B: worker (main-race.c:8)
==3365==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==3365==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==3365==    by 0x518461E: clone (clone.S:95)
==3365==  Address 0x309014 is 0 bytes inside data symbol "balance"
==3365==
==3365== ----------------------------------------------------------------
==3365==
==3365== Possible data race during write of size 4 at 0x309014 by thread #1
==3365== Locks held: none
==3365==    at 0x10880F: main (main-race.c:15)
==3365==
==3365== This conflicts with a previous write of size 4 by thread #2
==3365== Locks held: none
==3365==    at 0x10879B: worker (main-race.c:8)
==3365==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==3365==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==3365==    by 0x518461E: clone (clone.S:95)
==3365==  Address 0x309014 is 0 bytes inside data symbol "balance"
==3365==
==3365==
==3365== For counts of detected and suppressed errors, rerun with: -v
==3365== Use --history-level=approx or =none to gain increased speed, at
==3365== the cost of reduced accuracy of conflicting-access information
==3365== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
```

---

**Q2**: What happens when you remove one of the offending lines of code?  Now add a lock around one of the updates to the shared variable, and then around both. What does `helgrind` report in each of these cases?

**A**: With one of the offending lines of code removed, `helgrind` reports no errors. With a lock around both, `helgrind` reports no errors. With a lock around one of the updates to the shared variable, `helgrind` reports errors as previous.

```zsh
$ valgrind --tool=helgrind ./main-race
==5997== Helgrind, a thread error detector
==5997== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==5997== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==5997== Command: ./main-race
==5997==
==5997==
==5997== For counts of detected and suppressed errors, rerun with: -v
==5997== Use --history-level=approx or =none to gain increased speed, at
==5997== the cost of reduced accuracy of conflicting-access information
==5997== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

```zsh
$ valgrind --tool=helgrind ./main-race
==6397== Helgrind, a thread error detector
==6397== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==6397== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==6397== Command: ./main-race
==6397==
==6397== ---Thread-Announcement------------------------------------------
==6397==
==6397== Thread #1 is the program's root thread
==6397==
==6397== ---Thread-Announcement------------------------------------------
==6397==
==6397== Thread #2 was created
==6397==    at 0x518460E: clone (clone.S:71)
==6397==    by 0x4E4BEC4: create_thread (createthread.c:100)
==6397==    by 0x4E4BEC4: pthread_create@@GLIBC_2.2.5 (pthread_create.c:797)
==6397==    by 0x4C38A27: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6397==    by 0x1088AE: main (main-race.c:17)
==6397==
==6397== ----------------------------------------------------------------
==6397==
==6397==  Lock at 0x309060 was first observed
==6397==    at 0x4C3603C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6397==    by 0x108851: worker (main-race.c:9)
==6397==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6397==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==6397==    by 0x518461E: clone (clone.S:95)
==6397==  Address 0x309060 is 0 bytes inside data symbol "mutex"
==6397==
==6397== Possible data race during read of size 4 at 0x309040 by thread #1
==6397== Locks held: none
==6397==    at 0x1088D2: main (main-race.c:18)
==6397==
==6397== This conflicts with a previous write of size 4 by thread #2
==6397== Locks held: 1, at address 0x309060
==6397==    at 0x10885B: worker (main-race.c:10)
==6397==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6397==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==6397==    by 0x518461E: clone (clone.S:95)
==6397==  Address 0x309040 is 0 bytes inside data symbol "balance"
==6397==
==6397== ----------------------------------------------------------------
==6397==
==6397==  Lock at 0x309060 was first observed
==6397==    at 0x4C3603C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6397==    by 0x108851: worker (main-race.c:9)
==6397==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6397==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==6397==    by 0x518461E: clone (clone.S:95)
==6397==  Address 0x309060 is 0 bytes inside data symbol "mutex"
==6397==
==6397== Possible data race during write of size 4 at 0x309040 by thread #1
==6397== Locks held: none
==6397==    at 0x1088DB: main (main-race.c:18)
==6397==
==6397== This conflicts with a previous write of size 4 by thread #2
==6397== Locks held: 1, at address 0x309060
==6397==    at 0x10885B: worker (main-race.c:10)
==6397==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==6397==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==6397==    by 0x518461E: clone (clone.S:95)
==6397==  Address 0x309040 is 0 bytes inside data symbol "balance"
==6397==
==6397==
==6397== For counts of detected and suppressed errors, rerun with: -v
==6397== Use --history-level=approx or =none to gain increased speed, at
==6397== the cost of reduced accuracy of conflicting-access information
==6397== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
```

---

**Q3**: Now let’s look at `main-deadlock.c`. Examine the code. This code has a problem known as **deadlock** (which we discuss in much more depth in a forthcoming chapter). Can you see what problem it might have?

**A**: Thread `p1` is trying to acquire `m1` and then `m2`, but thread `p2` is trying to acquire `m2` and then `m1`. This will cause a deadlock.

---

**Q4**: Now run `helgrind` on this code. What does `helgrind` report?

**A**:

```zsh
$ valgrind --tool=helgrind ./main-deadlock
==18662== Helgrind, a thread error detector
==18662== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==18662== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==18662== Command: ./main-deadlock
==18662==
==18662== ---Thread-Announcement------------------------------------------
==18662==
==18662== Thread #3 was created
==18662==    at 0x518460E: clone (clone.S:71)
==18662==    by 0x4E4BEC4: create_thread (createthread.c:100)
==18662==    by 0x4E4BEC4: pthread_create@@GLIBC_2.2.5 (pthread_create.c:797)
==18662==    by 0x4C38A27: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==18662==    by 0x1089E8: main (main-deadlock.c:24)
==18662==
==18662== ----------------------------------------------------------------
==18662==
==18662== Thread #3: lock order "0x30A040 before 0x30A080" violated
==18662==
==18662== Observed (incorrect) order is: acquisition of lock at 0x30A080
==18662==    at 0x4C3603C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==18662==    by 0x1088B6: worker (main-deadlock.c:13)
==18662==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==18662==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==18662==    by 0x518461E: clone (clone.S:95)
==18662==
==18662==  followed by a later acquisition of lock at 0x30A040
==18662==    at 0x4C3603C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==18662==    by 0x1088E5: worker (main-deadlock.c:14)
==18662==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==18662==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==18662==    by 0x518461E: clone (clone.S:95)
==18662==
==18662== Required order was established by acquisition of lock at 0x30A040
==18662==    at 0x4C3603C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==18662==    by 0x108858: worker (main-deadlock.c:10)
==18662==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==18662==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==18662==    by 0x518461E: clone (clone.S:95)
==18662==
==18662==  followed by a later acquisition of lock at 0x30A080
==18662==    at 0x4C3603C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==18662==    by 0x108887: worker (main-deadlock.c:11)
==18662==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==18662==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==18662==    by 0x518461E: clone (clone.S:95)
==18662==
==18662==  Lock at 0x30A040 was first observed
==18662==    at 0x4C3603C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==18662==    by 0x108858: worker (main-deadlock.c:10)
==18662==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==18662==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==18662==    by 0x518461E: clone (clone.S:95)
==18662==  Address 0x30a040 is 0 bytes inside data symbol "m1"
==18662==
==18662==  Lock at 0x30A080 was first observed
==18662==    at 0x4C3603C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==18662==    by 0x108887: worker (main-deadlock.c:11)
==18662==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==18662==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==18662==    by 0x518461E: clone (clone.S:95)
==18662==  Address 0x30a080 is 0 bytes inside data symbol "m2"
==18662==
==18662==
==18662==
==18662== For counts of detected and suppressed errors, rerun with: -v
==18662== Use --history-level=approx or =none to gain increased speed, at
==18662== the cost of reduced accuracy of conflicting-access information
==18662== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 7 from 7)
```

---

**Q5**: Now run `helgrind` on `main-deadlock-global.c`. Examine the code; does it have the same problem that `main-deadlock.c` has? Should `helgrind` be reporting the same error? What does this tell you about tools like `helgrind`?

**A**: Tools like `helgrind` could produced false reports of errors, be it false positive or false negative.

```zsh
$ valgrind --tool=helgrind ./main-deadlock-global
==19985== Helgrind, a thread error detector
==19985== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==19985== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==19985== Command: ./main-deadlock-global
==19985==
==19985== ---Thread-Announcement------------------------------------------
==19985==
==19985== Thread #3 was created
==19985==    at 0x518460E: clone (clone.S:71)
==19985==    by 0x4E4BEC4: create_thread (createthread.c:100)
==19985==    by 0x4E4BEC4: pthread_create@@GLIBC_2.2.5 (pthread_create.c:797)
==19985==    by 0x4C38A27: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==19985==    by 0x108A46: main (main-deadlock-global.c:27)
==19985==
==19985== ----------------------------------------------------------------
==19985==
==19985== Thread #3: lock order "0x30A080 before 0x30A0C0" violated
==19985==
==19985== Observed (incorrect) order is: acquisition of lock at 0x30A0C0
==19985==    at 0x4C3603C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==19985==    by 0x1088E5: worker (main-deadlock-global.c:15)
==19985==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==19985==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==19985==    by 0x518461E: clone (clone.S:95)
==19985==
==19985==  followed by a later acquisition of lock at 0x30A080
==19985==    at 0x4C3603C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==19985==    by 0x108914: worker (main-deadlock-global.c:16)
==19985==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==19985==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==19985==    by 0x518461E: clone (clone.S:95)
==19985==
==19985== Required order was established by acquisition of lock at 0x30A080
==19985==    at 0x4C3603C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==19985==    by 0x108887: worker (main-deadlock-global.c:12)
==19985==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==19985==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==19985==    by 0x518461E: clone (clone.S:95)
==19985==
==19985==  followed by a later acquisition of lock at 0x30A0C0
==19985==    at 0x4C3603C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==19985==    by 0x1088B6: worker (main-deadlock-global.c:13)
==19985==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==19985==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==19985==    by 0x518461E: clone (clone.S:95)
==19985==
==19985==  Lock at 0x30A080 was first observed
==19985==    at 0x4C3603C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==19985==    by 0x108887: worker (main-deadlock-global.c:12)
==19985==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==19985==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==19985==    by 0x518461E: clone (clone.S:95)
==19985==  Address 0x30a080 is 0 bytes inside data symbol "m1"
==19985==
==19985==  Lock at 0x30A0C0 was first observed
==19985==    at 0x4C3603C: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==19985==    by 0x1088B6: worker (main-deadlock-global.c:13)
==19985==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==19985==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==19985==    by 0x518461E: clone (clone.S:95)
==19985==  Address 0x30a0c0 is 0 bytes inside data symbol "m2"
==19985==
==19985==
==19985==
==19985== For counts of detected and suppressed errors, rerun with: -v
==19985== Use --history-level=approx or =none to gain increased speed, at
==19985== the cost of reduced accuracy of conflicting-access information
==19985== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 7 from 7)
```

---

**Q6**: Let’s next look at `main-signal.c`. This code uses a variable (`done`) to signal that the child is done and that the parent can now continue.  Why is this code inefficient? (what does the parent end up spending its time doing, particularly if the child thread takes a long time to complete?)

**A**: The parent thread is waiting with a spin loop that wasting CPU cycles.

---

**Q7**: Now run `helgrind` on this program. What does it report? Is the code correct?

**A**: It reports a data race between reading from the global variable on the main thread and writing to the global variable on the child thread. It also reports a data race between printf calls on both threads.

```zsh
$ valgrind --tool=helgrind ./main-signal
==23174== Helgrind, a thread error detector
==23174== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==23174== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==23174== Command: ./main-signal
==23174==
this should print first
==23174== ---Thread-Announcement------------------------------------------
==23174==
==23174== Thread #2 was created
==23174==    at 0x518460E: clone (clone.S:71)
==23174==    by 0x4E4BEC4: create_thread (createthread.c:100)
==23174==    by 0x4E4BEC4: pthread_create@@GLIBC_2.2.5 (pthread_create.c:797)
==23174==    by 0x4C38A27: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==23174==    by 0x1087DD: main (main-signal.c:15)
==23174==
==23174== ---Thread-Announcement------------------------------------------
==23174==
==23174== Thread #1 is the program's root thread
==23174==
==23174== ----------------------------------------------------------------
==23174==
==23174== Possible data race during write of size 4 at 0x309014 by thread #2
==23174== Locks held: none
==23174==    at 0x108792: worker (main-signal.c:9)
==23174==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==23174==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==23174==    by 0x518461E: clone (clone.S:95)
==23174==
==23174== This conflicts with a previous read of size 4 by thread #1
==23174== Locks held: none
==23174==    at 0x108802: main (main-signal.c:16)
==23174==  Address 0x309014 is 0 bytes inside data symbol "done"
==23174==
==23174== ----------------------------------------------------------------
==23174==
==23174== Possible data race during read of size 4 at 0x309014 by thread #1
==23174== Locks held: none
==23174==    at 0x108802: main (main-signal.c:16)
==23174==
==23174== This conflicts with a previous write of size 4 by thread #2
==23174== Locks held: none
==23174==    at 0x108792: worker (main-signal.c:9)
==23174==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==23174==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==23174==    by 0x518461E: clone (clone.S:95)
==23174==  Address 0x309014 is 0 bytes inside data symbol "done"
==23174==
==23174== ----------------------------------------------------------------
==23174==
==23174== Possible data race during write of size 1 at 0x5C551A5 by thread #1
==23174== Locks held: none
==23174==    at 0x4C3E546: mempcpy (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==23174==    by 0x50EE933: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1258)
==23174==    by 0x50E3A3E: puts (ioputs.c:40)
==23174==    by 0x108817: main (main-signal.c:18)
==23174==  Address 0x5c551a5 is 21 bytes inside a block of size 1,024 alloc'd
==23174==    at 0x4C32F2F: malloc (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==23174==    by 0x50E113B: _IO_file_doallocate (filedoalloc.c:101)
==23174==    by 0x50F1328: _IO_doallocbuf (genops.c:365)
==23174==    by 0x50F0447: _IO_file_overflow@@GLIBC_2.2.5 (fileops.c:759)
==23174==    by 0x50EE98C: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1266)
==23174==    by 0x50E3A3E: puts (ioputs.c:40)
==23174==    by 0x108791: worker (main-signal.c:8)
==23174==    by 0x4C38C26: ??? (in /usr/lib/valgrind/vgpreload_helgrind-amd64-linux.so)
==23174==    by 0x4E4B6DA: start_thread (pthread_create.c:463)
==23174==    by 0x518461E: clone (clone.S:95)
==23174==  Block was alloc'd by thread #2
==23174==
this should print last
==23174==
==23174== For counts of detected and suppressed errors, rerun with: -v
==23174== Use --history-level=approx or =none to gain increased speed, at
==23174== the cost of reduced accuracy of conflicting-access information
==23174== ERROR SUMMARY: 24 errors from 3 contexts (suppressed: 49 from 49)
```

---

**Q8**: Now look at a slightly modified version of the code, which is found in `main-signal-cv.c`. This version uses a condition variable to do the signaling (and associated lock). Why is this code preferred to the previous version? Is it correctness, or performance, or both?

**A**: The condition variable with a mutex used by the code doesn't have the busy waiting problem. So it is correct with better performance.

---

**Q9**: Once again run `helgrind` on `main-signal-cv`. Does it report any errors?

**A**: No, it detects 0 errors.

```zsh
$ valgrind --tool=helgrind ./main-signal-cv
==24982== Helgrind, a thread error detector
==24982== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==24982== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==24982== Command: ./main-signal-cv
==24982==
this should print first
this should print last
==24982==
==24982== For counts of detected and suppressed errors, rerun with: -v
==24982== Use --history-level=approx or =none to gain increased speed, at
==24982== the cost of reduced accuracy of conflicting-access information
==24982== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 13 from 13)
```

## Part 3: Chapter 28 Simulation

**Q1**: Examine `flag.s`. This code “implements” locking with a single memory flag. Can you understand the assembly?

**A**:

---

**Q2**: When you run with the defaults, does `flag.s` work? Use the `-M` and `-R` flags to trace variables and registers (and turn on `-c` to see their values).  Can you predict what value will end up in `flag`?

**A**:

---

**Q3**: Change the value of the register `%bx` with the `-a` flag (e.g., `-a bx=2,bx=2` if you are running just two threads). What does the code do? How does it change your answer for the question above?

**A**:

---

**Q4**: Set `bx` to a high value for each thread, and then use the `-i` flag to generate different interrupt frequencies; what values lead to a bad outcomes? Which lead to good outcomes?

**A**:

---

**Q5**: Now let’s look at the program `test-and-set.s`. First, try to understand the code, which uses the `xchg` instruction to build a simple locking primitive. How is the lock acquire written? How about lock release?

**A**:

---

**Q6**: Now run the code, changing the value of the interrupt interval (`-i`) again, and making sure to loop for a number of times. Does the code always work as expected? Does it sometimes lead to an inefficient use of the CPU? How could you quantify that?

**A**:

---

**Q7**: Use the `-P` flag to generate specific tests of the locking code. For example, run a schedule that grabs the lock in the first thread, but then tries to acquire it in the second. Does the right thing happen? What else should you test?

**A**:

---

**Q8**: Now let’s look at the code in `peterson.s`, which implements Peterson’s algorithm (mentioned in a sidebar in the text). Study the code and see if you can make sense of it.

**A**:

---

**Q9**: Now run the code with different values of `-i`. What kinds of different behavior do you see? Make sure to set the thread IDs appropriately (using `-a bx=0,bx=1` for example) as the code assumes it.

**A**:

---

**Q10**: Can you control the scheduling (with the `-P` flag) to “prove” that the code works? What are the different cases you should show hold? Think about mutual exclusion and deadlock avoidance.

**A**:

---

**Q11**: Now study the code for the ticket lock in `ticket.s`. Does it match the code in the chapter? Then run with the following flags: `-a bx=1000,bx=1000` (causing each thread to loop through the critical section 1000 times). Watch what happens; do the threads spend much time spin-waiting for the lock?

**A**:

---

**Q12**: How does the code behave as you add more threads?

**A**:

---

**Q13**: Now examine `yield.s`, in which a `yield` instruction enables one thread to yield control of the CPU (realistically, this would be an OS primitive, but for the simplicity, we assume an instruction does the task). Find a scenario where `test-and-set.s` wastes cycles spinning, but `yield.s` does not.  How many instructions are saved? In what scenarios do these savings arise?

**A**:

---

**Q14**： Finally, examine `test-and-test-and-set.s`. What does this lock do? What kind of savings does it introduce as compared to `test-and-set.s`?

**A**:
