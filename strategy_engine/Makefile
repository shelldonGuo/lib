GCC = /usr/bin/g++
CPPFLAGS = -g -finline-functions -Wall -Winline -pipe -DOS_LINUX
HEAD_INFO = -DPROJECT_NAME="\"StrategyEngine\"" -DDESCRIPTION="\"strategy_engine\""

THIRDPATH=../../../../../../third-64

INCLUDES = -I./ \
		   -I$(THIRDPATH)/libxml2/include/ 

LIBS     = -L./ \
		   -L$(THIRDPATH)/libxml2/lib/ -lxml2 \
		   -lm -lpthread -lcrypto -ldl

LDFLAGS :=  -Xlinker "-(" \
			$(LIBS) \
			-Xlinker "-)"

all : test strategyA  strategyB
	rm *.o
	@echo "all done"

test : test.o strategy_engine.o plugin_helper.o 
	$(GCC)  $^ -o $@ $(CPPFLAGS) $(HEAD_INFO) $(LDFLAGS) 

strategyA : strategyA.o
	$(GCC)  -shared -fPIC $^ -o strategyA.so

strategyB : strategyB.o
	$(GCC)  -shared -fPIC $^ -o strategyB.so


%.o : %.cpp
	$(GCC) -shared -fPIC $(CPPFLAGS) $(HEAD_INFO) -c $< -o $@ $(INCLUDES)

clean :
	rm -f *.o
	rm -f *.so
	rm test

