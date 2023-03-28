#include "Config.h"
#include "Initializer.h"

int main(void)
{
    Config c("./zmijica.config");

	if (Config::mode == "learn" && Config::learningType == "SARL")
		Initializer::initializeLearn();
	else if (Config::mode == "play")
		Initializer::initializePlay();
	else if (Config::learningType == "MARL_cooperative")
		Initializer::initializeCooperativePlay();

	return 0;
}