#include"ScreenController.hpp"
#include<SFML\Graphics.hpp>
#include"Button.hpp"
#include"../EdgeAndPoint/Edge_Point.cpp"
#include<cstdint>
#include<vector>
#include<string>
#include<iostream>

using u32 = uint32_t;

void Screen::DeleteObjects()
{
    if (map) {delete map;}
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
    CreateButton(sf::Vector2f(SCREEN_CENTER.x, SCREEN_CENTER.y - 60), &Close);
    CreateButton(sf::Vector2f(SCREEN_CENTER.x, SCREEN_CENTER.y + 60), &LoadMap1);
}

void Screen::LoadMap1()
{
    DeleteObjects();

    map = new Map(1);
}

void Screen::Update()
{
    window.clear(sf::Color::White);

    if (map)
    {
        window.setView(view);
        for (u32 i = 0; i < map->points.size(); i++)
        {
            window.draw(*map->points[i]->Shape());
        }
        for (u32 i = 0; i < map->edges.size(); i++)
        {
            window.draw(*map->edges[i]->Shape());
        }
        if (!point_label.getString().isEmpty())
        {
            window.draw(point_label);
        }
    }

    window.setView(default_view);
    for (u32 i = 0; i < buttons.size(); i++)
    {
        window.draw(*buttons[i]->Shape());
    }

    window.display();
}

void Screen::CreateButton(sf::Vector2f pos, void (Screen::*f)())
{
    sf::RectangleShape* shape = new sf::RectangleShape(sf::Vector2f(100, 100));
    shape->setFillColor(sf::Color(255, 255, 0));
    shape->setOrigin(sf::Vector2f(50, 50));
    shape->setPosition(pos);

    buttons.push_back(new Button(shape, this, f));
}

void Screen::SetPointLabel(int index)
{
    if (index < 0)
    {
        if (point_label.getString().isEmpty()) {return;}

        point_label.setString("");
        return;
    }

    point_label.setString(std::to_string(index));
    point_label.setPosition(sf::Vector2f(0, 15) + map->points[index]->Shape()->getPosition());
}