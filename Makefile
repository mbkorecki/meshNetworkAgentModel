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

.PHONY: depend clean all-results clean-results

clean:
	rm -f $(MAIN) $(OBJS)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

results:
	mkdir -p results

results/setup1results.txt: app results
	./app 1  80  50 50  10  100  0.01  >  $@

results/setup2results.txt: app results
	./app 2  80  50 50  10  100  0.01  >  $@

results/setup3setting1TFTresults.txt: app results
	./app 3  80  50 50  10  100  0.01 0.5 0 0  >  $@

results/setup3setting2BBresults.txt: app results
	./app 3  80  50 50  10  100  0.01 0 0.5 0  >  $@

results/setup3setting3HybridResults.txt: app results
	./app 3  80  50 50  10  100  0.01 0 0 0.5  >  $@

results/setup3setting4TournamentResults.txt: app results
	./app 3  80  50 50  10  100  0.01 0.25 0.25 0.25  >  $@

all-results: results/setup1results.txt results/setup2results.txt results/setup3setting1TFTresults.txt  results/setup3setting3HybridResults.txt results/setup3setting2BBresults.txt results/setup3setting4TournamentResults.txt

clean-results:
	rm -rf results
