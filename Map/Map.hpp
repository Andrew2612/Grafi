#ifndef Grafi__Map_
#define Grafi__Map_

#include<SFML\Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include"../EdgeAndPoint/Edge_Point.hpp"

using u32 = uint32_t;
using i64 = int64_t;

struct Map;

class MapReader
{
public:
    MapReader(u32 map_index, Map* map_ptr);

private:
    std::ifstream map_file;

    Map* map;

    std::vector<Point*> points;
    std::vector<Edge*> edges;
    std::vector<Point*> places;

    void ReadMapInfo();

    u32 BuildMap(u32 j);
    u32 CreatePoints(u32 j, Point::PointType t);
    u32 CreateEdges(u32 j);
    u32 CreatePlaces(u32 j);

    enum struct TokenType {
        T_LSQUARE, T_RSQUARE, T_LCURLY, T_RCURLY,
        T_COMMA, T_COLON,
        T_INTEGER, T_STRING, T_TRUE, T_FALSE,
        T_EOF
    };

    struct Token {
        TokenType type;

        std::string value_str;
        i64 value_int;
        bool value_bool;
    };
    std::vector<Token> tokens;

    std::string ReadString(std::istream& is);
    bool is_letter(char c);
    i64 ReadInteger(std::istream& is);
    bool ReadComplexChar(char * symbol, std::istream& is);
    std::string ReadLiteral(std::istream& is);
    void EatWhitespace(std::istream& is);

    Token MakeIntegerToken(i64 value);
    Token MakeBoolToken(bool value);
    Token MakeStringToken(const std::string& value);
    Token MakeRawToken(TokenType tt);

    Token ReadNextToken(std::istream& is);
};

struct Map
{
    Map(u32 map) {MapReader reader{map, this};}

    ~Map();

    Map(const Map& s) = delete;
    Map& operator=(const Map& s) = delete;
    Map(Map&& s) = delete;
    Map& operator=(Map&& s) = delete;
    u32 width, height, zoom_max, zoom_min, scale;

    sf::Texture map_texture;
    sf::Sprite map_sprite;

    std::vector<Point*> points;
    std::vector<Edge*> edges;
    std::vector<Point*> places;

    void CreatePoint(const float posX, const float posY, const std::string& name, Point::PointType t);

    void CreateEdge(const u32 origin, const u32 dest, const u32 scale);
    void AddEdge(const u32 place_origin, const u32 point_dest, const u32 scale);

    void CreatePlace(const float posX, const float posY, const std::string& name, const u32 closest);
};


#endif