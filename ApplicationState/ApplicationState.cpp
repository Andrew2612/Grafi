#include"ApplicationState.hpp"
#include"../Interface/ScreenController.hpp"

class AppState
{
private:
    enum State {Menu = 0, Scene1 = 1};
    State state;
    Screen sc;
    AppState() : state(State::Menu)
    {
        ReturnToMenu();
    };
public:
    static AppState& GetInstance()
    {
        static AppState instance;
        return instance;
    }

    void ReturnToMenu() {GetInstance().sc.LoadMenu();}

    void Update()
    {
        GetInstance().sc.Update();
    }

    bool Open() const {return GetInstance().sc.Open();}
};

