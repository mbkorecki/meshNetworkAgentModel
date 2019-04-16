CC=g++-8
CCFLAGS= -std=c++17

SRCS = main.cc $(wildcard ./world/*.cc) $(wildcard ./agent/*.cc)

OBJS = $(SRCS:.cc=.o)
MAIN = app

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CCFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
.cc.o:
	$(CC) $(CCFLAGS) $(INCLUDES) -c $<  -o $@

.PHONY: depend clean
clean:
	rm -f $(MAIN) $(OBJS)

depend: $(SRCS)
	makedepend $(INCLUDES) $^
