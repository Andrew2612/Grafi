#include"Edge_Point.hpp"
#include<SFML\Graphics.hpp>
#include<string>
#include<cmath>
#include<vector>
#include<cstdint>

using u32 = uint32_t;

Point::Point(const u32 num, const float posX, const float posY, 
        const std::string& name_, const PointType t)
        : point_number(num), name(name_), type(t)
{
    switch (t)
    {
    case PointType::Metro:
        shape = new sf::CircleShape(radius);
        shape->setOrigin(radius, radius);
        shape->setPosition(posX, posY);
        point_texture.loadFromFile("../EdgeAndPoint/Metro.jpg");
        shape->setTexture(&point_texture);
        break;
    
    case PointType::Street:
        shape = new sf::RectangleShape({2*radius, 2*radius});
        shape->setOrigin(radius, radius);
        shape->setPosition({posX - radius, posY - radius});
        point_texture.loadFromFile("../EdgeAndPoint/Street.png");
        shape->setTexture(&point_texture);
        break;
    case PointType::Place:
        shape = new sf::CircleShape(radius, radius);
        shape->setOrigin(radius/2, radius/2);
        shape->setPosition(posX, posY);
        point_texture.loadFromFile("../EdgeAndPoint/Place.jpg");
        shape->setTexture(&point_texture);
        break;
    }
}

std::vector<u32> Point::FindPath(const std::vector<Edge*>& edges, const u32 num_of_points, u32 destination_num) const
{
    float *distance = new float[num_of_points];
    for (int i = 0; i < num_of_points; i++)
    {
        distance[i] = INT_MAX;
    }
    distance[point_number] = 0;

    int *edge_to_prev = new int[num_of_points]{-1};

    for (u32 i = 0; i < num_of_points; i++)
    {
        for (u32 j = 0; j < edges.size(); j++)
        {
            if (distance[edges[j]->Origin()->PointNumber()] != INT_MAX
                && distance[edges[j]->Origin()->PointNumber()] + edges[j]->WayTime()
                    < distance[edges[j]->Destination()->PointNumber()])
            {
                distance[edges[j]->Destination()->PointNumber()] = 
                distance[edges[j]->Origin()->PointNumber()] + edges[j]->WayTime();
                edge_to_prev[edges[j]->Destination()->PointNumber()] = j;
            }

            if (edges[j]->SingleSided()) {continue;}

            if (distance[edges[j]->Destination()->PointNumber()] != INT_MAX
                && distance[edges[j]->Destination()->PointNumber()] + edges[j]->WayTime()
                    < distance[edges[j]->Origin()->PointNumber()])
            {
                distance[edges[j]->Origin()->PointNumber()] = 
                distance[edges[j]->Destination()->PointNumber()] + edges[j]->WayTime();
                edge_to_prev[edges[j]->Origin()->PointNumber()] = j;
            }
        }
    }

    u32 tmp = destination_num;
    std::vector<u32> way;
    while (tmp != point_number)
    {
        edges[edge_to_prev[tmp]]->TurnOn();
        way.push_back(edge_to_prev[tmp]);

        int d = edges[edge_to_prev[tmp]]->Origin()->PointNumber();
        if (tmp == d)
        {
            tmp = edges[edge_to_prev[tmp]]->Destination()->PointNumber();
        }
        else {tmp = d;}
    }
    way.push_back(distance[destination_num]);

    delete[] edge_to_prev;
    delete[] distance;
    return way;
}


Edge::Edge(Point* ori, Point* dest, const u32 scale, const bool singleSided)
    : destination(dest), origin(ori), single_sided(singleSided)
{
    line = sf::RectangleShape(ori->Shape()->getPosition());

    sf::Vector2f way = dest->Shape()->getPosition() - ori->Shape()->getPosition();
    float length = std::sqrt(way.x * way.x + way.y * way.y);
    line.setSize({length, width});

    if (origin->Type() == destination->Type())
    {
        if (origin->Type() == Point::PointType::Metro)
        {
            way_time = 60*length / (scale * METRO_SPEED);
        }
        else
        {
            way_time = 60*length / (scale * CAR_SPEED);
        }
    }
    else
    {
        way_time = 60*length / (scale * WALK_SPEED);
    }


    float angle = std::acos(way.x/length);
    if (way.y < 0) {angle = -angle;}

    line.setPosition(ori->Shape()->getPosition());
    line.setRotation(180 * angle/3.141f);

    line.setFillColor(sf::Color::Black);
};

void Edge::TurnOn()
{
    if (origin->Type() == destination->Type())
    {
        if (origin->Type() == Point::PointType::Metro)
        {
            line.setFillColor(sf::Color::Red);
        }
        else
        {
            line.setFillColor(sf::Color::Blue);
        }
    }
    else
    {
        line.setFillColor(sf::Color::Magenta);
    }
}

void Edge::TurnOff() {line.setFillColor(sf::Color::Black);}

