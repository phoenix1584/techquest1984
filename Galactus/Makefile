.PHONY: all src test

all: src test

src: 
	+make -C src/

test:
	+make -C test/

src-clean:
	+make -C src/ clean

test-clean:
	+make -C test/ clean

clean: src-clean test-clean
