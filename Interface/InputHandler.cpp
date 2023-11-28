#include<SFML\Graphics.hpp>
#include"InputHandler.hpp"
#include"../EdgeAndPoint/Edge_Point.hpp"

void InputHandler::GetInput()
{
    while (sc->window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            sc->Close();
            return;
        }

        if (event.type == sf::Event::MouseButtonPressed
            && event.mouseButton.button == sf::Mouse::Left)
        {
            clock.restart();
            mouse_prev_pos = sf::Mouse::getPosition(sc->window);
            return;
        }

        if (event.type == sf::Event::MouseButtonReleased
            && event.mouseButton.button == sf::Mouse::Left)
        {
            dt = clock.getElapsedTime().asSeconds();
            if (dt < CLICK_TIME)
            {
                ActicatePressed();
            }
            return;
        }

        if (event.type == sf::Event::MouseMoved)
        {
            Move();
        }
        if (event.type == sf::Event::MouseWheelScrolled)
        {
            Scroll(event.mouseWheelScroll.delta);
        }
    }
}

void InputHandler::ActicatePressed()
{
    bool flag = false;
    mouse_pos = sf::Mouse::getPosition(sc->window);
    for (u32 i = 0; i < sc->buttons.size(); i++)
    {
        if (sc->buttons[i]->Shape()->getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
        {
            sc->buttons[i]->Call();
            flag = true;
            return;
        }
    }

    if (flag) {return;}

    for (u32 i = 0; i < sc->points.size(); i++)
    {
        if (sc->points[i]->Shape()->getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
        {
            if (origin_waypoint < 0)
            {
                origin_waypoint = i;
                for (u32 j = 0; j < way.size(); j++)
                {
                    sc->edges[way[j]]->TurnOff();
                }
                way.clear();
                return;
            }
            if (origin_waypoint == i) {return;}
            way = sc->points[origin_waypoint]->FindPath(sc->edges, sc->points.size(), i);
            origin_waypoint = -1;
            return;
        }
    }
}

void InputHandler::Move()
{
    mouse_pos = sf::Mouse::getPosition(sc->window);

    sc->view.move(current_zoom * (mouse_pos.x - mouse_prev_pos.x),
        current_zoom * (mouse_pos.y - mouse_prev_pos.y));
}

void InputHandler::Scroll(float scroll)
{
    // float tmp = current_zoom;
    // if (scroll > 0)
    // {
    //     tmp -= scroll;
    // }
    // current_zoom = 
    // event.mouseWheelScroll.y
    std::cerr << scroll << '\n';
}