#ifndef Grafi__Input_
#define Grafi__Input_

#include<SFML\Graphics.hpp>
#include"ScreenController.hpp"
#include<vector>
#include<cstdint>

using u32 = uint32_t;

class InputHandler
{
private:
    std::vector<u32> way;
    int origin_waypoint = -1;

    sf::Vector2i mouse_pos;
    sf::Event event;

    Screen* sc;
public:
    InputHandler(Screen* screen) : sc(screen) {}

    InputHandler(const InputHandler& i) = delete;
    InputHandler& operator=(const InputHandler& i) = delete;
    InputHandler(InputHandler&& i) = delete;
    InputHandler& operator=(InputHandler&& i) = delete;

    void GetInput();
    void ActicatePathFinder();
};

#endif