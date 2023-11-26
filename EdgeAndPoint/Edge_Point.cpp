//#include"Edge_Point.hpp"
#include<SFML\Graphics.hpp>
#include<string>
#include<cmath>
#include<iostream>
#include<vector>
#include<cstdint>

using u32 = uint32_t;


class Point;

class Edge
{
public:
    Edge(Point* ori, Point* dest, const float way_time_);

    Edge(const Edge& e) = delete;
    Edge& operator=(const Edge& e) = delete;
    Edge(Edge&& e) = delete;
    Edge& operator=(Edge&& e) = delete;

    Point* Origin() const noexcept{return origin;}
    Point* Destination() const noexcept{return destination;}
    float WayTime() const noexcept {return way_time;}
    sf::RectangleShape* Shape() noexcept {return &line;} 

private:
    sf::RectangleShape line;
    Point* origin;
    Point* destination;
    float way_time;
};


class Point
{
private:
    sf::Shape* shape;

    std::string name; //later
    float delay_time; //later
    int point_number;

public:
    Point(const int num, sf::Shape* sh)
        : point_number(num), shape(sh) {}
    ~Point() {delete shape;}

    Point(const Point& p) = delete;
    Point& operator=(const Point& p) = delete;
    Point(Point&&) = default;
    Point& operator=(Point&& p) = delete;

    sf::Shape* Shape() const noexcept {return shape;}
    std::string Name() const noexcept {return name;}
    u32 PointNumber() const noexcept {return point_number;}
    float DelayTime() const noexcept {return delay_time;}

    void FindPath(std::vector<Edge*>& edges, u32 num_of_points) const
    {
        int distance[num_of_points];
        for (u32 i = 0; i < num_of_points; i++)
        {
            distance[i] = INT_MAX;
        }
        distance[point_number] = 0;

        
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
                }
            }
        }

        std::cout << "Vertex Distance from Origin: " << std::endl;
        for (int i = 0; i < num_of_points; i++)
        {
            std::cout << i << "\t\t" << distance[i] << std::endl;
        } 
    }
};


Edge::Edge(Point* ori, Point* dest, const float way_time_)
    : destination(dest), origin(ori), way_time(way_time_)
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