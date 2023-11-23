//#include<Edge_Point.hpp>
#include<string>
#include<vector>
#include<iostream>
#include<cstdint>

using u32 = uint32_t;

class Point
{
private:
    std::string name;
    float delay_time;
    int point_number;


public:
    Point(const int num, const float delay_time_)
        : point_number(num), delay_time(delay_time_) {}

    Point(const Point& p) = delete;
    Point& operator=(const Point& p) = delete;
    Point(Point&&) = default;
    Point& operator=(Point&& p) = delete;

    std::string Name() const noexcept {return name;}
    u32 PointNumber() const noexcept {return point_number;}
    float DelayTime() const noexcept {return delay_time;}

};

class Edge
{
public:
    Edge(Point* ori, Point* dest, const float way_time_)
        : destination(dest), origin(ori), way_time(way_time_) {};

    Edge(const Edge& e) = delete;
    Edge& operator=(const Edge& e) = delete;
    Edge(Edge&&) = default;
    Edge& operator=(Edge&& e) = delete;

    Point* Origin() const noexcept{return origin;}
    Point* Destination() const noexcept{return destination;}
    float WayTime() const noexcept {return way_time;}
private:
    Point* origin;
    Point* destination;
    float way_time;
};

void FindPath(Edge* edges[], u32 num_of_points, u32 num_of_edges)
{
    int distance[num_of_points];
    for (u32 i = 0; i < num_of_points; i++)
    {
        distance[i] = INT_MAX;
    }
    distance[0] = 0;

    
    for (u32 i = 0; i < num_of_points; i++)
    {
        for (u32 j = 0; j < num_of_edges; j++)
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

int main()
{
    //Screen screen;
    //sf::Clock clock;

	// while (screen.Open())
	// {
    //     screen.GetClosingEvent();

	// 	float dt = clock.getElapsedTime().asMicroseconds();

	// 	clock.restart();

    // }
    Point p0(0, 0);
    Point p1(1, 0);
    Point p2(2, 0);
    Point p3(3, 0);
    Point p4(4, 0);

    
    Edge e0(&p0, &p1, 3);
    Edge e1(&p0, &p2, 3);
    Edge e2(&p3, &p4, 3);
    Edge e3(&p1, &p0, 3);
    Edge e4(&p0, &p4, 3);
    Edge e5(&p2, &p1, 3);
    Edge e6(&p4, &p0, 3);

    Edge* Edges[7] = {&e0, &e1, &e2, &e3, &e4, &e5, &e6};

    FindPath(Edges, 5, 7);
    
    return 0;
}
