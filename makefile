CXX = g++
CXXFLAGS = -Wall -g -lncurses -lsane -std=c++17
all: _init bin/main
dbg: _init bin/main
	gdb bin/main
_remote: _deploy
	ssh iggyvolz@192.168.0.19 "cd PrintScan && make _local"
run:
	bin/main
_init:
	mkdir -p bin
clean:
	rm -Rf bin
_deploy: clean
	scp -rp * iggyvolz@192.168.0.19:~/PrintScan
bin/main: $(shell for file in *.cpp;do echo $$file|sed 's/cpp/o/'|awk '{ print "bin/" $$0 }'; done|xargs echo)
	$(CXX) -o bin/main $^ $(CXXFLAGS)
bin/%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)