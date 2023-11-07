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

**Q1**:

**A**:

---

