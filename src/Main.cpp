#include "Game.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>

int main()
{
	srand((unsigned)time(NULL));
	Game g("config/config.txt");
	g.run();
	return 0;
}