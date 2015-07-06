CC=g++
CFlags=-c -Wall -O3 -MMD -std=c++0x
SDLFlags=-lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
Includes=-I$(HOME)/Dropbox/bin/cpp/include
Library=-L$(HOME)/Dropbox/bin/cpp/lib
Sources=$(wildcard src/*.cpp)
Objects=$(addprefix obj/,$(notdir $(Sources:.cpp=.o)))
Executable=dbr

all: $(Sources) $(Executable)

$(Executable): $(Objects)
	$(CC) $(Objects) $(SDLFlags) $(Library) -lmine -o $@

obj/%.o: src/%.cpp
	$(CC) $(CFlags) $(Includes) $< -o $@

-include $(Objects:.o=.d)

clean:
	rm -rf obj/*.o obj/*.d $(Executable)

