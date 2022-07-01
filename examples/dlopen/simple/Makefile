main: main.cpp libhello.so
	$(CXX) $(CXXFLAGS) -std=c++11 -o main main.cpp -ldl -pthread
	patchelf --set-rpath '$$ORIGIN' ./main

libhello.so: hello.cpp
	$(CXX) $(CXXFLAGS) -std=c++11 -fPIC -shared -o libhello.so hello.cpp

clean:
	rm -f main libhello.so

.PHONY: clean