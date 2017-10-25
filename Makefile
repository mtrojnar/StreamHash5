# Makefile by Michal Trojnara 2017

all:
	$(MAKE) -C src
	$(MAKE) -C tests

clean:
	$(MAKE) -C src clean
	$(MAKE) -C tests clean
