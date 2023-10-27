#!/usr/bin/env zsh

# Run script for Homework 6 to call the tlb program

pages=1
while (( pages <= 100000 )); do
    ./tlb -p $pages -t 10000
    pages=$((pages * 2))
done
