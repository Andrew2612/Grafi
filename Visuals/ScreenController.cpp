#include<ScreneController.hpp>
#include<SFML/Graphics.hpp>
#include<cstdint>

using u32 = uint32_t;

class Screen
{
public:
    bool Open() const {return window.isOpen();}
    void GetClosingEvent() const
    {
        sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
    }
    void Update()
    {
        window.clear(sf::Color::White);

        //for (u32 i = 0; i < )
    }
private:
    const u32 WIDTH = 450;
    const u32 HEIGHT = 850;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Grafi");
};
