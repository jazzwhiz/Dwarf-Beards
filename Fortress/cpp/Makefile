CC=g++
CFlags=-c -Wall -O3 -MMD -std=c++0x
SDLFlags=-lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
Sources=$(wildcard src/*.cpp)
Objects=$(addprefix obj/,$(notdir $(Sources:.cpp=.o)))
Executable=dbf

all: $(Sources) $(Executable)

$(Executable): $(Objects)
	$(CC) $(Objects) $(SDLFlags) -o $@

obj/%.o: src/%.cpp
	$(CC) $(CFlags) $< -o $@

-include $(Objects:.o=.d)

clean:
	rm -rf obj/*.o obj/*.d $(Executable)

