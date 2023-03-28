#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Simulator;

class Graphics
{
private:
	sf::RenderWindow* window;
	sf::Texture bgTexture;
	sf::Sprite bgSprite;
	sf::Texture appleTexture;
	sf::Sprite appleSprite;
	sf::Font font;
	sf::Text scoreText;
	sf::Text statText;
	Simulator* simulator;
	sf::Vector2f cellSize;
	sf::Vector2f topLeft;
	sf::Vector2f scoreOrigin;
	unsigned int* gameSpeed;
	unsigned int* generation;

	/// <summary>
	/// Updates sprite size to keep relative with window size
	/// </summary>
	/// <param name="sprite">Sprite to be updated</param>
	/// <param name="percent">Size of sprite relative to window size</param>
	void updateSize(sf::Sprite& sprite, float percent);

	void updateCellSize();
	void drawSnakes();
	bool drawScore();
	void drawGameSpeed();
	void drawGeneration();
	void drawGameOver();

public:
	/// <summary>
	/// </summary>
	/// <param name="width">Width of window in pixels</param>
	/// <param name="height">Height of widow in pixels</param>
	/// <param name="title">Window title</param>
	/// <param name="sim">Simulator to be drawn</param>
	Graphics(unsigned int width, unsigned int height, std::string title, Simulator* simulator = nullptr);

	/// <summary>
	/// Draws frame on screen
	/// </summary>
	/// <returns>True if displayed simulator has live snakes</returns>
	bool draw(bool displayGameOverOnEnd = false);

	/// <param name="simulator">Simulator to be displayed</param>
	void setSimulator(Simulator* simulator);

	void setGameSpeedTracker(unsigned int* speed);
	void setGenerationTracker(unsigned int* generation);

	/// <summary>
	/// Wrapper for sf::RenderWindow::pollEvent
	/// used for processing user input
	/// handles window close
	/// </summary>
	/// <param name="event">Variable to be initialized</param>
	/// <returns>True if event stack is not empty</returns>
	bool pollEvent(sf::Event& event);

	/// <summary>
	/// Wrapper for sf::RenderWindow::isOpen
	/// </summary>
	/// <returns>True if window is open</returns>
	bool isOpen();

	~Graphics();
};
