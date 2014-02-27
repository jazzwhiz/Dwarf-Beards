#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#include "World.h"
#include "Dwarf.h"
#include "Timer.h"
#include "Earth.h"

const int framerate=60;
Timer fps;
SDL_Event e;
SDL_Surface *screen;
SDL_Surface *message;

// Some colors
// Grayscale
const SDL_Color World::WHITE={255,255,255};
const SDL_Color World::LIGHT_GRAY={150,150,150};
const SDL_Color World::GRAY={90,90,90};
const SDL_Color World::DARK_GRAY={60,60,60};
const SDL_Color World::BLACK={0,0,0};
// Others
const SDL_Color World::BROWN={139,69,19};
const SDL_Color World::GOLD={255,215,0};
const SDL_Color World::RED={255,0,0};
const SDL_Color World::DARK_BLUE={0,0,100};
const SDL_Color World::YELLOW={255,255,0};

World::World()
{
	// screen parameter stuff
	screen_size[0]=800;
	screen_size[1]=600;

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	screen=SDL_SetVideoMode(screen_size[0],screen_size[1],32,SDL_SWSURFACE);

	// rng
	seed=1888;
//	seed=time(NULL);
	srand(seed);

	// set up bools
	running=true;
	paused=false;

	version="0.02";
	copyright="(c) 2014 Peter Denton";

	init_names();

	earth_size[0]=50;
	earth_size[1]=50;
	earth_size[2]=11;
	earth=Earth();
	earth.init(earth_size);

	run();
	clean_up();
}

void World::text(const char* msg,int size,SDL_Color color,int x,int y,bool centerx)
{
	TTF_Font *font=NULL;
	font=TTF_OpenFont("Font/sfd/FreeSans.ttf",size);
	message=TTF_RenderText_Solid(font,msg,color);

	if (centerx)
	{
		int w;
		TTF_SizeText(font,msg,&w,NULL);
		x=x-w/2;
	}

	apply_surface(x,y,message,screen);
	TTF_CloseFont(font);
//	SDL_Flip(screen);
}
void World::apply_surface(int x,int y,SDL_Surface* source,SDL_Surface* destination)
{
	//Make a temporary rectangle to hold the offsets
	SDL_Rect offset;
	
	//Give the offsets to the rectangle
	offset.x = x;
	offset.y = y;
	//Blit the surface
	SDL_BlitSurface(source,NULL,destination,&offset);
}
void World::draw_world()
{
	
}
void World::run()
{
	title();
}
void World::title()
{
	bool titling=true;

	char tmp[100];
	text("jazzwhiz games",30,WHITE,screen_size[0]/2,40,true);
	text("presents",20,WHITE,screen_size[0]/2,100,true);
	text("DWARF BEARDS",50,WHITE,screen_size[0]/2,300,true);
	sprintf(tmp,"Version %s %s",version,copyright);
	text(tmp,15,DARK_GRAY,567,580,false);

	while (titling)
	{
		fps.start();
		SDL_PollEvent(&e); 
		if(e.type==SDL_QUIT)
			running=false;
		if(e.type==SDL_KEYDOWN)
		{
			switch(e.key.keysym.sym)
			{
				case SDLK_RETURN:
					titling=false;
					break;
				default:
					break;
			}
		}
		SDL_Flip(screen);
		if(fps.get_ticks()<1000/framerate)
		{
			SDL_Delay((1000/framerate)-fps.get_ticks());
		}
	}

}

void World::init_names()
{
	std::ifstream firstfile("first.txt");
	std::ifstream lastfile("last.txt");

	std::string tmp;
	char* tmp2;
	if (firstfile.is_open())
	{
		while (firstfile)
		{
			getline(firstfile,tmp);
			if (tmp=="") continue;
			tmp2=strdup(tmp.c_str());
			firsts.push_back(tmp2);
		}
	}
	if (lastfile.is_open())
	{
		while (lastfile)
		{
			getline(lastfile,tmp);
			if (tmp=="") continue;
			tmp2=strdup(tmp.c_str());
			lasts.push_back(tmp2);
		}
	}
	firstfile.close();
	lastfile.close();
}

char* World::random_name()
{
	char* name=new char[50];
	sprintf(name,"%s %s",firsts[rand()%firsts.size()],lasts[rand()%lasts.size()]);
	return name;
}

void World::clean_up()
{
	SDL_FreeSurface(message);
	TTF_Quit();
	SDL_Quit();	
}
