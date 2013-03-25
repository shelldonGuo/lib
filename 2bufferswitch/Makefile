GCC = /usr/bin/g++
CPPFLAGS = -g -finline-functions -Wall -Winline -pipe -DOS_LINUX

INCLUDES = -I./

all : test 
	rm *.o
	@echo "all done"

test : test.o Resource.o ResourceTracker.o
	$(GCC)  $^ -o $@ $(CPPFLAGS)

%.o : %.cpp
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDES)

clean :
	rm test


