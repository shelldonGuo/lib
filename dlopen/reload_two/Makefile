# makefile
all: test so
	rm *.o
	@echo done

test: test.o
	g++ -rdynamic -L. -ldl -o test test.o

test.o: test.cpp
	g++ -c -o test.o test.cpp
so:
	g++ -shared -fPIC foo1.cpp -o libfoo1.so
	g++ -shared -fPIC foo2.cpp -o libfoo2.so

clean:
	rm test
	rm *.so
