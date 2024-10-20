CC=clang
CFLAGS=-g
BINS=async sync 

all: $(BINS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $^ -lrt 

clean:
	rm -rf *.dSYM $(BINS)