#ifndef Grafi__Screen_
#define Grafi__Screen_

#include<SFML\Graphics.hpp>
#include"Button.hpp"
#include"../EdgeAndPoint/Edge_Point.hpp"
#include"../Map/Map.cpp"
#include<cstdint>
#include<vector>

using u32 = uint32_t;

class Screen
{
public:
    Screen();

    ~Screen() {DeleteObjects();}

    Screen(const Screen& s) = delete;
    Screen& operator=(const Screen& s) = delete;
    Screen(Screen&& s) = delete;
    Screen& operator=(Screen&& s) = delete;

    bool Open() const {return window.isOpen();}

    void Close();

    void Update();    

private:
    friend class InputHandler;

    void LoadMenu();

    void LoadMapInterface();

    void LoadMap1();

    void CreateButton(const sf::Vector2f pos, void (Screen::*f)(), const std::string text);

    void CreatePoint(sf::Vector2f pos);

    void SetPointLabel(int index);

    void DeleteObjects();

    const sf::Vector2f SCREEN_CENTER{300, 300};

    sf::RenderWindow window;
    sf::View view{sf::FloatRect(0, 0, 2*SCREEN_CENTER.x, 2*SCREEN_CENTER.y)};
    sf::View default_view = window.getDefaultView();

    Map* map; 
    std::vector<Button<Screen>*> buttons;

    sf::Font font;
    sf::Text point_label;
    sf::Text way_time_label;
};

#endif
