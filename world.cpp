#include <allegro.h>
#include <iostream>
#include "world.h"

world::world()
{
	int screen_size[]={800,600};
	frame_rate=40;

	seed=1888;
	srand(seed);
//	srand(time(NULL));

	// set up bools
	running=true;

	// set up allegro
	allegro_init();
	install_keyboard();
	set_color_depth(16);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED,screen_size[0],screen_size[1],0,0);

	// Some colors
	// Grayscale
	WHITE=makecol(255,255,255);
	LIGHT_GRAY=makecol(150,150,150);
	GRAY=makecol(90,90,90);
	DARK_GRAY=makecol(60,60,60);
	BLACK=makecol(0,0,0);
	
	// Others
	BROWN=makecol(139,69,19);
	GOLD=makecol(255,215,0);
	RED=makecol(255,0,0);
	DARK_BLUE=makecol(0,0,100);
	YELLOW=makecol(255,255,0);

	run();
}

void world::text(const char* msg,int size,int color,int location[3],bool centerx,bool centery,bool bold)
{
	acquire_screen();
	textout_ex(screen,font,msg,location[0],location[1],color,-1);
	release_screen();
}

void world::run()
{
	while (running)
	{
		if (key[KEY_DOWN]) 
		{
			int a[3]={1,2,3};
			text("asdfj",3,makecol(255,255,255),a,false,false,false);
		}
//		if (key[KEY_UP]) text("jkl;o",3,WHITE,triplet(20,20,4),false,false,false);
	}

}
