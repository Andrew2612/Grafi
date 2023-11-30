#ifndef Grafi__Screen_
#define Grafi__Screen_

#include<SFML\Graphics.hpp>
#include"Button.hpp"
#include"InputHandler.hpp"
#include"../EdgeAndPoint/Edge_Point.hpp"
#include<cstdint>
#include<vector>

using u32 = uint32_t;

class Screen
{
public:
    Screen()
    : window(sf::RenderWindow{sf::VideoMode(WIDTH, HEIGHT), "Grafi"}) {LoadMenu();}

    ~Screen() {DeleteObjects();}

    Screen(const Screen& s) = delete;
    Screen& operator=(const Screen& s) = delete;
    Screen(Screen&& s) = delete;
    Screen& operator=(Screen&& s) = delete;

    bool Open() const {return window.isOpen();}

    void Close();

    void LoadMenu();

    void LoadScene1();

    void Update();    

private:
    friend class InputHandler;
    void CreateButton(sf::Vector2f pos, void (Screen::*f)());

    void CreatePoint(sf::Vector2f pos);

    void DeleteObjects();

    const u32 WIDTH = 600;
    const u32 HEIGHT = 600;

    sf::RenderWindow window;
    sf::View view{sf::FloatRect(0, 0, WIDTH, HEIGHT)};
    sf::View default_view = window.getDefaultView();

    std::vector<Button*> buttons;
    std::vector<Point*> points;
    std::vector<Edge*> edges;

    InputHandler input = InputHandler(this);
};

#endif
