all: clean build
clean:
	rm -rf *.so
build:
	gcc -O0 -g -W -Wall -Wextra -shared -fPIC malloc.c -o malloc.so
test: all
	gcc -g -std=c11 test.c -o test
	LD_PRELOAD=./malloc.so ./test

