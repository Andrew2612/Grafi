#include<SFML\Graphics.hpp>
#include"InputHandler.hpp"
#include"ScreenController.hpp"
#include"../EdgeAndPoint/Edge_Point.hpp"

void InputHandler::GetInput()
{
    window_mouse_pos = sf::Mouse::getPosition(sc->window);
    mouse_pos = sc->view.getCenter() - current_zoom *(sc->SCREEN_CENTER 
        - static_cast<sf::Vector2f>(window_mouse_pos));
    if (sc->map) {current_point_index = GetPointTargetIndex(true);
                  current_place_index = GetPointTargetIndex(false);}
    else {current_point_index = -1; current_place_index = -1;}

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
            if (current_point_index > -1) {sc->SetPointLabel(current_point_index, true);}
            else {sc->SetPointLabel(current_place_index, false);}

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


int InputHandler::GetPointTargetIndex(bool for_point)
{
    if (for_point)
    {
        for (u32 i = 0; i < sc->map->points.size(); i++)
        {
            if (sc->map->points[i]->Shape()->getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
            {
                return i;
            }
        }
    }
    else
    {
        for (u32 i = 0; i < sc->map->places.size(); i++)
        {
            if (sc->map->places[i]->Shape()->getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
            {
                return i;
            }
        }
    }
    return -1;
}

void InputHandler::OnClick()
{
    if (current_point_index < 0 && current_place_index < 0)
    {
        if (to_place)
        {
            delete sc->map->edges[sc->map->edges.size() - 1];
            sc->map->edges.pop_back();
            to_place = false;
        }
        if (from_place)
        {
            delete sc->map->edges[sc->map->edges.size() - 1];
            sc->map->edges.pop_back();
            from_place = false;
        }

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
            if (current_point_index > -1) {origin_waypoint = current_point_index; from_place = false;}
            else {origin_waypoint = current_place_index; from_place = true;}

            for (u32 j = 0; j < way.size(); j++)
            {
                sc->map->edges[way[j]]->TurnOff();
            }
            way.clear();
            sc->way_time_label.setString("");
            return;
        }

        if (current_place_index > -1)
        {current_point_index = sc->map->places[current_place_index]->PointNumber(); to_place = true;}

        if (current_point_index > -1)
        {
            if (from_place)
            {
                if (sc->map->places[origin_waypoint]->PointNumber() == current_point_index) {return;}    
            }
            else if (origin_waypoint == current_point_index) {return;}

            if (from_place)
            {
                sc->map->AddEdge(origin_waypoint, sc->map->places[origin_waypoint]->PointNumber(), sc->map->scale);
                origin_waypoint = sc->map->places[origin_waypoint]->PointNumber();
            }
            if (to_place) {sc->map->AddEdge(current_place_index, current_point_index, sc->map->scale);}

            way = sc->map->points[origin_waypoint]->FindPath(sc->map->edges, sc->map->points.size(), current_point_index);

            u32 t = way[way.size()-1];
            way.pop_back();

            if (to_place)
            {
                sc->map->edges[sc->map->edges.size()-1]->TurnOn();
                t += sc->map->edges[sc->map->edges.size()-1]->WayTime();
            }
            if (from_place) {
                u32 n;
                if (to_place) {n = sc->map->edges.size() - 2;}
                else {n = sc->map->edges.size() - 1;}

                sc->map->edges[n]->TurnOn();
                t += sc->map->edges[n]->WayTime();
            }
            sc->way_time_label.setString("The Travel Time will be: " + std::to_string(t) + "min");
        
            origin_waypoint = -1;
            return;
        }
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
