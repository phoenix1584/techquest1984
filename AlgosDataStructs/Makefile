all:
	make -C src/
	make -C test/

help:
	@cat README 

library:
	make -C src/

tests: library
	make -C test/

clean:
	make -C src/ clean
	make -C test/ clean	
	rm -f bin/*

