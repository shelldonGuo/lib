####################Makefile####################
CXX=g++
CXXFLAGS=-g \
  -pipe \
  -W \
  -Wall \
  -fPIC
CFLAGS=-g \
  -pipe \
  -W \
  -Wall \
  -fPIC
INCPATH=-I.
DEP_INCPATH= -I../../../../../third-64/mysql \
  -I../../../../../third-64/mysql/include \
  -I../../../../../third-64/mysql/output \
  -I../../../../../third-64/mysql/output/include \
  -I../../../../../lib2-64/ullib \
  -I../../../../../lib2-64/ullib/include \
  -I../../../../../lib2-64/ullib/output \
  -I../../../../../lib2-64/ullib/output/include 


.PHONY:all
all:test
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mall[0m']"
	@echo "make all done"

.PHONY:test
test: test.o mysql_conn.o
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mtest[0m']"
	$(CXX) $(CXXFLAGS)  -o test test.o mysql_conn.o -lpthread -lz \
	../../../../../lib2-64/ullib/lib/libullib.a \
	../../../../../third-64/mysql/lib/libdbug.a \
	../../../../../third-64/mysql/lib/libheap.a \
	../../../../../third-64/mysql/lib/libmyisam.a \
	../../../../../third-64/mysql/lib/libmyisammrg.a \
	../../../../../third-64/mysql/lib/libmysqlclient.a \
	../../../../../third-64/mysql/lib/libmysqlclient_r.a \
	../../../../../third-64/mysql/lib/libmystrings.a \
	../../../../../third-64/mysql/lib/libmysys.a \
	../../../../../third-64/mysql/lib/libvio.a

.PHONY:clean
clean:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mclean[0m']"
	rm -rf *.o

mysql_conn.o:mysql_conn.cpp mysql_conn.h
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mmysql_conn.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CXXFLAGS)  -o mysql_conn.o mysql_conn.cpp

test.o:test.cpp
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mtest.o[0m']"
	$(CXX) -c $(INCPATH) $(DEP_INCPATH) $(CXXFLAGS)  -o test.o test.cpp
