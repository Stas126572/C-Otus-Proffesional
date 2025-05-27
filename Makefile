.PHONY: all clean

all: bin/mapper bin/reducer bin/mapper_quadretic

bin/mapper:
	mkdir -p bin
	g++ -o $@ mapper.cpp

bin/reducer:
	mkdir -p bin
	g++ -o $@ reducer.cpp

bin/mapper_quadretic:
	mkdir -p bin
	g++ -o $@ mapper_quadretic.cpp

clean:
	rm -rf bin output
