#include<SFML\Graphics.hpp>
#include<cstdint>
#include<vector>

using u32 = uint32_t;

class Screen
{
public:
    Screen() : window(sf::RenderWindow{sf::VideoMode(WIDTH, HEIGHT), "Grafi"}) {}
    bool Open() const {return window.isOpen();}

    void LoadMenu()
    {
        sf::RectangleShape BeginButton(sf::Vector2f(100, 100));
        BeginButton.setFillColor(sf::Color(0, 255, 0));
        BeginButton.setOrigin(sf::Vector2f(50, 50));
        BeginButton.setPosition(sf::Vector2f(WIDTH/2, HEIGHT/2));

        buttons.push_back(BeginButton);
    }
    void Update()
    {
        window.clear(sf::Color::White);

        sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
            {
                window.close();
            }
		}
        for (u32 i = 0; i < buttons.size(); i++)
        {
            window.draw(buttons[i]);
        }
    }

private:
    const u32 WIDTH = 450;
    const u32 HEIGHT = 850;

    sf::RenderWindow window;

    std::vector<sf::RectangleShape> buttons;
};
