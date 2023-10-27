# Homework 6

## Part 1: Chapter 18 Simulation

**Q1**: Before doing any translations, let’s use the simulator to study how
linear page tables change size given different parameters. Compute
the size of linear page tables as different parameters change. Some
suggested inputs are below; by using the `-v` flag, you can see
how many page-table entries are filled. First, to understand how
linear page table size changes as the address space grows, run with
these flags:

```zsh
-P 1k -a 1m -p 512m -v -n 0
-P 1k -a 2m -p 512m -v -n 0
-P 1k -a 4m -p 512m -v -n 0
```

Then, to understand how linear page table size changes as page size
grows:

```zsh
-P 1k -a 1m -p 512m -v -n 0
-P 2k -a 1m -p 512m -v -n 0
-P 4k -a 1m -p 512m -v -n 0
```

Before running any of these, try to think about the expected trends.
How should page-table size change as the address space grows? As
the page size grows? Why not use big pages in general?

**A**: Because big pages are not flexible enough. If the page size is too big, there could be only a few users get a page before it runs out of space.

---

**Q2**: Now let’s do some translations. Start with some small examples,
and change the number of pages that are allocated to the address
space with the `-u` flag. For example:

```zsh
-P 1k -a 16k -p 32k -v -u 0
-P 1k -a 16k -p 32k -v -u 25
-P 1k -a 16k -p 32k -v -u 50
-P 1k -a 16k -p 32k -v -u 75
-P 1k -a 16k -p 32k -v -u 100
```

What happens as you increase the percentage of pages that are allocated in each address space?

**A**: The number of invalid pages decreases.

---

**Q3**: Now let’s try some different random seeds, and some different (and
sometimes quite crazy) address-space parameters, for variety:

```zsh
-P 8 -a 32 -p 1024 -v -s 1
-P 8k -a 32k -p 1m -v -s 2
-P 1m -a 256m -p 512m -v -s 3
```

Which of these parameter combinations are unrealistic? Why?

**A** The third one as the address space is too large comparing to the physical memory.

---

**Q4**: Use the program to try out some other problems. Can you find the
limits of where the program doesn’t work anymore? For example,
what happens if the address-space size is bigger than physical memory?

**A**: It won't work as the physical memory size must be GREATER than address space size.

## Part 2: Chapter 19 Measurement