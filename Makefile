all:
	clang -std=c++17 -fPIC -shared -O3 -g -o oldlib.so oldlib.cpp

lint:
	@buildifier -mode=check -r .

format:
	@buildifier -mode=fix -r .

clean:
	rm -rf oldlib.so

.PHONY: clean