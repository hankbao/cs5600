# Homework 11

## Part 1: Final VMM presentation

Please see our final VMM presentation [here](https://docs.google.com/presentation/d/1cqL5pHquOcsk4v-Pn6lAISUzBKTm0BOb61Xs_q1OGuA/edit?usp=sharing).

## Part 2: Project update

### Progress

- [x] study `userfaultfd` API
- [x] study `userfaultfd` Rust [crate](https://crates.io/crates/userfaultfd)
- [ ] design user-space paging program with `userfaultfd`
- [ ] program implementation
- [ ] test
- [ ] report

### Plan

1. Finish the in-progress designing of the paging program
2. Finish the implementation of the paging program
3. Test the implementation with different payloads to see its paging behavior
4. Write a report on the implementation and testing results

### Future Work

1. Explore the way of using `userfaultfd` to do lazy paging.
2. Explore the way of using `userfaultfd` to do efficient memory copying and zeroing
