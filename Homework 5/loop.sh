#!zsh

for i in {0..1024}
do
    python relocation.py -s 0 -l $i -c | grep "VALID:" | wc -l
done
