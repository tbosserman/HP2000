CFLAGS=-Wall -g

hp2000: main.o getsym.o parse.o symtab.o
	$(CC) -o $@ $^

symtab.o: symtab.c symtab.h

getsym.o: getsym.c symbols.h

parse.o: parse.c parse.h symbols.h

main.o: symbols.h parse.h

clean:
	$(RM) *.o hp2000
