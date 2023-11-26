//#include"ScreenController.hpp"
#include<SFML\Graphics.hpp>
#include"Button.cpp"
#include"../EdgeAndPoint/Edge_Point.cpp"
#include<cstdint>
#include<vector>
#include<iostream>

using u32 = uint32_t;

class Screen
{
public:
    Screen()
    : window(sf::RenderWindow{sf::VideoMode(WIDTH, HEIGHT), "Grafi"}) {LoadMenu();}
    ~Screen() {DeleteButtons();}

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
    void DeleteButtons()
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

    const u32 WIDTH = 600;
    const u32 HEIGHT = 600;

    sf::RenderWindow window;

    sf::Vector2i mouse_pos;
    sf::Event event;

    std::vector<Button*> buttons;
    std::vector<Point*> points;
    std::vector<Edge*> edges;
};

void Screen::Close()
{
    DeleteButtons();
    window.close();
}

void Screen::LoadMenu()
{
    DeleteButtons();

    sf::RectangleShape* bb = new sf::RectangleShape(sf::Vector2f(100, 100));
    bb->setFillColor(sf::Color(255, 255, 0));
    bb->setOrigin(sf::Vector2f(50, 50));
    bb->setPosition(sf::Vector2f(WIDTH/2, HEIGHT/2 - 60));

    sf::RectangleShape* bb1 = new sf::RectangleShape(sf::Vector2f(100, 100));
    bb1->setFillColor(sf::Color(255, 255, 0));
    bb1->setOrigin(sf::Vector2f(50, 50));
    bb1->setPosition(sf::Vector2f(WIDTH/2, HEIGHT/2 + 60));

    buttons.push_back(new Button(bb, this, &Close));
    buttons.push_back(new Button(bb1, this, &LoadScene1));
}

void Screen::LoadScene1()
{
    DeleteButtons();

    sf::RectangleShape* bb = new sf::RectangleShape(sf::Vector2f(100, 100));
    bb->setFillColor(sf::Color(255, 255, 0));
    bb->setOrigin(sf::Vector2f(50, 50));
    bb->setPosition(sf::Vector2f(WIDTH/2, 50));

    buttons.push_back(new Button(bb, this, &LoadMenu));

    
    sf::CircleShape* bb1 = new sf::CircleShape(10);
    bb1->setFillColor(sf::Color(255, 0, 150));
    bb1->setPosition(sf::Vector2f(100, 100));
    points.push_back(new Point(0, bb1));

    sf::CircleShape* bb2 = new sf::CircleShape(10);
    bb2->setFillColor(sf::Color(255, 0, 150));
    bb2->setPosition(sf::Vector2f(200, 200));
    points.push_back(new Point(1, bb2));

    sf::CircleShape* bb3 = new sf::CircleShape(10);
    bb3->setFillColor(sf::Color(255, 0, 150));
    bb3->setPosition(sf::Vector2f(500, 300));
    points.push_back(new Point(2, bb3));

    edges.push_back(new Edge(points[0], points[1], 10));
    edges.push_back(new Edge(points[1], points[2], 100));
    edges.push_back(new Edge(points[0], points[2], 50));
}

void Screen::Update()
{
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            Close();
        }

        if (event.type == sf::Event::MouseButtonPressed
            && event.mouseButton.button == sf::Mouse::Left)
        {
            bool found = false;
            mouse_pos = sf::Mouse::getPosition(window);
            for (u32 i = 0; i < buttons.size(); i++)
            {
                if (buttons[i]->Shape()->getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
                {
                    buttons[i]->Call();
                    found = true;
                    break;
                }
            }

            if (found) {break;}

            for (u32 i = 0; i < points.size(); i++)
            {
                if (points[i]->Shape()->getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
                {
                    points[i]->FindPath(edges, points.size());
                    break;
                }
            }
        }
    }
    window.clear(sf::Color::White);

    for (u32 i = 0; i < buttons.size(); i++)
    {
        window.draw(*buttons[i]->Shape());
    }
    for (u32 i = 0; i < points.size(); i++)
    {
        window.draw(*points[i]->Shape());
    }

    window.display();
}