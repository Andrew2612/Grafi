#include<SFML/Graphics.hpp>
#include"ApplicationState/ApplicationState.cpp"
#include<time.h>
#include<cstdint>
#include<iostream>

using u32 = uint32_t;

int main()
{
    std::cerr << "=======================================\n";
    sf::Clock clock;

    AppState app_state;

    float dt;
	while (app_state.Open())
	{
        app_state.Update();

		dt = clock.getElapsedTime().asMicroseconds();

		clock.restart();
    }
    
    return 0;
}