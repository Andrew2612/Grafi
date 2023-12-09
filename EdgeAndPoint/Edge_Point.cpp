#include"Edge_Point.hpp"
#include<SFML\Graphics.hpp>
#include<string>
#include<cmath>
#include<iostream>
#include<vector>
#include<cstdint>

using u32 = uint32_t;

Point::Point(const int num, const float posX, const float posY, 
        const std::string& name_, const PointType t)
        : point_number(num), name(name_), type(t)
{
    switch (t)
    {
    case PointType::Metro:
        shape = new sf::CircleShape(radius);
        shape->setOrigin(radius, radius);
        shape->setFillColor(sf::Color::Green);
        shape->setPosition(posX, posY);
        break;
    
    case PointType::Street:
        shape = new sf::RectangleShape({2*radius, 2*radius});
        shape->setPosition({posX - radius, posY - radius});
        shape->setOrigin(radius, radius);
        shape->setFillColor(sf::Color::Magenta);
    }
}

std::vector<u32> Point::FindPath(const std::vector<Edge*>& edges, const u32 num_of_points, u32 destination_num) const
{
    int distance[num_of_points];
    for (u32 i = 0; i < num_of_points; i++)
    {
        distance[i] = INT_MAX;
    }
    distance[point_number] = 0;

    int edge_to_prev[num_of_points] = {-1};

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

    std::cerr << "Distance from Origin: "<< distance[destination_num] << '\n';
    std::vector<u32> way;
    while (destination_num != point_number)
    {
        edges[edge_to_prev[destination_num]]->TurnOn();
        way.push_back(edge_to_prev[destination_num]);

        int d = edges[edge_to_prev[destination_num]]->Origin()->PointNumber();
        if (destination_num == d)
        {
            destination_num = edges[edge_to_prev[destination_num]]->Destination()->PointNumber();
        }
        else {destination_num = d;}
    }
    return way;
}

Edge::Edge(Point* ori, Point* dest, const u32 way_time_, const bool singleSided)
    : destination(dest), origin(ori), way_time(way_time_), single_sided(singleSided)
{
    line = sf::RectangleShape(ori->Shape()->getPosition());

    sf::Vector2f way = dest->Shape()->getPosition() - ori->Shape()->getPosition();
    float length = std::sqrt(way.x * way.x + way.y * way.y);
    line.setSize({length, width});

    float angle = std::acos(way.x/length);
    if (way.y < 0) {angle = -angle;}

    line.setPosition(ori->Shape()->getPosition());
    line.setRotation(180 * angle/3.141f);

    line.setFillColor(sf::Color::Black);
};

void Edge::TurnOn()
{
    if (origin->Type() == Point::PointType::Metro)
    {
        if (destination->Type() == Point::PointType::Metro)
        {
            line.setFillColor(sf::Color::Green);
        }
        else
        {
            line.setFillColor(sf::Color::Blue);
        }
    }
    if (origin->Type() == Point::PointType::Street)
    {
        if (destination->Type() == Point::PointType::Street)
        {
            line.setFillColor(sf::Color::Magenta);
        }
        else
        {
            line.setFillColor(sf::Color::Blue);
        }
    }
}

void Edge::TurnOff() {line.setFillColor(sf::Color::Black);}