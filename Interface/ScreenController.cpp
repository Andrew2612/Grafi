#include<SFML\Graphics.hpp>
#include<cstdint>
#include<vector>
#include<iostream>

using u32 = uint32_t;

class Screen
{
public:
    Screen() : window(sf::RenderWindow{sf::VideoMode(WIDTH, HEIGHT), "Grafi"}) {}
    bool Open() const {return window.isOpen();}

    void LoadMenu()
    {
        sf::RectangleShape BeginButton(sf::Vector2f(100, 100));
        BeginButton.setFillColor(sf::Color(255, 255, 0));
        BeginButton.setOrigin(sf::Vector2f(50, 50));
        BeginButton.setPosition(sf::Vector2f(WIDTH/2, HEIGHT/2));

        buttons.push_back(BeginButton);
    }
    void Update()
    {
        sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
            {
                window.close();
            }
		}

        window.clear(sf::Color::White);

        for (u32 i = 0; i < buttons.size(); i++)
        {
            window.draw(buttons[i]);
        }

        window.display();
    }

private:
    const u32 WIDTH = 600;
    const u32 HEIGHT = 600;

    sf::RenderWindow window;

    std::vector<sf::RectangleShape> buttons;
};
