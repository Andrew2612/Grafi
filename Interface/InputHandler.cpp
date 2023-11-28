#include<SFML\Graphics.hpp>
#include"InputHandler.hpp"
#include"../EdgeAndPoint/Edge_Point.hpp"

void InputHandler::ActicatePathFinder()
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

            way = sc->points[origin_waypoint]->FindPath(sc->edges, sc->points.size(), i);
            origin_waypoint = -1;
            return;
        }
    }
}