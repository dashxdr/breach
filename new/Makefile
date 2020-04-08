CFLAGS = -O2
all: breach.bin

breach.bin: makerom b.a list
	asm b #--l
	./makerom  list breach.bin

clean:
	rm -f makerom b #breach.bin
