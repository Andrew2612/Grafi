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
    Edge(Point* ori, Point* dest, const u32 scale, const bool singleSided = false);

    Edge(const Edge& e) = delete;
    Edge& operator=(const Edge& e) = delete;
    Edge(Edge&& e) = delete;
    Edge& operator=(Edge&& e) = delete;

    Point* Origin() const {return origin;}
    Point* Destination() const {return destination;}
    float WayTime() const {return way_time;}
    bool SingleSided() const {return single_sided;}
    sf::RectangleShape* Shape() {return &line;}

    void TurnOn();
    void TurnOff();

private:
    sf::RectangleShape line;
    Point* origin;
    Point* destination;

    const u32 METRO_SPEED = 40;
    const u32 CAR_SPEED = 16;
    const u32 WALK_SPEED = 4;
    float way_time;
    bool single_sided;

    const float width = 3;
};


class Point
{
public:
    enum PointType {Metro, Street};

    Point(const int num, const float posX,
        const float posY, const std::string& name_, const PointType t);
    ~Point() {delete shape;}

    Point(const Point& p) = delete;
    Point& operator=(const Point& p) = delete;
    Point(Point&&) = default;
    Point& operator=(Point&& p) = delete;

    sf::Shape* Shape() const noexcept {return shape;}
    std::string Name() const noexcept {return name;}
    u32 PointNumber() const noexcept {return point_number;}
    PointType Type() const noexcept {return type;}

    std::vector<u32> FindPath(const std::vector<Edge*>& edges,
        u32 num_of_points, u32 destination_num) const;

private:
    sf::Shape* shape;
    const float radius = 12;
    
    sf::Texture point_texture;

    PointType type;
    std::string name; 
    int point_number;
};

#endif
