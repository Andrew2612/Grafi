#include"../Interface/ScreenController.cpp"

class AppState
{
private:
    enum State {Menu = 0, Scene1 = 1};
    State state;
    Screen sc;
public:
    AppState() : state(State::Menu)
    {
        ReturnToMenu();
    };
    void ReturnToMenu() {sc.LoadMenu();}

    void Update()
    {
        sc.Update();
    }

    bool Open() const {return sc.Open();}
};

