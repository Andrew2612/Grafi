//#include <SFML/Graphics.hpp>
#include"Edge_Point/Edge_Point.cpp"
#include<time.h>
#include<cstdint>

using u32 = uint32_t;

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

    p0.FindPath(Edges, 5, 7);
    
    return 0;
}