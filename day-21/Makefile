LDFLAGS = -lfl
OBJ = main.o lexer.o ingredient.o allergen.o line.o

main: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -f *.o main
