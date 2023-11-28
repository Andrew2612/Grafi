#ifndef Grafi__Edge_Point_
#define Grafi__Edge_Point_

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
    Edge(Point* ori, Point* dest, const float way_time_, const bool singleSided);

    Edge(const Edge& e) = delete;
    Edge& operator=(const Edge& e) = delete;
    Edge(Edge&& e) = delete;
    Edge& operator=(Edge&& e) = delete;

    Point* Origin() const {return origin;}
    Point* Destination() const {return destination;}
    float WayTime() const {return way_time;}
    bool SingleSided() const {return single_sided;}
    sf::RectangleShape* Shape() {return &line;}

    void TurnOn() {line.setFillColor(sf::Color(255, 0, 0));}
    void TurnOff() {line.setFillColor(sf::Color(0, 155, 120));}

private:
    sf::RectangleShape line;
    Point* origin;
    Point* destination;
    float way_time;
    bool single_sided;
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

    std::vector<u32> FindPath(const std::vector<Edge*>& edges,
        u32 num_of_points, u32 destination_num) const;
};

#endif
