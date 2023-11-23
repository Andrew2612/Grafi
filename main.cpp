#include<SFML/Graphics.hpp>
#include<time.h>
#include<cstdint>
#include<iostream>

using u32 = uint32_t;

int main()
{
    std::cerr << "=======================================\n";
    sf::Clock clock;

    AppState& app_state = AppState::GetInstance();

    float dt;
	while (app_state.Open())
	{
        app_state.Update();

		dt = clock.getElapsedTime().asMicroseconds();

		clock.restart();
    }
    
    return 0;
}