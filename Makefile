CXX=g++
CXXFLAGS=-O3# -Wall -Wextra
SDLFLAGS=-lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

EXEC	= db
SRC     = $(wildcard *.cpp)
OBJ     = $(patsubst %.cpp,%.o,$(SRC))

all: $(SRC) $(EXEC)

-include $(OBJ:.o=.d)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) $(SDLFLAGS) -o $@

# compile and generate dependency info
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $*.o
	$(CXX) -MM $(CXXFLAGS) $*.cpp > $*.d

.phony: clean

clean:
	rm -rf $(OBJ) $(EXEC) *~ *.d
