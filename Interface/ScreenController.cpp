#include"ScreenController.hpp"
#include<SFML\Graphics.hpp>
#include"Button.hpp"
#include"../EdgeAndPoint/Edge_Point.cpp"
#include<cstdint>
#include<vector>
#include<string>

using u32 = uint32_t;

Screen::Screen()
: map(nullptr), window(sf::RenderWindow{sf::VideoMode(2*SCREEN_CENTER.x, 2*SCREEN_CENTER.y), "Grafi"})
{
    font.loadFromFile("arial.ttf");
    point_label.setFont(font);
    point_label.setCharacterSize(40);

    way_time_label.setFont(font);
    way_time_label.setFillColor(sf::Color::Yellow);
    way_time_label.setCharacterSize(30);
    way_time_label.setPosition({0, 2*SCREEN_CENTER.y - 45});

    LoadMenu();
}

void Screen::DeleteObjects()
{
    if (map) {delete map; map = nullptr;}
    for (u32 i = 0; i < buttons.size(); i++)
    {
        delete buttons[i];
    }
    buttons.clear();
}

void Screen::Close()
{
    DeleteObjects();
    window.close();
}

void Screen::LoadMenu()
{
    DeleteObjects();
    CreateButton({SCREEN_CENTER.x, SCREEN_CENTER.y - 60}, &Close, "Exit");
    CreateButton({SCREEN_CENTER.x, SCREEN_CENTER.y + 60}, &LoadMap1, "First Map");
}


void Screen::LoadMapInterface()
{
    CreateButton({70, 50}, &LoadMenu, "Menu");
    CreateButton({2*SCREEN_CENTER.x - 70, 50}, &Close, "Exit");
}


void Screen::LoadMap1()
{
    DeleteObjects();
    LoadMapInterface();
    map = new Map(1);
}

void Screen::Update()
{
    if (map)
    {
        window.clear(sf::Color::Black);
        window.setView(view);
        window.draw(map->map_sprite);
        for (u32 i = 0; i < map->edges.size(); i++)
        {
            window.draw(*map->edges[i]->Shape());
        }
        for (u32 i = 0; i < map->points.size(); i++)
        {
            window.draw(*map->points[i]->Shape());
        }
        if (!point_label.getString().isEmpty())
        {
            window.draw(point_label);
        }
    } else {window.clear(sf::Color::White);}

    window.setView(default_view);
    for (u32 i = 0; i < buttons.size(); i++)
    {
        window.draw(*buttons[i]->Shape());
        window.draw(buttons[i]->Text());
    }
    if (!way_time_label.getString().isEmpty()) {window.draw(way_time_label);}

    window.display();
}

void Screen::CreateButton(const sf::Vector2f pos, void (Screen::*f)(), const std::string text)
{
    sf::RectangleShape* shape = new sf::RectangleShape({120, 60});
    shape->setFillColor(sf::Color::Blue);
    shape->setOrigin(sf::Vector2f(60, 30));
    shape->setPosition(pos);

    buttons.push_back(new Button(shape, this, f, text, font));
}

void Screen::SetPointLabel(int index)
{
    if (index < 0)
    {
        if (point_label.getString().isEmpty()) {return;}

        point_label.setString("");
        return;
    }

    point_label.setString(map->points[index]->Name());
    if (map->points[index]->Type() == Point::PointType::Metro)
    {
        point_label.setFillColor(sf::Color::Yellow);
    }
    else if (map->points[index]->Type() == Point::PointType::Street)
    {
        point_label.setFillColor({30, 144, 255});
    }
    point_label.setPosition(sf::Vector2f(-40, 15) + map->points[index]->Shape()->getPosition());
}