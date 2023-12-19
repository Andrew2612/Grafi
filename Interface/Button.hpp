#ifndef Grafi__Button_
#define Grafi__Button_

#include<SFML\Graphics.hpp>
#include<functional>

template<class T>
class Button
{
private:
    T* owner;

    std::function<void(T*)> func_ptr;
    sf::Shape* shape;
    sf::Text text;
public:
    Button(const sf::Vector2f pos, T* o, void (T::*func)(), const std::string& t, sf::Font& font)
    : owner(o), func_ptr(func), text({t, font})
    {
        float scale = t.length() * text.getCharacterSize()/1.6f;
        shape = new sf::RectangleShape({scale, 50});

        shape->setOrigin(sf::Vector2f(scale/2, 25));
        shape->setPosition(pos);
        shape->setFillColor({0,0,0,0});

        text.setFillColor(sf::Color::Blue);
        text.setPosition(pos.x - scale/2, pos.y - 15);
    }

    ~Button() {delete shape;}

    Button(const Button& b) = delete;
    Button& operator=(const Button& b) = delete;
    Button(Button&& b) = delete;
    Button& operator=(Button&& b) = delete;

    const sf::Shape* Shape() const
    {
        return shape;
    }

    const sf::Text& Text() const {return text;}

    void OnClick()
    {
        func_ptr(owner);
    }
};

#endif
