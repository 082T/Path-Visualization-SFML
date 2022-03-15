#include<iostream>
#include<vector>
#include<string>
#include<SFML\Graphics.hpp>
#include"functions.h"





int main() 
{
	int windowx = 1300;
	int windowy = 750;
	int click = 0;
	int grid_start_x = 89, grid_start_y = 30;
	int cellnumber_x = 40, cellnumber_y = 20;
	int cellsize = 30;

	sf::RenderWindow window(sf::VideoMode(windowx, windowy), "Path visualization");
	
	Grid grid(cellnumber_x, cellnumber_y, &window, grid_start_x, grid_start_y, cellsize);
	Buttons buttons(&window);

	buttons.add_buttons();

	int state = 0;

	
	while (window.isOpen())
	{
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed) window.close();
			if (Event.type == sf::Event::MouseButtonPressed)
			{
				int mouse_x = sf::Mouse::getPosition(window).x;
				int mouse_y = sf::Mouse::getPosition(window).y;

				register_click(mouse_x, mouse_y, &grid, &buttons, &state);
			}
		}

		window.clear(sf::Color::White);
		grid.show();
		buttons.show();
		window.display();
	}


	return 0;
}






