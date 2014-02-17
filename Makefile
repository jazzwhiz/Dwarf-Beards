CXX=g++
CXXFLAGS=-Wall -O3
AllegroFlags=`allegro-config --libs`

EXEC	= db
SRC     = $(wildcard *.cpp)
OBJ     = $(patsubst %.cpp,%.o,$(SRC))

all: $(SRC) $(EXEC)

-include $(OBJ:.o=.d)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) $(AllegroFlags) -o $@

# compile and generate dependency info
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $*.o
	$(CXX) -MM $(CXXFLAGS) $*.cpp > $*.d

.phony: clean

clean:
	rm -rf $(OBJ) $(EXEC) *~ *.d
