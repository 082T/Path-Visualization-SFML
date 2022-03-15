#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include<set>
#include<queue>
#include<tuple>
#include<functional>
#include<chrono>
#include<thread>
#include<SFML\Graphics.hpp>
#include"functions.h"


Grid::Grid(int c, int r, sf::RenderWindow* window, int startx, int starty, int cellsize) :
			col(c), row(r), gridWindow(window), begin(-1, -1), end(-1, -1),
			start_x(startx), start_y(starty), cell_size(cellsize)
{
	for (int i = 0; i < col; i++)
	{
		rect_row rows;
		state_row row_states;
		std::vector<pii> vec_pairs;

		for (int j = 0; j < row; j++)
		{
			vec_pairs.push_back({ 0,0 });
			row_states.push_back(0);
			sf::RectangleShape shape(sf::Vector2f(cell_size, cell_size));
			shape.setFillColor(sf::Color(255, 255, 255));
			shape.setOutlineThickness(-2.f);
			shape.setOutlineColor(sf::Color(0, 153, 153));
			shape.setPosition(start_x + i * (cell_size - 2), start_y + j * (cell_size - 2));
			rows.push_back(shape);
		}
		rectangles.push_back(rows);
		rec_states.push_back(row_states);
		previous_path.push_back(vec_pairs);
	}
}

void Grid::show()
{
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			gridWindow->draw(rectangles[i][j]);
		}
	}
}

void Grid::set_cellsize(int x)
{
	cell_size = x;
}

void Grid::clicked(int mouse_x, int mouse_y, int state, Buttons buttons)
{
	int index_x = (mouse_x - start_x - 1) / (cell_size - 2);
	int index_y = (mouse_y - start_y - 1) / (cell_size - 2);
	
	if (state!=2)
		std::cout << "Clicked rectangle (" << index_x << ", " << index_y << ")" << std::endl;

	switch (state){
		case 1:
			this->change_state_1(index_x, index_y, state);
			break;
		case 2:
			this->change_state_2(index_x, index_y, state, buttons);
			break;
		case 3:
			this->change_state_3(index_x, index_y, state);
			break;
	}
	


}

void Grid::change_state_1(int x, int y, int state)
{
	pii prior;

	if (begin != std::make_pair(-1, -1)) //remove current begin
	{
		rectangles[begin.first][begin.second].setFillColor(sf::Color(255, 255, 255));
		rec_states[begin.first][begin.second] = 0;
		prior = begin;
		begin = std::make_pair(-1, -1);
	}
	if (end == std::make_pair(x, y)) //if already end
	{
		end = std::make_pair(-1, -1);
		begin = std::make_pair(x, y);
		rectangles[x][y].setFillColor(sf::Color(0, 0, 255));
		rec_states[x][y] = state;
	}
	else if (prior != std::make_pair(x, y)) //if not taken or obstacle
	{
		begin = std::make_pair(x, y);
		rectangles[x][y].setFillColor(sf::Color(0, 0, 255));
		rec_states[x][y] = state;
	}
}

void Grid::change_state_2(int x, int y, int state, Buttons buttons)
{
	while (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		int mouse_x = sf::Mouse::getPosition(*gridWindow).x;
		int mouse_y = sf::Mouse::getPosition(*gridWindow).y;
		int ok = 0;
		if (mouse_x > get_value("start_x") && mouse_y > get_value("start_y") && mouse_x < get_value("start_x") + (get_value("cell_size") - 2) * get_value("col") + 1 && mouse_y < get_value("start_y") + (get_value("cell_size") - 2) * get_value("row") + 1)
			ok = 1;
		if (ok == 0) continue;
		int index_x = (mouse_x - start_x - 1) / (cell_size - 2);
		int index_y = (mouse_y - start_y - 1) / (cell_size - 2);

		if (begin == std::make_pair(index_x, index_y)) //if already begin
		{
			begin = std::make_pair(-1, -1);
		}
		if (end == std::make_pair(index_x, index_y)) //if already end
		{
			end = std::make_pair(-1, -1);
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) //if already obstacle
		{
			rec_states[index_x][index_y] = 0;
			rectangles[index_x][index_y].setFillColor(sf::Color(255, 255, 255));
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			rec_states[index_x][index_y] = state;
			rectangles[index_x][index_y].setFillColor(sf::Color(240, 14, 14));
		}
		
		gridWindow->clear(sf::Color::White);
		this->show();
		buttons.show();
		gridWindow->display();
	}
}

void Grid::change_state_3(int x, int y, int state)
{
	pii prior;

	if (end != std::make_pair(-1, -1)) //remove current end
	{
		rectangles[end.first][end.second].setFillColor(sf::Color(255, 255, 255));
		rec_states[end.first][end.second] = 0;
		prior = end;
		end = std::make_pair(-1, -1);
	}
	if (begin == std::make_pair(x, y)) //if already begin
	{
		begin = std::make_pair(-1, -1);
		end = std::make_pair(x, y);
		rectangles[x][y].setFillColor(sf::Color(0, 0, 0));
		rec_states[x][y] = state;
	}
	else if (prior != std::make_pair(x, y)) //if not taken or obstacle
	{
		end = std::make_pair(x, y);
		rectangles[x][y].setFillColor(sf::Color(0, 0, 0));
		rec_states[x][y] = state;
	}
}

void Buttons::create(int i, int j, int x, int y, std::string text_string, int size)
{
	sf::RectangleShape shape(sf::Vector2f(x, y));
	shape.setFillColor(sf::Color(255, 255, 255));
	shape.setOutlineThickness(-3.f);
	shape.setOutlineColor(sf::Color(0, 0, 0));
	shape.setPosition(i, j);
	button_list.push_back(shape);

	sf::Font font;
	font.loadFromFile("Fonts/cmr10.ttf");
	sf::Text text;
	text.setFont(font);
	text.setString(text_string);
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(size);

	double width = text.getLocalBounds().width;
	double height = text.getLocalBounds().height;
	text.setPosition(((shape.getSize().x - width) / 2) + shape.getPosition().x, ((shape.getSize().y - 2 * height) / 2) + shape.getPosition().y);

	text_list.push_back(text);
}

void Buttons::show()
{
	for (auto it = button_list.begin(); it != button_list.end(); it++)
	{
		gridWindow->draw(*it);
	}
	for (auto it = text_list.begin(); it != text_list.end(); it++)
	{
		sf::Font font;
		font.loadFromFile("Fonts/cmr10.ttf");
		it->setFont(font);
		gridWindow->draw(*it);
	}
}

int Buttons::getx(int which)
{
	return button_list[which].getPosition().x;
	return 0;
}

int Buttons::gety(int which)
{
	return button_list[which].getPosition().y;
	return 0;
}

void Buttons::add_buttons()
{
	create(936, 640, 120, 80, "START",18);
	create(1053, 640, 120, 80, "OBSTACLE",18);
	create(1170, 640, 120, 80, "END", 18);
	create(756, 640, 120, 80, "CLEAR", 18);
	create(10, 640, 120, 80, "A*", 18);
	create(140, 640, 120, 80, "greedyBFS", 18);
	create(639, 640, 120, 80, "SOFT CLEAR", 17);
	create(270, 640, 120, 80, "DIJKSTRA", 18);
}

int Buttons::getheight(int which)
{
	return button_list[which].getSize().y;
}

int Buttons::getlength(int which)
{
	return button_list[which].getSize().x;
}

void Buttons::change_text_color(std::string name, sf::Color color)
{
	for (auto it = text_list.begin(); it != text_list.end(); it++)
	{
		if (it->getString() == name) 
		{
			it->setFillColor(color);
		}
	}
}

void change_draw_state(int* state, int mouse_x, int mouse_y, Buttons* buttons)
{
	if (mouse_x < buttons->getx(1) + 1)
	{
		buttons->change_text_color("END", sf::Color::Black);
		buttons->change_text_color("START", sf::Color::White);
		std::cout << "Change draw state: start" << std::endl;
		*state = 1;
		buttons->button_list[0].setFillColor(sf::Color(0, 0, 255));
		buttons->button_list[1].setFillColor(sf::Color(255, 255, 255));
		buttons->button_list[2].setFillColor(sf::Color(255, 255, 255));
	}
	else if (mouse_x < buttons->getx(2) + 1)
	{
		buttons->change_text_color("END", sf::Color::Black);
		buttons->change_text_color("START", sf::Color::Black);
		std::cout << "Change draw state: obstacle" << std::endl;
		*state = 2;
		buttons->button_list[0].setFillColor(sf::Color(255, 255, 255));
		buttons->button_list[1].setFillColor(sf::Color(240, 14, 14));
		buttons->button_list[2].setFillColor(sf::Color(255, 255, 255));
	}
	else
	{
		buttons->change_text_color("END", sf::Color::White);
		buttons->change_text_color("START", sf::Color::Black);
		std::cout << "Change draw state: end" << std::endl;
		*state = 3;
		buttons->button_list[0].setFillColor(sf::Color(255, 255, 255));
		buttons->button_list[1].setFillColor(sf::Color(255, 255, 255));
		buttons->button_list[2].setFillColor(sf::Color(0, 0, 0));
	}
}

void register_click(int mouse_x, int mouse_y, Grid* grid, Buttons* buttons, int* state)
{
	if (mouse_x > grid->get_value("start_x") && mouse_y > grid->get_value("start_y") && mouse_x < grid->get_value("start_x") + (grid->get_value("cell_size") - 2) * grid->get_value("col") + 1 && mouse_y < grid->get_value("start_y") + (grid->get_value("cell_size") - 2) * grid->get_value("row") + 1)
		grid->clicked(mouse_x, mouse_y, *state, *buttons);
	if (mouse_x > buttons->getx(0) && mouse_x < buttons->getx(2) + buttons->getlength(2) && mouse_y > buttons->gety(0) && mouse_y < buttons->gety(0) + buttons->getheight(0))
		change_draw_state(state, mouse_x, mouse_y, buttons);
	else if (mouse_x > buttons->getx(3) && mouse_x < buttons->getx(3) + buttons->getlength(3) && mouse_y > buttons->gety(3) && mouse_y < buttons->gety(3) + buttons->getheight(3))
		grid->erase();
	else if (mouse_x > buttons->getx(4) && mouse_x < buttons->getx(4) + buttons->getlength(4) && mouse_y > buttons->gety(4) && mouse_y < buttons->gety(4) + buttons->getheight(4))
		grid->algorithm_pick("astar");
	else if (mouse_x > buttons->getx(5) && mouse_x < buttons->getx(5) + buttons->getlength(5) && mouse_y > buttons->gety(5) && mouse_y < buttons->gety(5) + buttons->getheight(5))
		grid->algorithm_pick("greedyBFS");
	else if (mouse_x > buttons->getx(6) && mouse_x < buttons->getx(6) + buttons->getlength(6) && mouse_y > buttons->gety(6) && mouse_y < buttons->gety(6) + buttons->getheight(6))
		grid->soft_erase();
	else if (mouse_x > buttons->getx(7) && mouse_x < buttons->getx(7) + buttons->getlength(7) && mouse_y > buttons->gety(7) && mouse_y < buttons->gety(7) + buttons->getheight(7))
		grid->algorithm_pick("dijkstra");
}

void Grid::erase()
{
	std::cout << "Grid: erase" << std::endl;
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			rectangles[i][j].setFillColor(sf::Color(255, 255, 255));
			rec_states[i][j] = 0;
			previous_path[i][j] = { 0,0 };
		}
	}
	begin = std::make_pair(-1, -1);
	end = std::make_pair(-1, -1);
}

void Grid::soft_erase()
{
	std::cout << "Grid: soft erase" << std::endl;
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			if (rec_states[i][j] == 4)
			{
				rectangles[i][j].setFillColor(sf::Color(255, 255, 255));
				rec_states[i][j] = 0;
				previous_path[i][j] = { 0,0 };
			}
		}
	}
}

int Grid::get_value(std::string value)
{
	if (value == "start_x") return start_x;
	else if (value == "start_y") return start_y;
	else if (value == "cell_size") return cell_size;
	else if (value == "col") return col;
	else if (value == "row") return row;

	return 0;
}

void Grid::algorithm_pick(std::string which)
{
	int found = 0;
	int first = 1;

	std::cout << "Algorithm call: " << which << std::endl;
	if (begin == std::make_pair(-1, -1))
	{
		std::cout << "-Error: start missing" << std::endl;
		return;
	}
	if (end == std::make_pair(-1, -1))
	{
		std::cout << "-Error: end missing" << std::endl;
		return;
	}

	std::set<pii> closed;
	
	std::priority_queue<tupleiiid, std::vector<tupleiiid>, myComp> open;
	open.push({ begin.first,begin.second,0,distance_to_end(begin.first,begin.second) });
	
	

	while (!open.empty())
	{
		tupleiiid qu_top = open.top();
		open.pop();
		closed.insert({ std::get<0>(qu_top), std::get<1>(qu_top) });
		if (first == 0)
		{
			rectangles[std::get<0>(qu_top)][std::get<1>(qu_top)].setFillColor(sf::Color(45, 161, 145));
		}
		first = 0;
		std::vector<pii> expand = this->neighbour_expand(std::get<0>(qu_top), std::get<1>(qu_top));

		while (!expand.empty())
		{
			pii last = expand.back();
			expand.pop_back();
			if (closed.find(last) != closed.end()) //already visited
			{
				continue;
			}
			if (rec_states[last.first][last.second] == 2) continue; //obstacle found
			
			if (rec_states[last.first][last.second] == 1) continue; //begin found

			previous_path[last.first][last.second] = { std::get<0>(qu_top), std::get<1>(qu_top) };

			if (rec_states[last.first][last.second] == 3) //end found
			{
				found = 1;
				break;
			}
			rec_states[last.first][last.second] = 4;

			if (which == "astar")
				open.push({ last.first,last.second,std::get<2>(qu_top) + 1 , std::get<2>(qu_top) + 1 + distance_to_end(last.first,last.second) });
			else if (which == "greedyBFS")
				open.push({ last.first,last.second,std::get<2>(qu_top) + 1 , distance_to_end(last.first,last.second) });
			else if (which == "dijkstra")
				open.push({ last.first,last.second,std::get<2>(qu_top) + 1 , std::get<2>(qu_top) + 1 });

			rectangles[last.first][last.second].setFillColor(sf::Color(56, 201, 181));
			closed.insert({ last.first,last.second });

			gridWindow->clear(sf::Color::White);
			this->show();
			gridWindow->display();

		}
		if (found == 1) break;

	}
	pii last_path = end;
	if (found == 0) return;
	while (1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		pii i = previous_path[last_path.first][last_path.second];
		if (i == begin) break;
		last_path = i;
		rectangles[last_path.first][last_path.second].setFillColor(sf::Color(0, 255, 0));
		gridWindow->clear(sf::Color::White);
		this->show();
		gridWindow->display();
	}
}

double Grid::distance_to_end(int x, int y)
{
	return sqrt((end.first - x) * (end.first - x) + (end.second - y) * (end.second - y));
}

std::vector<pii> Grid::neighbour_expand(int x, int y)
{
	std::vector<pii> to_return;

	if (x > 0) to_return.push_back({ x - 1, y });
	if (x < col-1) to_return.push_back({ x + 1,y });
	if (y > 0) to_return.push_back({ x,y - 1 });
	if (y < row-1) to_return.push_back({ x,y + 1 });

	return to_return;
	
}








