#ifndef Draw_H
#define Draw_H

#include "SDL/SDL.h"
//#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
//#include "SDL/SDL_mixer.h"

class World;

namespace draw
{
// colors
extern const SDL_Color YELLOW;

// fonts
extern const int num_fonts;
extern const int font_sizes[];
extern TTF_Font* fonts[];

// finished products
bool title(World* w);
int dwarf_profile(World* w);
int earth(World* w);
void battle(World* w, int attack_style, int attack_target, std::vector<std::string> readout); // subset of dwarf_battle

// text and surfaces
int text(const std::string msg, int size, SDL_Color color, int x, int y, int alignx); // returns the width
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination);
void clear_screen();

// keyboard interfaces
bool wait_static(); // enter progresses, otherwise, no action
int wait_earth(); // main gameplay screen, captures keystrokes
int wait_battle(); // battle screen, captures keystrokes

// large scale structure
void init(World* w);
void clean_up();

} // namespace draw

#endif
