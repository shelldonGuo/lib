# make
all:test

test:test.o so
	gcc test.o -rdynamic -L. -ldl -o test

test.o:test.c
	gcc -c test.c -o test.o
so:
	gcc  -fPIC -shared  add.c -o libadd.so
	gcc  -fPIC -shared  min.c -o libmin.so
	cp libadd.so libsthc.so
clean:
	rm *.so
	rm -f *.o test
