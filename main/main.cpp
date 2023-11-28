#include<SFML/Graphics.hpp>
#include"../Interface/ScreenController.cpp"
#include"../Interface/InputHandler.cpp"
#include<iostream>

int main()
{
    std::cerr << "=======================================\n";
    Screen sc;
	while (sc.Open())
	{
        sc.Update();
    }
    
    return 0;
}