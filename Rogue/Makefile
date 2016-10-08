#CXX=g++
CFlags=-c -Wall -Werror -O3 -MMD -std=c++0x -pedantic
SDLFlags=-lSDL -lSDL_ttf# -lSDL_image -lSDL_mixer
Includes=-Iinclude
Sources=$(wildcard src/*.cpp)
Test_Sources=$(wildcard test/*.cpp)
Objects=$(addprefix src/obj/,$(notdir $(Sources:.cpp=.o)))
Test_Objects=$(addprefix test/obj/,$(notdir $(Test_Sources:.cpp=.o)))
Executable=dbr
Test_Executable=dbr_test

all: $(Sources) $(Executable) $(Test_Executable)

$(Executable): $(Objects)
	$(CXX) $(Objects) $(SDLFlags) $(Library) -o $@

$(Test_Executable): $(Objects) $(Test_Objects)
	$(CXX) $(filter-out src/obj/dbr.o,$(Objects)) $(Test_Objects) $(SDLFlags) $(Library) -o $@

src/obj/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFlags) $(Includes) $< -o $@

test/obj/%.o: test/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFlags) $(Includes) -Isrc $< -o $@

#-include $(Objects:.o=.d)
#-include $(Test_Objects:.o=.d)

test: $(Test_Executable)
	./dbr_test

clean:
	rm -rf src/obj/* test/obj/* $(Executable) $(Test_Executable)

