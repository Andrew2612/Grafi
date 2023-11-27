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
    void CreateButton(sf::Vector2f pos, void (Screen::*f)());

    void CreatePoint(sf::Vector2f pos);

    void DeleteObjects()
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

    int origin = -1;
};

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
                    if (origin < 0) {origin = i; std::cerr << "Origin " << i << '\n';break;}

                    points[origin]->FindPath(edges, points.size(), i);
                    origin = -1;
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
