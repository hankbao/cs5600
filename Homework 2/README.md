# Homework 2

## Part 1:  Chapter 5 Simulation

**A1**: The process tree can easily be foreseen at each step.

```zsh
$ python fork.py -s 10
ARG seed 10
ARG fork_percentage 0.7
ARG actions 5
ARG action_list
ARG show_tree False
ARG just_final False
ARG leaf_only False
ARG local_reparent False
ARG print_style fancy
ARG solve False

                           Process Tree:
                               a

Action: a forks b
Process Tree?
Action: a forks c
Process Tree?
Action: c EXITS
Process Tree?
Action: a forks d
Process Tree?
Action: a forks e
Process Tree?
```

```zsh
$ python fork.py -s 10 -c
ARG seed 10
ARG fork_percentage 0.7
ARG actions 5
ARG action_list
ARG show_tree False
ARG just_final False
ARG leaf_only False
ARG local_reparent False
ARG print_style fancy
ARG solve True

                           Process Tree:
                               a

Action: a forks b
                               a
                               └── b
Action: a forks c
                               a
                               ├── b
                               └── c
Action: c EXITS
                               a
                               └── b
Action: a forks d
                               a
                               ├── b
                               └── d
Action: a forks e
                               a
                               ├── b
                               ├── d
                               └── e
```

---

**A2**: The higher the `fork_percentage`, the more likely it will form a large process tree. At `0.1` the child process will almost exit right after spawning.

---

**A3**: Given the following process tree,

```zsh
$ python fork.py -t
ARG seed -1
ARG fork_percentage 0.7
ARG actions 5
ARG action_list
ARG show_tree True
ARG just_final False
ARG leaf_only False
ARG local_reparent False
ARG print_style fancy
ARG solve False

                           Process Tree:
                               a

Action?
                               a
                               └── b
Action?
                               a
Action?
                               a
                               └── c
Action?
                               a
                               ├── c
                               └── d
Action?
                               a
                               ├── c
                               │   └── e
                               └── d
```

the actions could be:

```zsh
a forks b
b EXITS
a forks c
a forks d
c forks e
```

---

**A4**: Without `-R`, `d` and `e` become children of `a` after `c` exited.

```zsh
$ python fork.py -A a+b,b+c,c+d,c+e,c- -t

                           Process Tree:
                               a

Action?
                               a
                               └── b
Action?
                               a
                               └── b
                                   └── c
Action?
                               a
                               └── b
                                   └── c
                                       └── d
Action?
                               a
                               └── b
                                   └── c
                                       ├── d
                                       └── e
Action?
                               a
                               ├── b
                               ├── d
                               └── e
```

With `-R`, `d` and `e` become children of process `b` after `c` exited.

```zsh
$ python fork.py -A a+b,b+c,c+d,c+e,c- -R -t

                           Process Tree:
                               a

Action?
                               a
                               └── b
Action?
                               a
                               └── b
                                   └── c
Action?
                               a
                               └── b
                                   └── c
                                       └── d
Action?
                               a
                               └── b
                                   └── c
                                       ├── d
                                       └── e
Action?
                               a
                               └── b
                                   ├── d
                                   └── e
```

---

**A5**: Given the following actions,

```zsh
$ python fork.py -F
ARG seed -1
ARG fork_percentage 0.7
ARG actions 5
ARG action_list
ARG show_tree False
ARG just_final True
ARG leaf_only False
ARG local_reparent False
ARG print_style fancy
ARG solve False

                           Process Tree:
                               a

Action: a forks b
Action: b forks c
Action: b forks d
Action: b EXITS
Action: d forks e

                        Final Process Tree?
```

The final process tree could be:

```zsh
            a
            ├── c
            └── d
                └── e
``````

---

**A6**: Given the following process tree,

```zsh
python fork.py -F -t
                           Process Tree:
                               a

Action?
Action?
Action?
Action?
Action?

                        Final Process Tree:
                               a
                               ├── c
                               └── d
                                   └── e
```

The actions that took place could be:

```zsh
Action: a forks b
Action: b forks c
Action: b forks d
Action: d forks e
Action: b EXITS
```

## Part 2:  Chapter 5 Code

**Q1**: What value is the variable in the child process? What happens to the variable when both the child and parent change the value of x?

**A**: The variable in the child process has the same value as in the parent process. Both processes can update the value of x, because the memory of x is on the stack and each process has its own stack. Please see the code in [ch5c1.c](./ch5c1.c).

---

**Q2**: Can both the child and parent access the file descriptor returned by `open()`? What happens when they are writing to the file concurrently, i.e., at the same time?

**A**: Both the child and parent can access the file descriptor returned by `open()`. When they are writing to the file concurrently, the output could be mixed up. Please see the code in [ch5c2.c](./ch5c2.c).

---

**Q3**: Can you do this without calling `wait()` in the parent?

**A**: Yes, I can use `waitpid()` to wait for the specific child process. Please see the code in [ch5c3.c](./ch5c3.c).

---

**Q4**: Why do you think there are so many variants of the same basic call?

**A**: The `exec()` family of functions provides different functionalities. For example, `execl()` takes a list of arguments, while `execv()` takes an array of arguments. This helps developer to call them in their preferred way. Also some of them duplicate the actions of the shell in searching for an executable file. Others let developers specify environment of the executed program. Please see the code in [ch5c4.c](./ch5c4.c).

---

**Q5**: What does `wait()` return? What happens if you use `wait()` in the child?

**A**: If `wait()` returns due to a stopped or terminated child process, the process ID of the child is returned to the calling process. Otherwise, a value of -1 is returned and errno is set to indicate the error. The `wait()` called in the child returned -1 as it doesn't have a child process. Please see the code in [ch5c5.c](./ch5c5.c).

---

**Q6**: When would `waitpid()` be useful?

**A**: It would be handy if you just want to wait on a specific child process instead of any child process. Please see the code in [ch5c6.c](./ch5c6.c).

---

**Q7**: What happens if the child calls `printf()` to print some output after closing the descriptor?

**A**: Nothing being printed out. Please see the code in [ch5c7.c](./ch5c7.c).

---

**Q8**: Write a program that creates two children, and connects the standard output of one to the standard input of the other, using the
`pipe()` system call.

**A**: Please see the code in [ch5c8.c](./ch5c8.c).

## Part 3: Chapter 6 Measurement

## Part 4: Chapter 7 Simulation

**Q1**: Compute the response time and turnaround time when running
three jobs of length 200 with the SJF and FIFO schedulers

**A**:

---

**Q2**: Now do the same but with jobs of different lengths: 100, 200, and 300.

**A**:

---

**Q3**: Now do the same, but also with the RR scheduler and a time-slice of 1.

**A**:

---

**Q4**: For what types of workloads does SJF deliver the same turnaround times as FIFO?

**A**:

---

**Q5**: For what types of workloads and quantum lengths does SJF deliver the same response times as RR?

**A**:

---

**Q6**: What happens to response time with SJF as job lengths increase?  Can you use the simulator to demonstrate the trend?

**A**:

---

**Q7**: What happens to response time with RR as quantum lengths increase? Can you write an equation that gives the worst-case response time, given N jobs?

**A**:
