CXX=g++
CFlags=-Wall -O3
AllegroFlags=`allegro-config --libs`

EXEC	= db
SRC     = $(wildcard *.cpp)
OBJ     = $(patsubst %.cpp,%.o,$(SRC))
DEP     = $(patsubst %.cpp,%.d,$(SRC))

all: $(DEP) $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) $(CFlags) $(AllegroFlags) -o $@

%.d:
	$(CXX) -MM $(CFlags) $(AllegroFlags) $(SRC) > $@

.cpp.o:
	$(CXX) -c $(CFlags) $(AllegroFlags) $< -o $@

clean:
	rm -rf $(OBJ) $(EXEC) *~ *.d

-include $(DEP)
