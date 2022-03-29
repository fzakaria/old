all:
	clang -std=c++17 -fPIC -shared -O3 -g -o oldlib.so oldlib.cpp


clean:
	rm -rf oldlib.so

.PHONY: clean