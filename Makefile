CXX = g++
CXXFLAGS = -I. -I/usr/local/include -Wall -O2
LDFLAGS = -L/usr/local/lib -lboost_serialization-xgcc40-mt
TARGET = subgraph_serialize_test

all: $(TARGET)

cc.o:
	$(CXX) $< -c $(CXXFLAGS) 

$(TARGET): subgraph_serialize_test.o
	$(CXX) $< -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf *.o $(TARGET)
