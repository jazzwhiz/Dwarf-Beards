CC=g++
CFlags=-c -Wall -O3 -MMD -std=c++0x -pedantic
SDLFlags=-lSDL -lSDL_ttf# -lSDL_image -lSDL_mixer
Includes=-I$(HOME)/Dropbox/bin/cpp/include -Iinclude
Library=-L$(HOME)/Dropbox/bin/cpp/lib
Sources=$(wildcard src/*.cpp)
Test_Sources=$(wildcard test/*.cpp)
Objects=$(addprefix src/obj/,$(notdir $(Sources:.cpp=.o)))
Test_Objects=$(addprefix test/obj/,$(notdir $(Test_Sources:.cpp=.o)))
Executable=dbr
Test_Executable=dbr_test

all: $(Sources) $(Executable) $(Test_Executable)

$(Executable): $(Objects)
	$(CC) $(Objects) $(SDLFlags) $(Library) -lmine -o $@

$(Test_Executable): $(Objects) $(Test_Objects)
	$(CC) $(filter-out src/obj/dbr.o,$(Objects)) $(Test_Objects) $(SDLFlags) $(Library) -lmine -o $@

src/obj/%.o: src/%.cpp include/%.h
	$(CC) $(CFlags) $(Includes) $< -o $@

test/obj/%.o: test/%.cpp
	$(CC) $(CFlags) $(Includes) -Isrc $< -o $@

#-include $(Objects:.o=.d)
#-include $(Test_Objects:.o=.d)

test: $(Test_Executable)
	./dbr_test

clean:
	rm -rf src/obj/* test/obj/* $(Executable) $(Test_Executable)

