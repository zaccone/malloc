all: clean build
clean:
	rm -rf *.so
build:
	gcc -O0 -g -W -Wall -Wextra -shared -fPIC malloc.c -o malloc.so

