#include<SFML\Graphics.hpp>
#include"InputHandler.hpp"
#include"../EdgeAndPoint/Edge_Point.hpp"

void InputHandler::GetInput()
{
    mouse_pos = sc->view.getCenter() - current_zoom *(sc->SCREEN_CENTER - static_cast<sf::Vector2f>(sf::Mouse::getPosition(sc->window)));
    current_point_index = GetPointTargetIndex();

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
            
            continue;
        }

        if (event.type == sf::Event::MouseButtonReleased
            && event.mouseButton.button == sf::Mouse::Left)
        {
            dt = clock.getElapsedTime().asSeconds();
            if (dt < CLICK_TIME)
            {
                OnClick();
            }
            continue;
        }

        if (event.type == sf::Event::MouseMoved)
        {
            sc->SetPointLabel(current_point_index);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                Move();
            }
        }
        if (event.type == sf::Event::MouseWheelScrolled)
        {
            Scroll(event.mouseWheelScroll.delta);
        }
    }
    mouse_prev_pos = mouse_pos;
}


int InputHandler::GetPointTargetIndex()
{
    for (u32 i = 0; i < sc->points.size(); i++)
    {
        if (sc->points[i]->Shape()->getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
        {
            return i;
        }
    }
    return -1;
}

void InputHandler::OnClick()
{
    if (current_point_index < 0)
    {
        for (u32 i = 0; i < sc->buttons.size(); i++)
        {
            if (sc->buttons[i]->Shape()->getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
            {
                sc->buttons[i]->OnClick();
                return;
            }
        }
        return;
    }
    else
    {
        if (origin_waypoint < 0)
        {
            origin_waypoint = current_point_index;
            for (u32 j = 0; j < way.size(); j++)
            {
                sc->edges[way[j]]->TurnOff();
            }
            way.clear();
            return;
        }
        if (origin_waypoint == current_point_index) {return;}
        way = sc->points[origin_waypoint]->FindPath(sc->edges, sc->points.size(), current_point_index);
        origin_waypoint = -1;
        return;
    }
}

void InputHandler::Move()
{
    sc->view.setCenter(sc->view.getCenter() + current_zoom * (mouse_prev_pos - mouse_pos));
}

void InputHandler::Scroll(int scroll)
{
    if (scroll > 0 && current_zoom * ZOOM_SPEED > MIN_ZOOM)
    {
        sc->view.zoom(ZOOM_SPEED);
        current_zoom *= ZOOM_SPEED;
    }
    if (scroll < 0 && current_zoom / ZOOM_SPEED < MAX_ZOOM)
    {
        sc->view.zoom(1/ZOOM_SPEED);
        current_zoom /= ZOOM_SPEED;
    }
}
