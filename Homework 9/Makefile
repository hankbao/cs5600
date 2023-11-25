all: simple_counter approximate_counter hand_over_hand hash_table

clean:
	rm simple_counter approximate_counter hand_over_hand hash_table

simple_counter: simple_counter.c timer.c timer.h
	gcc -o simple_counter simple_counter.c timer.c -Wall -g -lpthread

approximate_counter: approximate_counter.c timer.c timer.h
	gcc -o approximate_counter approximate_counter.c timer.c -Wall -g -lpthread

hand_over_hand: hand_over_hand.c timer.c timer.h
	gcc -o hand_over_hand hand_over_hand.c timer.c -Wall -g -lpthread

hash_table: hash_table.c timer.c timer.h
	gcc -o hash_table hash_table.c timer.c -Wall -g -lpthread
