# Homework 10

## Part 1: Chapter 32 Code

**Q1**: First let’s make sure you understand how the programs generally work, and some of the key options. Study the code in `vector-deadlock.c`, as well as in `main-common.c` and related files.

Now, run `./vector-deadlock -n 2 -l 1 -v`, which instantiates two threads (`-n 2`), each of which does one vector add (`-l 1`), and does so in verbose mode (`-v`). Make sure you understand the output. How does the output change from run to run?

**A**: The output is the same for each run.

```zsh
$ ./vector-deadlock -n 2 -l 1 -v
->add(0, 1)
<-add(0, 1)
              ->add(0, 1)
              <-add(0, 1)
```

---

**Q2**: Now add the `-d` flag, and change the number of loops (`-l`) from 1 to higher numbers. What happens? Does the code (always) deadlock?

**A**: More output generated. I tried different `-l` numbers and it deadlocks when `-l` is 100000.

---

**Q3**: How does changing the number of threads (`-n`) change the outcome of the program? Are there any values of `-n` that ensure no deadlock occurs?

**A**: It generates more columns of output as the number of threads grows. I tried different `-n` numbers and it will not deadlock when `-n` is 1.

---

**Q4**: Now examine the code in `vector-global-order.c`. First, make sure you understand what the code is trying to do; do you understand why the code avoids deadlock? Also, why is there a special case in this `vector_add()` routine when the source and destination vectors are the same?

**A**: Because it locks the mutexes in order by comparing the addresses of the source and destination vectors. If the source and destination vectors are the same, it will try to lock the same mutex twice. If the mutex is not reentrant, it will deadlock.

---

**Q5**: Now run the code with the following flags: `-t -n 2 -l 100000 -d`.  How long does the code take to complete? How does the total time change when you increase the number of loops, or the number of threads?

**A**: The time grows as the number of loops or the number of threads increases. However, the number of threads impact the total time more.

```zsh
$ ./vector-global-order -t -n 2 -l 100000 -d
Time: 0.02 seconds

$ ./vector-global-order -t -n 2 -l 1000000 -d
Time: 0.13 seconds

$ ./vector-global-order -t -n 20 -l 100000 -d
Time: 0.76 seconds
```

---

**Q6**: What happens if you turn on the parallelism flag (`-p`)? How much would you expect performance to change when each thread is working on adding different vectors (which is what `-p` enables) versus working on the same ones?

**A**: Turning on the parallelism flag will significantly improve the performance. When each thread is working on adding different vectors, the performance will be better than working on the same ones.

```zsh
$ ./vector-global-order -t -n 20 -l 100000 -d -p
Time: 0.03 seconds

$ ./vector-global-order -t -n 2 -l 1000000 -d -p
Time: 0.08 seconds
```

---

**Q7**: Now let’s study `vector-try-wait.c`. First make sure you understand the code. Is the first call to `pthread_mutex_trylock()` really needed?  Now run the code. How fast does it run compared to the global order approach? How does the number of retries, as counted by the code, change as the number of threads increases?

**A**: Yes, the first call to `pthread_mutex_trylock()` is needed to protect modifying the global variable `retry`. It's way more slower than the global order approach with same arguments. The number of retries grows as the number of threads increases.

```zsh
$ ./vector-try-wait -t -n 2 -l 100000 -d
Retries: 1185301
Time: 0.12 seconds

$ ./vector-try-wait -t -n 2 -l 1000000 -d
Retries: 7872643
Time: 0.77 seconds

$ ./vector-try-wait -t -n 20 -l 100000 -d
Retries: 688647198
Time: 177.72 seconds
```

---

**Q8**: Now let’s look at `vector-avoid-hold-and-wait.c`. What is the main problem with this approach? How does its performance compare to the other versions, when running both with `-p` and without it?

**A**: It decreases concurrency as all locks must be acquired early on instead of when they are truly needed.

```zsh
$ ./vector-avoid-hold-and-wait -t -n 2 -l 100000 -d
Time: 0.03 seconds

$ ./vector-avoid-hold-and-wait -t -n 2 -l 1000000 -d
Time: 0.20 seconds

$ ./vector-avoid-hold-and-wait -t -n 20 -l 100000 -d
Time: 1.22 seconds
```

As with `-p`:

```zsh
$ ./vector-avoid-hold-and-wait -t -n 2 -l 100000 -d -p
Time: 0.02 seconds

$ ./vector-avoid-hold-and-wait -t -n 2 -l 1000000 -d -p
Time: 0.21 seconds

$ ./vector-avoid-hold-and-wait -t -n 20 -l 100000 -d -p
Time: 0.25 seconds
```

---

**Q9**: Finally, let’s look at `vector-nolock.c`. This version doesn’t use locks at all; does it provide the exact same semantics as the other versions? Why or why not?

**A**: Yes, it uses locking version of x86 fetch-and-add instruction to make the adding in `vector_add` atomic. A problem with the `vector-nolock.c` is that its inline assembly code doesn't compile on macOS with clang. I managed to compile it on a Linux box.

```zsh
$ make vector-nolock
gcc -o vector-nolock vector-nolock.c -Wall -pthread -O
vector-nolock.c:15:5: error: invalid output constraint '=a' in asm
                 :"=a" (value)
                  ^
1 error generated.
make: *** [vector-nolock] Error 1
```

---

**Q10**: Now compare its performance to the other versions, both when threads are working on the same two vectors (no `-p`) and when each thread is working on separate vectors (`-p`). How does this no-lock version perform?

**A**: The no-lock version has better performance when there are more threads working with or without `-p`.

```zsh
$ ./vector-nolock -t -n 2 -l 100000 -d
Time: 0.15 seconds

$ ./vector-nolock -t -n 2 -l 1000000 -d
Time: 1.44 seconds

$ ./vector-nolock -t -n 20 -l 100000 -d
Time: 1.38 seconds
```

As with `-p`:

```zsh
$ ./vector-nolock -t -n 2 -l 100000 -d -p
Time: 0.15 seconds

$ ./vector-nolock -t -n 2 -l 1000000 -d -p
Time: 1.37 seconds

$ ./vector-nolock -t -n 20 -l 100000 -d -p
Time: 1.38 seconds
```

## Part 2: Chapter 33 Code

**Q1**: First, write a simple server that can accept and serve TCP connections. You’ll have to poke around the Internet a bit if you don’t already know how to do this. Build this to serve exactly one request at a time; have each request be very simple, e.g., to get the current time of day.
**Q2**: Now, add the `select()` interface. Build a main program that can accept multiple connections, and an event loop that checks which file descriptors have data on them, and then read and process those requests. Make sure to carefully test that you are using `select()` correctly.
**Q3**: Next, let’s make the requests a little more interesting, to mimic a simple web or file server. Each request should be to read the contents of a file (named in the request), and the server should respond by reading the file into a buffer, and then returning the contents to the client. Use the standard `open()`, `read()`, `close()` system calls to implement this feature. Be a little careful here: if you leave this running for a long time, someone may figure out how to use it to read all the files on your computer!
**Q4**: Now, instead of using standard I/O system calls, use the asynchronous I/O interfaces as described in the chapter. How hard was it to incorporate asynchronous interfaces into your program?
**Q5**: For fun, add some signal handling to your code. One common use of signals is to poke a server to reload some kind of configuration file, or take some other kind of administrative action. Perhaps one natural way to play around with this is to add a user-level file cache to your server, which stores recently accessed files. Implement a signal handler that clears the cache when the signal is sent to the server process.
**Q6**: Finally, we have the hard part: how can you tell if the effort to build an asynchronous, event-based approach are worth it? Can you create an experiment to show the benefits? How much implementation complexity did your approach add?

**A**: Please see the code in [tcp_server.c](./tcp_server.c) for the final version.
