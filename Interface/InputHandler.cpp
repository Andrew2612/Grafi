#include<SFML\Graphics.hpp>
#include"InputHandler.hpp"
#include"ScreenController.hpp"
#include"../EdgeAndPoint/Edge_Point.hpp"

void InputHandler::GetInput()
{
    window_mouse_pos = sf::Mouse::getPosition(sc->window);
    mouse_pos = sc->view.getCenter() - current_zoom *(sc->SCREEN_CENTER 
        - static_cast<sf::Vector2f>(window_mouse_pos));
    if (sc->map) {current_point_index = GetPointTargetIndex();}
    else {current_point_index = -1;}

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
            if (clock.getElapsedTime().asSeconds() < CLICK_TIME)
            {
                OnClick();
            }
            continue;
        }
        if (!sc->map) {continue;}
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
    for (u32 i = 0; i < sc->map->points.size(); i++)
    {
        if (sc->map->points[i]->Shape()->getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
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
        for (u32 j = 0; j < way.size(); j++)
        {
            sc->map->edges[way[j]]->TurnOff();
        }
        way.clear();
        origin_waypoint = -1;
        sc->way_time_label.setString("");

        for (u32 i = 0; i < sc->buttons.size(); i++)
        {
            if (sc->buttons[i]->Shape()->getGlobalBounds().contains(window_mouse_pos.x, window_mouse_pos.y))
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
                sc->map->edges[way[j]]->TurnOff();
            }
            way.clear();
            sc->way_time_label.setString("");
            return;
        }
        if (origin_waypoint == current_point_index) {return;}
        way = sc->map->points[origin_waypoint]->FindPath(sc->map->edges, sc->map->points.size(), current_point_index);
        sc->way_time_label.setString("The Travel Time will be: " + std::to_string(way[way.size()-1]) + "min");
        way.pop_back();
        origin_waypoint = -1;
        return;
    }
}

void InputHandler::Move()
{
    sf::Vector2f new_center(sc->view.getCenter() + (mouse_prev_pos - mouse_pos));
    
    if (new_center.x + sc->SCREEN_CENTER.x * current_zoom > sc->map->width)
    {
        new_center.x = sc->map->width - sc->SCREEN_CENTER.x * current_zoom;
    }
    else if (new_center.x  - sc->SCREEN_CENTER.x * current_zoom < 0)
    {
        new_center.x = sc->SCREEN_CENTER.x * current_zoom;
    }

    if (new_center.y + sc->SCREEN_CENTER.y * current_zoom > sc->map->height)
    {
        new_center.y = sc->map->height - sc->SCREEN_CENTER.y * current_zoom;
    }
    else if (new_center.y - sc->SCREEN_CENTER.y * current_zoom < 0)
    {
        new_center.y = sc->SCREEN_CENTER.y * current_zoom;
    }
    sc->view.setCenter(new_center);
}

void InputHandler::Scroll(i32 scroll)
{
    if (scroll > 0 && current_zoom * ZOOM_SPEED > sc->map->zoom_min)
    {
        sc->view.zoom(ZOOM_SPEED);
        current_zoom *= ZOOM_SPEED;
    }
    if (scroll < 0 && current_zoom / ZOOM_SPEED < sc->map->zoom_max)
    {
        sc->view.zoom(1/ZOOM_SPEED);
        current_zoom /= ZOOM_SPEED;
    }
    Move();
}
