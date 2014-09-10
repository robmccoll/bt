
.PHONY: all
all: bt

bt: bt.c
	$(CC) -O3 $^ -o $@ 

.PHONY: clean
clean:
	rm bt
