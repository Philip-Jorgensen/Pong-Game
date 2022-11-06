/* Pong specifications:
	http://www-classes.usc.edu/engr/ee-s/477p/s00/pong.html
*/
#include <SFML/Graphics.hpp>
#include <iostream>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 640


class player
{
public:
	// Public member variables
	float y_pos;

private:
	// Private member variables
	float x_pos;

	int paddle_height = 90;
	int paddle_width = 14;

	sf::RectangleShape paddle;


public:
	player(float x_position = 50, float y_position = 50)
	{
		x_pos = x_position;
		y_pos = y_position;

		paddle.setSize(sf::Vector2f(paddle_width, paddle_height));
	}

	void move(sf::Keyboard::Key up_key, sf::Keyboard::Key down_key)
	{
		if (sf::Keyboard::isKeyPressed(up_key))
		{
			y_pos -= 10;
		}

		if (sf::Keyboard::isKeyPressed(down_key))
		{
			y_pos += 10;
		}
		if (y_pos >= WINDOW_HEIGHT - paddle_height) y_pos = WINDOW_HEIGHT - paddle_height;
		if (y_pos <= 0) y_pos = 0;
	}


	void draw(sf::RenderWindow &window)
	{
		paddle.setPosition(x_pos, y_pos);
		window.draw(paddle);
	}

	float get_x_pos(void) { return x_pos; }
	int get_paddle_height(void) { return paddle_height; }
	
};

class ball
{
public:
	// Public member variables
	float y_pos;
	float x_pos;

private:
	// Private member variables
	float radius = 8;
	bool running = false;

	float x_vel = 15;
	float y_vel = 5;

	sf::CircleShape ball_shape;

public:
	ball(float x_position = WINDOW_WIDTH/2, float y_position = WINDOW_HEIGHT/2)
	{
		x_pos = x_position;
		y_pos = y_position;

		ball_shape.setRadius(radius);
	}

	void startToggle(sf::Keyboard::Key start_key)
	{
		if (sf::Keyboard::isKeyPressed(start_key))
			running = !running;
	}

	void reset(void)
	{
		running = false;
		x_pos = WINDOW_WIDTH / 2;
		y_pos = WINDOW_HEIGHT / 2;
	}

	void move(player &player1, player &player2)
	{
		if (running)
		{
			// Interaction between the walls
			if (x_pos >= WINDOW_WIDTH - radius || x_pos <= 0 + radius) reset(); // Left and right walls
			if (y_pos >= WINDOW_HEIGHT - radius || y_pos <= 0 + radius) y_vel *= -1; // Top and bottom walls

			// Interaction between the paddles
			if (x_pos >= player2.get_x_pos() - radius && 
				y_pos <= player2.y_pos + player2.get_paddle_height() && 
				y_pos >= player2.y_pos) 
					x_vel *= -1; 

			if (x_pos <= player1.get_x_pos() + radius && 
				y_pos <= player1.y_pos + player1.get_paddle_height() && 
				y_pos >= player1.y_pos) 
					x_vel *= -1; 

			// Movement
			x_pos += x_vel;
			y_pos += y_vel;
		}
	}

	void draw(sf::RenderWindow &window)
	{
		ball_shape.setPosition(x_pos, y_pos);
		window.draw(ball_shape);
	}

	void set_state(bool running_state)
	{
		running = running_state;
	}
};


int main(void)
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pong game");
	window.setFramerateLimit(45);

	// Line in the middle
	sf::RectangleShape mid_line(sf::Vector2f(6, WINDOW_HEIGHT));
	mid_line.setPosition(WINDOW_WIDTH / 2 - 3, 0);

	// Initializing the players
	player player1(50, 50);
	player player2(WINDOW_WIDTH - 50, 50);

	ball ball1;


	// Game loop
	while (window.isOpen())
	{
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window, exit
			if (event.type == sf::Event::Closed) window.close();

			// Close window, escape
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
		}
		ball1.startToggle(sf::Keyboard::Space);

		player2.move(sf::Keyboard::Up, sf::Keyboard::Down);
		player1.move(sf::Keyboard::W, sf::Keyboard::S);

		ball1.move(player1, player2);

		// Clear screen
		window.clear();

		// Drawing..
		window.draw(mid_line);

		player1.draw(window);
		player2.draw(window);
		ball1.draw(window);

		// Update the window
		window.display();
	}
}