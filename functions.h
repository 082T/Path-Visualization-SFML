#pragma once

#include<vector>
#include<string>
#include<utility>
#include<tuple>
#include<SFML\Graphics.hpp>



typedef std::vector<sf::RectangleShape> rect_row;
typedef std::vector<std::vector<sf::RectangleShape>> rect_matrix;
typedef std::vector<std::vector<int>> state_matrix;
typedef std::vector<int> state_row;
typedef std::tuple <int, int, int, double> tupleiiid;
typedef std::pair<int, int> pii;

class Buttons;

struct myComp {
	constexpr bool operator()(
		tupleiiid const& a,
		tupleiiid const& b)
		const noexcept
	{
		return std::get<3>(a) > std::get<3>(b) ||
			std::get<3>(a) == std::get<3>(b) && std::get<0>(a) > std::get<0>(b) ||
			std::get<3>(a) == std::get<3>(b) && std::get<0>(a) == std::get<0>(b) && std::get<1>(a) > std::get<1>(b);
	}
};

class Grid {

private:

	rect_matrix rectangles; //rectangles
	state_matrix rec_states; //rectangles states
	std::vector<std::vector<pii>> previous_path; //path from end to begin after algo call

	sf::RenderWindow* gridWindow; //window
	
	int start_x = 0; //grid position x
	int start_y = 0; //grid position y
	int row = 0; //number of rows
	int col = 0; //number of cols
	int cell_size = 0; //size of cells
	
	pii begin; //start rectangle
	pii end; //end rectangle


public:

	Grid() {}
	Grid(int c, int r, sf::RenderWindow* window, int startx, int starty, int cellsize);
	void show();
	friend void erase_grid(Grid* grid);
	void set_cellsize(int x);
	void clicked(int mouse_x, int mouse_y, int state, Buttons buttons);
	void change_state_1(int x, int y, int state);
	void change_state_2(int x, int y, int state, Buttons buttons);
	void change_state_3(int x, int y, int state);
	void erase();
	void soft_erase();
	int get_value(std::string value);

	void algorithm_pick(std::string which);
	double distance_to_end(int x, int y);
	std::vector<pii> neighbour_expand(int x, int y);


};

class Buttons {

private:

	rect_row button_list; //list of buttons
	sf::RenderWindow* gridWindow; //window
	std::vector<sf::Text> text_list;

public:

	void create(int i, int j, int x, int y, std::string text_string, int size);
	void add_buttons();
	Buttons(sf::RenderWindow* window) : gridWindow(window) {}
	void show();
	friend void change_draw_state(int* state, int mouse_x, int mouse_y, Buttons* buttons);
	int getx(int which);
	int gety(int which);
	int getheight(int which);
	int getlength(int which);
	friend void Grid::clicked(int mouse_x, int mouse_y, int state, Buttons buttons);
	friend void change_state_2(int x, int y, int state, Buttons buttons);
	void change_text_color(std::string name, sf::Color color);

};

void change_draw_state(int* state, int mouse_x, int mouse_y, Buttons* buttons);

void register_click(int mouse_x, int mouse_y, Grid* grid, Buttons* buttons, int* state);