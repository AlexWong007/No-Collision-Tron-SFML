#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>

void grid(sf::RenderWindow&);
void PlayersSpawn(sf::RenderWindow&, sf::CircleShape&, sf::CircleShape&);
void DrawPlayers(sf::RenderWindow&, sf::CircleShape&, sf::CircleShape&);
void Player1Move(sf::CircleShape&, sf::Keyboard::Key&);
void Player2Move(sf::CircleShape&, sf::Keyboard::Key&);
void menu(sf::RenderWindow&, bool&, sf::CircleShape&, sf::CircleShape&);
void CheckBorderCollision(sf::CircleShape, sf::CircleShape, bool&);
void Player1Trail(sf::RenderWindow&, std::vector<sf::RectangleShape>&, sf::CircleShape);
void Player2Trail(sf::RenderWindow&, std::vector<sf::RectangleShape>&, sf::CircleShape);
void CheckPlayerCollision(std::vector<sf::RectangleShape>, std::vector<sf::RectangleShape>, sf::CircleShape, sf::CircleShape, bool&);

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Tron by Alex Wong", sf::Style::Default, settings);
	window.setFramerateLimit(60);
	sf::CircleShape Player1;
	sf::CircleShape Player2;
	bool GameOver = true;
	sf::Music music;
	sf::Keyboard::Key PreviousMove = sf::Keyboard::D; 
	sf::Keyboard::Key PreviousMove2 = sf::Keyboard::Left;
	std::vector<sf::RectangleShape> TrailVector1;
	std::vector<sf::RectangleShape> TrailVector2;

	if (!music.openFromFile("theme.mp3"))
	{
		std::cout << "Audio Error" << std::endl;
	}

	music.setVolume(5);
	music.play();

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear(sf::Color::Black);

		if(!GameOver)
		{
			//music.stop();
			CheckPlayerCollision(TrailVector1, TrailVector2, Player1, Player2, GameOver);
			CheckBorderCollision(Player1, Player2, GameOver);
			Player1Move(Player1, PreviousMove);
			Player2Move(Player2, PreviousMove2);
			grid(window);
			Player1Trail(window, TrailVector1, Player1);
			Player2Trail(window, TrailVector2, Player2);
			DrawPlayers(window, Player1, Player2);
		}
		else
		{
			TrailVector1.clear();
			TrailVector2.clear();
			PreviousMove = sf::Keyboard::D; 
			PreviousMove2 = sf::Keyboard::Left;
			menu(window, GameOver, Player1, Player2);
		}

		window.display();
	}
}

void menu(sf::RenderWindow& window, bool& GameOver, sf::CircleShape& Player1, sf::CircleShape& Player2)
{
	sf::Sprite sprite;
	sf::Texture texture;
	sf::RectangleShape Horizontal(sf::Vector2f(1280, 5));;
	sf::RectangleShape Vertical(sf::Vector2f(5, 720));;
	sf::Font font;
	sf::Text text;

	grid(window);

	if (!texture.loadFromFile("tron_logo.png"))
	{
		return;
	}
	texture.setSmooth(true);

	if (!font.loadFromFile("TRON.ttf"))
	{
		return;
	}
	text.setFont(font);
	text.setString("left click to start");
	text.setFillColor(sf::Color::Blue);
	text.setOutlineColor(sf::Color::White);
	text.setOutlineThickness(2);
	text.setPosition(sf::Vector2f(348, 650));

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		GameOver = false;
	}

	Horizontal.setFillColor(sf::Color(102, 136, 52));
	Horizontal.setPosition(sf::Vector2f(0, 343));
	Horizontal.setOutlineColor(sf::Color::Black);
	Horizontal.setOutlineThickness(2);

	Vertical.setFillColor(sf::Color(102, 136, 52));
	Vertical.setPosition(sf::Vector2f(692, 0));
	// Vertical.setOutlineColor(sf::Color::Black);
	// Vertical.setOutlineThickness(2);

	sprite.setTexture(texture);
	sprite.setPosition(sf::Vector2f(300, 250));
	sprite.setScale(sf::Vector2f(1, 1));

	PlayersSpawn(window, Player1, Player2);

	window.draw(text);
	window.draw(sprite);
	window.draw(Horizontal);
	window.draw(Vertical);
}

void grid(sf::RenderWindow& window)
{
	sf::RectangleShape gridSquare(sf::Vector2f(50, 50));
	gridSquare.setFillColor(sf::Color(4, 2, 33));

	for (int r = 0; r < 750; r += 52)
	{
		for (int c = 0; c < 1300; c += 52)
		{
			gridSquare.setPosition(sf::Vector2f(c, r));
			window.draw(gridSquare);
		}
	}
}	

void PlayersSpawn(sf::RenderWindow& window, sf::CircleShape& Player1, sf::CircleShape& Player2)
{
	Player1.setFillColor(sf::Color::Blue);
	Player1.setRadius(3);
	Player1.setPosition(sf::Vector2f(100, 360));

	Player2.setFillColor(sf::Color::Red);
	Player2.setRadius(3);
	Player2.setPosition(sf::Vector2f(1180, 360));
}

void DrawPlayers (sf::RenderWindow& window, sf::CircleShape& Player1, sf::CircleShape& Player2)
{
	window.draw(Player1);
	window.draw(Player2);
}

void Player1Move(sf::CircleShape& Player1, sf::Keyboard::Key& PreviousMove)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			Player1.move(sf::Vector2f(0, -4));
			PreviousMove = sf::Keyboard::W;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			Player1.move(sf::Vector2f(-4, 0));
			PreviousMove = sf::Keyboard::A;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			Player1.move(sf::Vector2f(0, 4));
			PreviousMove = sf::Keyboard::S;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			Player1.move(sf::Vector2f(4, 0));
			PreviousMove = sf::Keyboard::D;
		}
	}
	else
	{
		if (PreviousMove == sf::Keyboard::W)
		{
			Player1.move(sf::Vector2f(0, -4));
		}
		else if (PreviousMove == sf::Keyboard::A)
		{
			Player1.move(sf::Vector2f(-4, 0));
		}
		else if (PreviousMove == sf::Keyboard::S)
		{
			Player1.move(sf::Vector2f(0, 4));
		}
		else if (PreviousMove == sf::Keyboard::D)
		{
			Player1.move(sf::Vector2f(4, 0));
		}
	}
}

void Player2Move(sf::CircleShape& Player2, sf::Keyboard::Key& PreviousMove)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			Player2.move(sf::Vector2f(0, -4));
			PreviousMove = sf::Keyboard::Up;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			Player2.move(sf::Vector2f(-4, 0));
			PreviousMove = sf::Keyboard::Left;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			Player2.move(sf::Vector2f(0, 4));
			PreviousMove = sf::Keyboard::Down;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			Player2.move(sf::Vector2f(4, 0));
			PreviousMove = sf::Keyboard::Right;
		}
	}
	else
	{
		if (PreviousMove == sf::Keyboard::Up)
		{
			Player2.move(sf::Vector2f(0, -4));
		}
		else if (PreviousMove == sf::Keyboard::Left)
		{
			Player2.move(sf::Vector2f(-4, 0));
		}
		else if (PreviousMove == sf::Keyboard::Down)
		{
			Player2.move(sf::Vector2f(0, 4));
		}
		else if (PreviousMove == sf::Keyboard::Right)
		{
			Player2.move(sf::Vector2f(4, 0));
		}
	}
}

void CheckBorderCollision(sf::CircleShape Player1, sf::CircleShape Player2, bool& GameOver)
{
	sf::Vector2f Player1Position = Player1.getPosition();
	sf::Vector2f Player2Position = Player2.getPosition();

	if (Player1Position.x <= 0 || Player1Position.x >= 1280 || Player1Position.y <= 0 || Player1Position.y >= 720)
	{
		GameOver = true;
	}

	if (Player2Position.x <= 0 || Player2Position.x >= 1280 || Player2Position.y <= 0 || Player2Position.y >= 720)
	{
		GameOver = true;
	}
}

void Player1Trail(sf::RenderWindow& window, std::vector<sf::RectangleShape>& TrailVector1, sf::CircleShape Player1)
{
	sf::RectangleShape Trail(sf::Vector2f(7, 7));
	Trail.setFillColor(sf::Color::Blue);
	Trail.setOutlineColor(sf::Color(51, 133, 255));
	Trail.setOutlineThickness(1);

	sf::Vector2f Player1Position = Player1.getPosition();
	Trail.setPosition(Player1Position);
	TrailVector1.push_back(Trail);

	for (sf::RectangleShape Trail : TrailVector1)
	{
		window.draw(Trail);
	}
}

void Player2Trail(sf::RenderWindow& window, std::vector<sf::RectangleShape>& TrailVector2, sf::CircleShape Player2)
{
	sf::RectangleShape Trail(sf::Vector2f(7, 7));
	Trail.setFillColor(sf::Color::Red);
	Trail.setOutlineColor(sf::Color(230, 115, 0));
	Trail.setOutlineThickness(1);

	sf::Vector2f Player2Position = Player2.getPosition();
	Trail.setPosition(Player2Position);
	TrailVector2.push_back(Trail);

	for (sf::RectangleShape Trail : TrailVector2)
	{
		window.draw(Trail);
	}
}

void CheckPlayerCollision(std::vector<sf::RectangleShape> TrailVector1, std::vector<sf::RectangleShape> TrailVector2, sf::CircleShape Player1, sf::CircleShape Player2, bool& GameOver)
{
	sf::Vector2f Player1Position = Player1.getPosition();
	sf::Vector2f Player2Position = Player2.getPosition();

	for (sf::RectangleShape Trail : TrailVector1)
	{
		if (Player2Position == Trail.getPosition())
		{
			GameOver = true;
		}
	}

	for (sf::RectangleShape Trail : TrailVector2)
	{
		if (Player1Position == Trail.getPosition())
		{
			GameOver = true;
		}
	}
}