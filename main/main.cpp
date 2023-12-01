#include<SFML/Graphics.hpp>
#include"../Interface/ScreenController.cpp"
#include"../Interface/InputHandler.cpp"
#include<iostream>

int main()
{
    std::cerr << "=======================================\n";
    Screen sc;
    InputHandler input{&sc};

	while (sc.Open())
	{
        input.GetInput();
        sc.Update();
    }
    
    return 0;
}