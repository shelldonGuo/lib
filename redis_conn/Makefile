GCC=g++

CXXFLAGS=-g \
  -pipe \
  -W \
  -Wall \
  -fPIC


INCFLAG= -I ../hiredis-master/
LDFLAG= ../hiredis-master/libhiredis.a

DEP_INCPATH= -I../../../../../lib2-64/ullib \
  -I../../../../../lib2-64/ullib/include \
  -I../../../../../lib2-64/ullib/output \
  -I../../../../../lib2-64/ullib/output/include 

DEP_LIB= ../../../../../lib2-64/ullib/lib/libullib.a -lpthread 



all: test

test : test.o redis_conn.o
	$(GCC) -o $@ $^ $(LDFLAG) $(DEP_LIB)

%.o : %.cpp
	$(GCC) -c -o $@ $< $(INCFLAG) $(CXXFLAG) $(DEP_INCPATH)

clean :
	rm *.o
	rm test
