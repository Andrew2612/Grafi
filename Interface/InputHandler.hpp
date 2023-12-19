#ifndef Grafi__Input_
#define Grafi__Input_

#include<SFML\Graphics.hpp>
#include"ScreenController.hpp"
#include<vector>
#include<cstdint>
#include<time.h>

using u32 = uint32_t;
using i32 = int32_t;

class Screen;

class InputHandler
{
private:
    std::vector<u32> way;
    i32 origin_waypoint = -1;
    bool from_place = false;
    bool to_place = false;

    Screen* sc;

    sf::Event event;
    sf::Vector2i window_mouse_pos;
    sf::Vector2f mouse_prev_pos;
    sf::Vector2f mouse_pos;
    i32 current_point_index;
    i32 current_place_index;

    sf::Clock clock;
    const float CLICK_TIME = 0.3f;

    float current_zoom = 1;
    const float ZOOM_SPEED = 0.95;

    i32 GetPointTargetIndex(bool for_point);
    void OnClick();
    void Move();
    void Scroll(i32 scroll);
public:
    InputHandler(Screen* screen) : sc(screen) {}

    InputHandler(const InputHandler& i) = delete;
    InputHandler& operator=(const InputHandler& i) = delete;
    InputHandler(InputHandler&& i) = delete;
    InputHandler& operator=(InputHandler&& i) = delete;

    void GetInput();
};

#endif