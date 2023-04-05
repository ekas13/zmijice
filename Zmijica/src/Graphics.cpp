#include "Graphics.h"
#include "Simulator.h"

#include <vector>
#include <string>
#include <algorithm>

void Graphics::updateSize(sf::Sprite& sprite, float percent)
{
	sf::Vector2u windowSize = window->getSize();

	sf::Vector2f targetSize;
	targetSize.x = windowSize.x * percent;
	targetSize.y = windowSize.y * percent;

	float shrink = targetSize.y / sprite.getLocalBounds().height;

	sprite.setScale(shrink, shrink);
}

void Graphics::updateCellSize()
{
	float shrink = window->getDefaultView().getSize().y / 1080;

	float screenSpace = 975 * shrink;

	float len = screenSpace / simulator->getMapSize();
	cellSize.x = len;
	cellSize.y = len;

	topLeft.x = 52.5 * shrink;
	topLeft.y = 52.5 * shrink;
}

void Graphics::drawSnakes()
{
	sf::RectangleShape cell;
	cell.setSize(cellSize);
	std::vector<std::vector<int>> map = simulator->getMap();
	int noCells = simulator->getMapSize();

	for (int i = 0; i < noCells; i++)
	{
		for (int j = 0; j < noCells; j++)
		{
			switch (map[i][j])
			{
			case(1):
				appleSprite.setPosition(j * cellSize.x + topLeft.x, i * cellSize.y + topLeft.y);
				window->draw(appleSprite);
				break;
			case(2):
				cell.setFillColor(sf::Color::Green);
				cell.setPosition(j * cellSize.x + topLeft.x, i * cellSize.y + topLeft.y);
				window->draw(cell);
				break;
			case(3):
				cell.setFillColor(sf::Color::Red);
				cell.setPosition(j * cellSize.x + topLeft.x, i * cellSize.y + topLeft.y);
				window->draw(cell);
				break;
			case(4):
				cell.setFillColor(sf::Color::Blue);
				cell.setPosition(j * cellSize.x + topLeft.x, i * cellSize.y + topLeft.y);
				window->draw(cell);
				break;
			case(5):
				cell.setFillColor(sf::Color::Yellow);
				cell.setPosition(j * cellSize.x + topLeft.x, i * cellSize.y + topLeft.y);
				window->draw(cell);
				break;
			default:
				break;
			}
		}
	}
}

bool Graphics::drawScore()
{
	std::vector<std::shared_ptr<SnakeBase>> liveSnakes = simulator->getLiveSnakes();
	std::vector<std::shared_ptr<SnakeBase>> deadSnakes = simulator->getDeadSnakes();
	std::vector<std::shared_ptr<SnakeBase>> allSnakes = liveSnakes;
	allSnakes.insert(allSnakes.end(), deadSnakes.begin(), deadSnakes.end());

	std::sort(allSnakes.begin(), allSnakes.end(),
		[](std::shared_ptr<SnakeBase>s1, std::shared_ptr<SnakeBase> s2) {return s1->getScore() > s2->getScore(); });

	for (int i = 0; i < allSnakes.size(); i++)
	{
		switch (allSnakes.at(i)->getIndex())
		{
		case(0):
			scoreText.setFillColor(sf::Color::Green);
			break;
		case(1):
			scoreText.setFillColor(sf::Color::Red);
			break;
		case(2):
			scoreText.setFillColor(sf::Color::Blue);
			break;
		case(3):
			scoreText.setFillColor(sf::Color::Yellow);
			break;
		default:
			break;
		}

		std::string t = "Snake ";
		t.append(std::to_string(allSnakes.at(i)->getIndex() + 1) + ": " + std::to_string(allSnakes.at(i)->getScore()));

		if (std::find(liveSnakes.begin(), liveSnakes.end(), allSnakes.at(i)) == liveSnakes.end())
		{
			sf::Color col = scoreText.getFillColor();
			col.a /= 2;
			scoreText.setFillColor(col);
		}

		scoreText.setString(t);
		scoreText.setPosition(scoreOrigin.x, scoreOrigin.y + 60 * i);
		window->draw(scoreText);
	}

	return liveSnakes.size() > 0;
}

void Graphics::drawGameSpeed()
{
	if (!gameSpeed)
		return;

	std::string s = "Speed: " + std::to_string(*gameSpeed) + "x";
	statText.setString(s);
	statText.setPosition(window->getDefaultView().getSize().x - 150, window->getDefaultView().getSize().y - 30);

	window->draw(statText);
}

void Graphics::drawGeneration()
{
	if (!generation)
		return;

	std::string s = "Generation: #" + std::to_string(*generation);
	statText.setString(s);
	statText.setPosition(window->getDefaultView().getSize().x - 350, window->getDefaultView().getSize().y - 30);

	window->draw(statText);
}

void Graphics::drawGameOver()
{
	sf::Text gameOver("Game over: press R to restart.", font, 30);
	gameOver.setPosition(window->getView().getSize().x * 0.14, window->getView().getSize().y * 0.5);
	gameOver.setFillColor(sf::Color::Color(80, 80, 80));
	window->draw(gameOver);
}

Graphics::Graphics(unsigned int width, unsigned int height, std::string title, Simulator* simulator)
{
	this->simulator = simulator;

	window = new sf::RenderWindow(sf::VideoMode(width, height), title);

	//window->setVerticalSyncEnabled(true);
	//Capped to 60 fps for consistent game speed across devices
	window->setFramerateLimit(60);

	if (!bgTexture.loadFromFile("./res/background.png"))
		throw std::exception("Failed to load background: /res/background.png");

	if (!appleTexture.loadFromFile("./res/apple.png"))
		throw std::exception("Failed to load texture: /res/apple.png");

	if (!font.loadFromFile("./res/Arial.ttf"))
		throw std::exception("Failed to load font: /res/Arial.ttf");

	sf::Image icon;
	if (!icon.loadFromFile("./res/icon.png"))
		throw std::exception("Failed to load icon: /res/icon.png");
	window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	scoreText.setFont(font);
	scoreText.setStyle(sf::Text::Italic);
	scoreText.setCharacterSize(30);

	statText.setFont(font);
	statText.setFillColor(sf::Color::White);
	statText.setCharacterSize(16);
	
	bgSprite.setTexture(bgTexture);
	updateSize(bgSprite, 1);

	appleSprite.setTexture(appleTexture);

	if (simulator)
	{
		updateCellSize();
		updateSize(appleSprite, 975.0 / simulator->getMapSize() / 1080);
	}

	scoreOrigin.x = window->getSize().x * 0.568;
	scoreOrigin.y = window->getSize().y * 0.509;
}

bool Graphics::draw(bool displayGameOverOnEnd)
{
	window->clear();

	window->draw(bgSprite);

	if (!simulator)
	{
		window->display();
		return false;
	}

	drawSnakes();
	bool ret = drawScore();
	drawGameSpeed();
	drawGeneration();
	if (!ret && displayGameOverOnEnd)
		drawGameOver();

	window->display();
	return ret;
}

void Graphics::setSimulator(Simulator* simulator)
{
	this->simulator = simulator;

	if (simulator)
	{
		updateCellSize();
		updateSize(appleSprite, 975.0 / simulator->getMapSize() / 1080);
	}
}

void Graphics::setGameSpeedTracker(unsigned int* speed)
{
	gameSpeed = speed;
}

void Graphics::setGenerationTracker(unsigned int* generation)
{
	this->generation = generation;
}

bool Graphics::pollEvent(sf::Event& event)
{
	bool ret = true;
	bool forward = false;

	while (ret && !forward)
	{
		ret = window->pollEvent(event);

		switch (event.type)
		{
		case(sf::Event::Closed):
			window->close();
			break;
		default:
			forward = true;
			break;
		}
	}

	return ret;
}

bool Graphics::isOpen()
{
	return window->isOpen();
}

Graphics::~Graphics()
{
	delete window;
}
