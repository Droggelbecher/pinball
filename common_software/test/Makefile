
.PHONY: run_test_checksum

all: run_test_checksum

test_checksum: test_checksum.c ../include/checksum.h ../src/checksum.c
	gcc -g ./test_checksum.c ../src/checksum.c -otest_checksum -I../include


run_test_checksum: test_checksum
	./test_checksum

