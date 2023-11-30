#include"ScreenController.hpp"
#include<SFML\Graphics.hpp>
#include"Button.hpp"
#include"../EdgeAndPoint/Edge_Point.cpp"
#include<cstdint>
#include<vector>
#include<iostream>

using u32 = uint32_t;

void Screen::DeleteObjects()
{
    for (u32 i = 0; i < buttons.size(); i++)
    {
        delete buttons[i];
    }
    buttons.clear();

    for (u32 i = 0; i < points.size(); i++)
    {
        delete points[i];
    }
    points.clear();

    for (u32 i = 0; i < edges.size(); i++)
    {
        delete edges[i];
    }
    edges.clear();
}

void Screen::Close()
{
    DeleteObjects();
    window.close();
}

void Screen::LoadMenu()
{
    DeleteObjects();
    CreateButton(sf::Vector2f(WIDTH/2, HEIGHT/2 - 60), &Close);
    CreateButton(sf::Vector2f(WIDTH/2, HEIGHT/2 + 60), &LoadScene1);
}

void Screen::LoadScene1()
{
    DeleteObjects();
    CreateButton(sf::Vector2f(WIDTH/2, 60), &LoadMenu);

    CreatePoint(sf::Vector2f(100, 100));
    CreatePoint(sf::Vector2f(200, 200));
    CreatePoint(sf::Vector2f(500, 300));

    edges.push_back(new Edge(points[0], points[1], 10));
    edges.push_back(new Edge(points[1], points[2], 100));
    edges.push_back(new Edge(points[0], points[2], 50));
}

void Screen::Update()
{
    input.GetInput();
    window.clear(sf::Color::White);
    window.setView(view);

    for (u32 i = 0; i < buttons.size(); i++)
    {
        window.draw(*buttons[i]->Shape());
    }
    for (u32 i = 0; i < points.size(); i++)
    {
        window.draw(*points[i]->Shape());
    }
    for (u32 i = 0; i < edges.size(); i++)
    {
        window.draw(*edges[i]->Shape());
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

void Screen::CreatePoint(sf::Vector2f pos)
{
    sf::CircleShape* shape = new sf::CircleShape(10);
    shape->setOrigin(sf::Vector2f(10.0f, 10.0f));
    shape->setFillColor(sf::Color(255, 0, 150));
    shape->setPosition(pos);
    points.push_back(new Point(points.size(), shape));
}

