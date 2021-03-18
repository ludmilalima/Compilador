# Makefile for C- compiler

CC = gcc
BISON = bison
LEX = flex

BIN = compiler

OBJS = parser.tab.o lex.yy.o main.o util.o symboltable.o bincode.o analyze.o codegenerate.o assemblygenerate.o

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $(BIN)

main.o: main.c globals.h util.h scan.h analyze.h codegenerate.h assemblygenerate.h bincode.h
	$(CC) -g -c main.c

util.o: util.c util.h globals.h assemblygenerate.h bincode.h
	$(CC) -g -c util.c

bincode.o: bincode.c codegenerate.h assemblygenerate.h bincode.h util.h globals.h
	$(CC) -g -c bincode.c

symtable.o: symboltable.c symboltable.h bincode.h
	$(CC) -g -c symboltable.c

analyze.o: analyze.c globals.h symboltable.h analyze.h bincode.h
	$(CC) -g -c analyze.c

codegenerate.o: codegenerate.c globals.h symboltable.h util.h assemblygenerate.h codegenerate.h bincode.h
	$(CC) -g -c codegenerate.c

assemblygenerate.o: assemblygenerate.c globals.h util.h codegenerate.h assemblygenerate.h bincode.h
	$(CC) -g -c assemblygenerate.c

lex.yy.o: scanner.l scan.h util.h globals.h
	$(LEX) -o lex.yy.c scanner.l
	$(CC) -c lex.yy.c

parser.tab.o: parser.y globals.h
	$(BISON) -d parser.y
	$(CC) -c parser.tab.c

clean:
	-rm -f $(BIN)
	-rm -f parser.tab.c
	-rm -f parser.tab.h
	-rm -f lex.yy.c
	-rm -f $(OBJS)