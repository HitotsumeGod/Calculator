CC=gcc
SRC=src/main
DEPS=src/headers
SRS=$(SRC)/Calculator.c

Calc: $(SRS)
	$(CC) -o $@ $^ 
Calc.s: $(SRS)
	$(CC) -S -o $@ $^ -masm=intel -fno-asynchronous-unwind-tables
clean: Calc
	rm -f $^
