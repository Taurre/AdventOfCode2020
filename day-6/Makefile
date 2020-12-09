OBJ = main.o common.o

all: part-1 part-2

part-1: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

part-2: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

clean:
	rm -f *.o part-1 part-2
