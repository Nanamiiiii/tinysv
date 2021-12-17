# Makefile
MAKE = make

.PHONY: all
all: 
	$(MAKE) tinysv
	$(MAKE) modules

.PHONY: tinysv
tinysv: src/Makefile
	$(MAKE) -C src

.PHONY: modules
modules: modules/Makefile
	$(MAKE) -C modules

.PHONY: tests
tests: test/Makefile
	$(MAKE) -C test

.PHONY: clean
clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean
	$(MAKE) -C modules clean
