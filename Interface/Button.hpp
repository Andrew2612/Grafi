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
public:
    Button(sf::Shape* shape_, T* o, void (T::*func)())
    : shape(shape_), owner(o), func_ptr(func) {}

    ~Button() {delete shape;}

    Button(const Button& b) = delete;
    Button& operator=(const Button& b) = delete;
    Button(Button&& b) = delete;
    Button& operator=(Button&& b) = delete;

    sf::Shape* Shape() const
    {
        return shape;
    }

    void OnClick()
    {
        func_ptr(owner);
    }
};

#endif
