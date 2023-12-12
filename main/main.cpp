#include<SFML/Graphics.hpp>
#include"../Interface/ScreenController.hpp"
#include"../Interface/InputHandler.hpp"

int main()
{
    Screen sc;
    InputHandler input{&sc};

	while (sc.Open())
	{
        input.GetInput();
        sc.Update();
    }
    
    return 0;
}