#makefile

GCC=g++

all : split
	rm *.o

split: split.o
	$(GCC) -o $@ $^

%.o : %.cpp
	$(GCC) -c $< -o $@
