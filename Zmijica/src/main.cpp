#include "Config.h"
#include "Initializer.h"

int main(void)
{
    Config c("./zmijica.config");

	if (Config::mode == "learn")
		Initializer::initializeLearn();
	else if (Config::mode == "play")
		Initializer::initializePlay();

	return 0;
}