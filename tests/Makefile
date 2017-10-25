# Makefile by Michal Trojnara 2017

CFLAGS=-g -O3 -Wall -I ../src
LDLIBS=-lcrypto
MANDATORY_EXE=speed selftest prng streamhash5sum
OPTIONAL_EXE=prng performance

#################### basic targets

all: $(MANDATORY_EXE)

check: selftest
	./selftest

clean:
	rm -f $(EXE) *.o *.dat

$(MANDATORY_EXE) $(OPTIONAL_EXE): ../src/streamhash5.o

../src/streamhash5.o: ../src/streamhash5.c ../src/streamhash5.h ../src/Makefile
	$(MAKE) -C ../src

#################### mandatory targets

speed: speed.o

streamhash5sum: streamhash5sum.o

selftest: selftest.o

#################### optional prng tests

dieharder: prng
	./prng big | dieharder -g 200 -a

ent: prng
	./prng big | dd bs=1k count=1M status=noxfer | ent

prng: prng.o

#################### optional performance plot

performance.svg: performance.plot performance.dat
	gnuplot performance.plot

performance.dat: performance
	stdbuf -o L ./performance | tee performance.dat

performance: performance.o

#################### Makefile ends here
