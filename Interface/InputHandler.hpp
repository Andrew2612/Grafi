#ifndef Grafi__Input_
#define Grafi__Input_

#include<SFML\Graphics.hpp>
#include"ScreenController.hpp"
#include<vector>
#include<cstdint>
#include<time.h>

using u32 = uint32_t;

class InputHandler
{
private:
    std::vector<u32> way;
    int origin_waypoint = -1;

    Screen* sc;

    sf::Event event;
    sf::Vector2i mouse_prev_pos;
    sf::Vector2i mouse_pos;
    sf::Clock clock;
    float dt;
    const float CLICK_TIME = 0.3f;

    float current_zoom = 1;

    void ActicatePressed();
    void Move();
    void Scroll(float scroll);
public:
    InputHandler(Screen* screen) : sc(screen) {}

    InputHandler(const InputHandler& i) = delete;
    InputHandler& operator=(const InputHandler& i) = delete;
    InputHandler(InputHandler&& i) = delete;
    InputHandler& operator=(InputHandler&& i) = delete;

    void GetInput();
};

#endif