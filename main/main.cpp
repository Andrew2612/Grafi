#include<SFML/Graphics.hpp>
#include"../Interface/ScreenController.cpp"
#include<time.h>
#include<cstdint>
#include<iostream>

using u32 = uint32_t;

int main()
{
    std::cerr << "=======================================\n";
    sf::Clock clock;

    Screen sc;

    float dt;
	while (sc.Open())
	{
        sc.Update();

		dt = clock.getElapsedTime().asMicroseconds();

		clock.restart();
    }
    
    return 0;
}