CXX = g++
CXXFLAGS = -Wall -g -lncurses -lsane -std=c++17
all: init bin/main
dbg: init bin/main
	gdb bin/main
run:
	bin/main
init:
	mkdir -p bin
clean:
	rm -Rf bin
deploy: clean
	scp -rp * remote:~/PrintScan
bin/main: $(shell for file in *.cpp;do echo $$file|sed 's/cpp/o/'|awk '{ print "bin/" $$0 }'; done|xargs echo)
	$(CXX) -o bin/main $^ $(CXXFLAGS)
# $(shell echo $@|tail -c +5|sed 's/o$$/cpp/'|xargs cat|grep '#include'|grep '"'|cut -d '"' -f2)
bin/%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)
