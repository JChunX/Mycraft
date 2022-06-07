#include <iostream>
#include "Application.h"
#include <filesystem>
int main()
{
	// print current directory
    std::cout << "Current directory: " << std::filesystem::current_path() << std::endl;
	Application app;
	app.Run();
	return 0;
}