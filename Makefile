
OBJECTS = memsim.o rep_alg_default.o rep_alg_rand.o rep_alg_lru.o rep_alg_esc.o rep_alg_clock.o
CC = gcc -g

memsim: $(OBJECTS)
	$(CC) -o memsim $(OBJECTS)

memsim.o:
	$(CC) -c memsim.c

rep_alg_default.o: rep_alg_default.h
	$(CC) -c rep_alg_default.c

rep_alg_rand.o: rep_alg_default.h
	$(CC) -c rep_alg_rand.c

rep_alg_lru.o: rep_alg_default.h
	$(CC) -c rep_alg_lru.c

rep_alg_clock.o: rep_alg_default.h
	$(CC) -c rep_alg_clock.c

rep_alg_esc.o: rep_alg_default.h
	$(CC) -c rep_alg_esc.c

clean:
	rm -f $(OBJECTS)

