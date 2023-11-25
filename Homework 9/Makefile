all: simple_counter approximate_counter hand_over_hand hash_table fork-join rendezvous barrier reader-writer reader-writer-nostarve mutex-nostarve

clean:
	rm simple_counter approximate_counter hand_over_hand hash_table fork-join rendezvous barrier reader-writer reader-writer-nostarve mutex-nostarve

simple_counter: simple_counter.c timer.c timer.h
	gcc -o simple_counter simple_counter.c timer.c -Wall -g -lpthread

approximate_counter: approximate_counter.c timer.c timer.h
	gcc -o approximate_counter approximate_counter.c timer.c -Wall -g -lpthread

hand_over_hand: hand_over_hand.c timer.c timer.h
	gcc -o hand_over_hand hand_over_hand.c timer.c -Wall -g -lpthread

hash_table: hash_table.c timer.c timer.h
	gcc -o hash_table hash_table.c timer.c -Wall -g -lpthread

fork-join: fork-join.c sem.c sem.h
	gcc -o fork-join fork-join.c sem.c -Wall -g -lpthread

rendezvous: rendezvous.c sem.c sem.h
	gcc -o rendezvous rendezvous.c sem.c -Wall -g -lpthread

barrier: barrier.c sem.c sem.h
	gcc -o barrier barrier.c sem.c -Wall -g -lpthread

reader-writer: reader-writer.c sem.c sem.h
	gcc -o reader-writer reader-writer.c sem.c -Wall -g -lpthread

reader-writer-nostarve: reader-writer-nostarve.c sem.c sem.h
	gcc -o reader-writer-nostarve reader-writer-nostarve.c sem.c -Wall -g -lpthread

mutex-nostarve: mutex-nostarve.c sem.c sem.h
	gcc -o mutex-nostarve mutex-nostarve.c sem.c -Wall -g -lpthread
