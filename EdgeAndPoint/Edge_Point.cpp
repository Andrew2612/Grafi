#include<string>
#include<iostream>
#include<cstdint>

using u32 = uint32_t;

class Point;

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


class Point
{
private:
    std::string name; //later
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

    void FindPath(Edge* edges[], u32 num_of_points, u32 num_of_edges) const
    {
        int distance[num_of_points];
        for (u32 i = 0; i < num_of_points; i++)
        {
            distance[i] = INT_MAX;
        }
        distance[point_number] = 0;

        
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
};
