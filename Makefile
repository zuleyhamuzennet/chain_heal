CC = gcc
CFLAGS = -o
LIBS = -lm

chain_heal: chain_heal.c
	$(CC) $(CFLAGS) chain_heal chain_heal.c $(LIBS)

.PHONY: clean

clean:
	rm -rf chain_heal
