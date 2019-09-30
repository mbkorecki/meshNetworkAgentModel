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

.PHONY: depend clean all-results

clean:
	rm -f $(MAIN) $(OBJS)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

results:
	mkdir -p results

results/setup1results.txt: app results
	./app 1  80  50 50  10  100  0.01 > $@

results/setup2results.txt: app results
	./app 2  80  50 50  10  100  0.01 > $@

results/setup3BBresults.txt: app results
	echo "TODO"

results/setup3FFAresults.txt: app results
	echo "TODO"

results/setup3HybridResults.txt: app results
	echo "TODO"

results/setup3TFTResults.txt: app results
	echo "TODO"

results/setup3TournamentResults.txt: app results
	echo "TODO"

all-results: results/setup1results.txt results/setup2results.txt results/setup3BBresults.txt results/setup3FFAresults.txt results/setup3HybridResults.txt results/setup3TFTResults.txt results/setup3TournamentResults.txt
