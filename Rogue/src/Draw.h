#ifndef Draw_H
#define Draw_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

#include "World.h"

namespace draw
{

void title(World w);

void text(const std::string msg, int size, SDL_Color color, int x, int y, int alignx);
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination);

void init(World w);
void clean_up();

} // namespace draw

#endif
