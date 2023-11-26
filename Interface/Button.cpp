//#include"Button.hpp"
#include<SFML\Graphics.hpp>
#include"ScreenController.hpp"
#include<functional>

class Button
{
private:
    Screen* sc;
    std::function<void(Screen*)> func_ptr;
    sf::Shape* shape;
public:
    Button(sf::Shape* shape_, Screen* screen, void (Screen::*func)())
    : shape(shape_), sc(screen), func_ptr(func) {}

    ~Button() {delete shape;}

    Button(const Button& b) = delete;
    Button& operator=(const Button& b) = delete;
    Button(Button&& b) = delete;
    Button& operator=(Button&& b) = delete;

    sf::Shape* Shape() const
    {
        return shape;
    }

    void Call()
    {
        func_ptr(sc);
    }
};
