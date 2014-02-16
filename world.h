#include "task.h"
#include "dwarf.h"

class world
{
	public:
		world();

		int screen_size[2];
		int frame_rate;

		int earth;
		
		void init_names();

		void text(const char* msg,int size,int color,int location[3],bool centerx,bool centery,bool bold);


	protected:
		// Some colors
		// Grayscale
		int WHITE;
		int LIGHT_GRAY;
		int GRAY;
		int DARK_GRAY;
		int BLACK;
	
		// Others
		int BROWN;
		int GOLD;
		int RED;
		int DARK_BLUE;
		int YELLOW;

		void run();

		bool running;


	private:
		int seed;
};
