#include "Config.h"
#include <stdexcept>
#include <iostream>

//learningType
std::string Config::learningType = "SARL"; // SARL, MARL_cooperative, MARL_competitive, MARL_mixed

// general
std::string Config::mode = "learn";	   // learn || play
unsigned int Config::snakesPerSim = 2; // max 4 (treba provjeriti prilikom ucitavanja)
unsigned int Config::mapSize = 20;
unsigned int Config::snakeInitSize = 4;

// play settings
float Config::playSpeed = 1.0;
std::string Config::playModePlayer1 = "./saves/snake_NN.save";	// Human, Hamilton or path to saved model
std::string Config::playModePlayer2 = "./saves/snake_CGP.save"; // snakes per sim odreduje broj igraca
std::string Config::playModePlayer3 = "Human";
std::string Config::playModePlayer4 = "Human";

// learn settings
unsigned int Config::populationSize = 100;
std::string Config::selectionType = "Cutoff";
unsigned int Config::selectionSize = 5;
std::string Config::crossType = "default";
float Config::mutationChance = 0.009;
std::string Config::AIModel = "CGP";

// NN
unsigned int Config::hiddenLayerDepth = 1;
unsigned int Config::hiddenLayerWidth = 10;
std::string Config::activationFunction = "tanh";

// CGP
int Config::numOfRows = 6;
int Config::numOfCols = 2;
int Config::numOfFunctions = 4; // min 1, max 6
int Config::numOfFunctionArgs = 2;

// GP
int Config::maxDepth = 6;
// int Config::numOfFunctions - dijeli i s CGP-om za sad; //min 1, max 6

Config::Config(std::string path) : Saveable(path)
{
	// sve varijable treba ucitati pomocu metoda load(ime, varijabla)
	// try catch je potreban jer neka varijabla moze ne postojati u save fileu
	try
	{
		std::string lt = "";
		load("learningType", lt);
		if (lt != "SARL" && lt != "MARL_cooperative" && lt != "MARL_competitive" && lt != "MARL_mixed")
			throw std::invalid_argument("Invalid learning type.");
		learningType = lt;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "learning tyype not found, using default value: " << learningType << std::endl;
	}

	try
	{
		std::string md = "";
		load("mode", md);
		if (md != "learn" && md != "play")
			throw std::invalid_argument("Invalid mode.");
		mode = md;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "mode not found, using default value: " << mode << std::endl;
	}
	
	try
	{
		int ms = 0;
		load("mapSize", ms);
		mapSize = ms;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "mapSize not found, using default value: " << mapSize << std::endl;
	}

	try
	{
		int sps = 0;
		load("snakesPerSim", sps);
		snakesPerSim = sps;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "SnakesPerSim not found, using default value: " << snakesPerSim << std::endl;
	}

	try
	{
		int sis = 0;
		load("snakesInitSize", sis);
		snakeInitSize = sis;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "SnakesInitSize not found, using default value: " << snakeInitSize << std::endl;
	}

	try
	{
		float speed = 0;
		load("playSpeed", speed);
		playSpeed = speed;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "PlaySpeed not found, using default value: " << playSpeed << std::endl;
	}

	try
	{
		std::string mode = "";
		load("playModePlayer1", mode);
		if (mode != "Human" && mode != "Hamilton" && mode.substr(0, 8) != "./saves/")
			throw std::invalid_argument("Invalid mode.");
		playModePlayer1 = mode;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "PlayModePlayer1 not found, using default value: " << playModePlayer1 << std::endl;
	}

	try
	{
		std::string mode = "";
		load("playModePlayer2", mode);
		if (mode != "Human" && mode != "Hamilton" && mode.substr(0, 8) != "./saves/")
			throw std::invalid_argument("Invalid mode.");
		playModePlayer2 = mode;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "PlayModePlayer2 not found, using default value: " << playModePlayer2 << std::endl;
	}

	try
	{
		std::string mode = "";
		load("playModePlayer3", mode);
		if (mode != "Human" && mode != "Hamilton" && mode.substr(0, 8) != "./saves/")
			throw std::invalid_argument("Invalid mode.");
		playModePlayer3 = mode;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "PlayModePlayer3 not found, using default value: " << playModePlayer3 << std::endl;
	}

	try
	{
		std::string mode = "";
		load("playModePlayer4", mode);
		if (mode != "Human" && mode != "Hamilton" && mode.substr(0, 8) != "./saves/")
			throw std::invalid_argument("Invalid mode.");
		playModePlayer4 = mode;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "PlayModePlayer4 not found, using default value: " << playModePlayer4 << std::endl;
	}

	try
	{
		int pop = 0;
		load("populationSize", pop);
		populationSize = pop;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "PopulationSize not found, using default value: " << populationSize << std::endl;
	}

	try
	{
		std::string selType = "";
		load("selectionType", selType);
		if (selType != "Cutoff" && selType != "Geometric")
			throw std::invalid_argument("Invalid selection type.");
		selectionType = selType;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "SelectionType not found, using default value: " << selectionType << std::endl;
	}

	try
	{
		int cutSize = 0;
		load("selectionSize", cutSize);
		selectionSize = cutSize;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "selectionSize not found, using default value: " << selectionSize << std::endl;
	}

	try
	{
		std::string crType = "";
		load("crossType", crType);
		if (crType != "default" && crType != "disable")
			throw std::invalid_argument("Invalid cross type.");
		crossType = crType;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "CrossType not found, using default value: " << crossType << std::endl;
	}

	try
	{
		float mutChance = 0;
		load("mutationChance", mutChance);
		mutationChance = mutChance;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "MutationChance not found, using default value: " << mutationChance << std::endl;
	}

	try
	{
		std::string model = "";
		load("AIModel", model);
		if (model != "NN" && model != "GP" && model != "CGP")
			throw std::invalid_argument("Invalid model.");
		AIModel = model;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "AIModel not found, using default value: " << AIModel << std::endl;
	}

	try
	{
		int hiddenDepth = 0;
		load("hiddenLayerDepth", hiddenDepth);
		hiddenLayerDepth = hiddenDepth;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "HiddenLayerDepth not found, using default value: " << hiddenLayerDepth << std::endl;
	}

	try
	{
		int hiddenWidth = 0;
		load("hiddenLayerWidth", hiddenWidth);
		hiddenLayerWidth = hiddenWidth;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "HiddenLayerWidth not found, using default value: " << hiddenLayerWidth << std::endl;
	}

	try
	{
		std::string function = "";
		load("activationFunction", function);
		if (function != "binary step" && function != "linear" && function != "sigmoid" && function != "tanh" && function != "relu")
			throw std::invalid_argument("Invalid function.");
		activationFunction = function;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "ActivationFunction not found, using default value: " << activationFunction << std::endl;
	}
	try
	{
		int cgp = 0;
		load("numOfRows", cgp);
		numOfRows = cgp;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "NumOfRows not found, using default value: " << numOfRows << std::endl;
	}

	try
	{
		int cgp = 0;
		load("numOfCols", cgp);
		numOfCols = cgp;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "NumOfCols not found, using default value: " << numOfCols << std::endl;
	}

	try
	{
		int cgp = 0;
		load("numOfFunctions", cgp);
		numOfFunctions = cgp;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "NumOfFunctions not found, using default value: " << numOfFunctions << std::endl;
	}

	try
	{
		int cgp = 0;
		load("numOfFunctionArgs", cgp);
		numOfFunctionArgs = cgp;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "NumOfFunctionArgs not found, using default value: " << numOfFunctionArgs << std::endl;
	}

	try
	{
		int gp = 0;
		load("maxDepth", gp);
		maxDepth = gp;
	}
	catch (std::invalid_argument err)
	{
		std::cout << "MaxDepth not found, using default value: " << maxDepth << std::endl;
	}
}

void Config::saveData()
{
	// Nema smisla spremati konfiguraciju jer je nije moguce mijenjati tijekom izvodenja
	// iako je saveable to nam je potrebno samo za ucitavanje
	throw std::runtime_error("Not implemented");
}