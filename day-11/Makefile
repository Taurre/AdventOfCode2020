PART_1 = common.o part-1.o
PART_2 = common.o part-2.o

all: part-1 part-2

part-1: $(PART_1)
	$(CC) $(PART_1) $(LDFLAGS) -o $@

part-2: $(PART_2)
	$(CC) $(PART_2) $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -rf *.o part-1 part-2
