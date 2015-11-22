#ifndef Draw_H
#define Draw_H

#include "SDL/SDL.h"
//#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
//#include "SDL/SDL_mixer.h"

#include <vector>
#include <string>

class World;
class Building_Base;

namespace draw
{
// screen
extern const int framerate;
extern const int screen_size[2];
extern SDL_Surface* screen;

// colors
extern const SDL_Color YELLOW;

// fonts
extern const int num_fonts;
extern const int font_sizes[];
extern TTF_Font* fonts[];

// finished products
int title(World* w);
int dwarf_profile(World* w);
int earth(World* w);
void battle(World* w, int attack_style, int attack_target, std::vector<std::string> readout); // subset of dwarf_battle
void help(); // help screen 
int inside_building(World* w, Building_Base* building); // buildings

// subsets
void progress_bar(double progress, int x, int y, int width, int alignx); // progress in [0,1]

// text and surfaces
int text(const std::string msg, int size, SDL_Color color, int x, int y, int alignx); // returns the width
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination);
void clear_screen();

// large scale structure
void init(World* w);
void clean_up();

} // namespace draw

#endif
