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
    Button(sf::Shape* shape_, T* o, void (T::*func)(), const std::string& t, sf::Font& font)
    : shape(shape_), owner(o), func_ptr(func), text({t, font})
    {
        text.setOrigin(shape->getOrigin());
        text.setPosition(shape->getPosition());
        text.setCharacterSize((shape->getLocalBounds().width) / text.getString().getSize());
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
