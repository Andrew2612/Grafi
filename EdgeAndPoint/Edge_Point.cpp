#include"Edge_Point.hpp"
#include<SFML\Graphics.hpp>
#include<string>
#include<cmath>
#include<iostream>
#include<vector>
#include<cstdint>

using u32 = uint32_t;

std::vector<u32> Point::FindPath(const std::vector<Edge*>& edges, u32 num_of_points, u32 destination_num) const
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

            if (edges[j]->SingleSided()) {std::cerr << "Baddd\n"; continue;}

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
    line.setSize(sf::Vector2f(length, 2));

    float angle = std::acos(way.x/length);
    if (way.y < 0) {angle = -angle;}

    line.setPosition(ori->Shape()->getPosition());
    line.setRotation(180 * angle/3.141f);

    line.setFillColor(sf::Color(0, 155, 120));
};
