#include"Map.hpp"
#include<SFML\Graphics.hpp>

Map::~Map()
{
    for (u32 i = 0; i < points.size(); i++)
    {
        delete points[i];
    }
    points.clear();

    for (u32 i = 0; i < edges.size(); i++)
    {
        delete edges[i];
    }
    edges.clear();

    for (u32 i = 0; i < places.size(); i++) {
        delete places[i];
    }
    places.clear();
}

void Map::CreatePoint(const float posX, const float posY, const std::string& name, Point::PointType t)
{
    points.push_back(new Point(points.size(), posX, posY, name, t));
}

void Map::CreateEdge(const u32 origin, const u32 dest, const u32 sclae)
{
    edges.push_back(new Edge(points[origin], points[dest], scale));
}
void Map::AddEdge(const u32 place_origin, const u32 point_dest, const u32 scale)
{
    edges.push_back(new Edge(places[place_origin], points[point_dest], scale));
}

void Map::CreatePlace(const float posX, const float posY, const std::string& name, const u32 closest)
{
    places.push_back(new Point(closest, posX, posY, name, Point::PointType::Place));
}


void Assert(bool cond, const char* cond_str) {
    if (!cond) {
        throw std::logic_error(std::string("Assertion failed:   ") + cond_str);
    }
}

#define ASSERT(c)  do { Assert((c), #c); } while(0);


MapReader::MapReader(u32 map_index, Map* map_ptr) : map(map_ptr)
{
    switch (map_index)
    {
        case 1:
            map_file.open("../Map/MapInfo/Map1.json");
            map->map_texture.loadFromFile("../Map/MapInfo/Map1.jpg");
            map->map_sprite.setTexture(map->map_texture);
            map->map_sprite.setPosition(0,0);
            break;
        default:
          Assert(false, "Wrong Map");
    }
    ReadMapInfo();
}

void MapReader::ReadMapInfo()
{
    while (map_file.good())
    {
        Token t = ReadNextToken(map_file);
        tokens.push_back(t);
    }

    u32 i = 0;
    while (tokens[i].type != TokenType::T_EOF)
    {
        if (tokens[i].type == TokenType::T_STRING)
        {
            if (tokens[i].value_str == "map")
            {
                i = BuildMap(i+1);
            }
            if (tokens[i].value_str == "metro_points")
            {
                i = CreatePoints(i+1, Point::PointType::Metro);
            }
            if (tokens[i].value_str == "street_points")
            {
                i = CreatePoints(i+1, Point::PointType::Street);
            }
            if (tokens[i].value_str == "edges")
            {
                i = CreateEdges(i+1);
            }
            if (tokens[i].value_str == "places") {
                i = CreatePlaces(i+1);
            }
        }
        i++;
    }
}

u32 MapReader::BuildMap(u32 j)
{
    u32 wigth, height, zoom_max, zoom_min, scale;
    std::string current_param;

    while (tokens[j].type != TokenType::T_RCURLY)
    {
        if (tokens[j].type == TokenType::T_STRING)
        {
            current_param = tokens[j].value_str;
        }
        else if (tokens[j].type == TokenType::T_INTEGER)
        {
            if (current_param == "width") {wigth = tokens[j].value_int;}
            else if (current_param == "height") {height = tokens[j].value_int;}
            else if (current_param == "zoom_max") {zoom_max = tokens[j].value_int;}
            else if (current_param == "zoom_min") {zoom_min = tokens[j].value_int;}
            else if (current_param == "scale") {scale = tokens[j].value_int;}
        }
        j++;
    }

    map->width = wigth;
    map->height = height;
    map->zoom_max = zoom_max;
    map->zoom_min = zoom_min;
    map->scale = scale;

    return j + 1;
}

u32 MapReader::CreatePlaces(u32 j)
{
    u32 posX, posY, closest;
    std::string name;
    std::string current_param;
    u32 counter = 0;

    while (tokens[j].type != TokenType::T_RSQUARE)
    {
        if(tokens[j].type == TokenType::T_STRING)
        {
            if (tokens[j+2].type == TokenType::T_STRING) {
                counter++;
                name = tokens[j+2].value_str;
                j+= 2;
            }
            else {
                current_param = tokens[j].value_str;
            }
        }
        else if (tokens[j].type == TokenType::T_INTEGER)
        {
            if (current_param == "posX") {posX = tokens[j].value_int; counter++;}
            else if (current_param == "posY") {posY = tokens[j].value_int; counter++;}
            else if (current_param == "closest") {closest = tokens[j].value_int; counter++;}
        }

        if (counter == 4)
        {
            map->CreatePlace(static_cast<float>(posX), static_cast<float>(posY), name, closest);
            counter = 0;
        }
        j++;
    }
    return j+1;
}

u32 MapReader::CreatePoints(u32 j, const Point::PointType t)
{
    u32 posX, posY;
    std::string name;
    std::string current_param;
    u32 counter = 0;

    while (tokens[j].type != TokenType::T_RSQUARE)
    {
        if (tokens[j].type == TokenType::T_STRING)
        {
            if (tokens[j+2].type == TokenType::T_STRING)
            {
                counter++;
                name = tokens[j+2].value_str;
                j += 2;
            }
            else
            {
                current_param = tokens[j].value_str;
            }
        }
        else if (tokens[j].type == TokenType::T_INTEGER)
        {
            if (current_param == "posX") {posX = tokens[j].value_int; counter++;}
            else if (current_param == "posY") {posY = tokens[j].value_int; counter++;}
        }

        if (counter == 3)
        {
            map->CreatePoint(static_cast<float>(posX), static_cast<float>(posY), name, t);
            counter = 0;
        }
        j++;
    }
    return j + 1;
}

u32 MapReader::CreateEdges(u32 j)
{
    u32 origin, dest;
    std::string current_param;
    u32 counter = 0;

    while (tokens[j].type != TokenType::T_RSQUARE)
    {
        if (tokens[j].type == TokenType::T_INTEGER && tokens[j+2].type == TokenType::T_INTEGER)
        {
            map->CreateEdge(tokens[j].value_int, tokens[j+2].value_int, map->scale);
            j++;
        }
        j++;
    }
    return j + 1;
}

i64 MapReader::ReadInteger(std::istream& is)
{
    i64 val = 0;
    bool negative = false;

    char c = is.peek();
    if ((c == '-') || (c == '+')) {
        negative = (c == '-');
        is.get();
        EatWhitespace(is);
        c = is.peek();
    }

    ASSERT(isdigit(c));
    
    do {
        val *= 10;
        val += (c - '0');
        is.get();
        c = is.peek();
    } while (isdigit(c));

    if (negative) val = -val;
    return val;
}

bool MapReader::is_letter(char c){
    if((c>='a') && (c<='z')){
      return true;
    }
    if((c>='A') && (c<='Z')){
      return true;
    }
    return false;
}


std::string MapReader::ReadLiteral(std::istream& is){
    std::string smth;

    char c = is.peek();
    if(is_letter(c)){
      while(is_letter(c) || (isdigit(c))){
        smth += c;
        is.get();
        c = is.peek();
      }
    }else{
      ASSERT(false);
    }

    return smth;
}

bool MapReader::ReadComplexChar(char * symbol, std::istream& is)
{
    char candidate = is.peek();
    if(candidate == '\\'){
        is.get();
        *symbol = is.peek();
        is.get();
        switch(*symbol){
            case 'n': *symbol = '\n'; break;
            case 'r': *symbol = '\r'; break;

            default: break;
      }
      return true;
    }
    is.get();
    *symbol = candidate;
    return false;
}

std::string MapReader::ReadString(std::istream& is){
    std::string smth;
    bool flag;

    char c = is.peek();
    
    ASSERT(c == '"');
    is.get();
    flag = ReadComplexChar(&c, is);
    while(flag || c!='"'){
      smth += c;
      flag = ReadComplexChar(&c, is);
    }
    return smth;
}


void MapReader::EatWhitespace(std::istream& is)
{
    while (isspace(is.peek())) {
        is.get();
    }
}

MapReader::Token MapReader::MakeIntegerToken(i64 value) {
    Token t; t.type = TokenType::T_INTEGER; t.value_int = value;
    return t;
}

MapReader::Token MapReader::MakeBoolToken(bool value) {
    Token t; t.type = TokenType::T_INTEGER; t.value_bool = value;
    return t;
}

MapReader::Token MapReader::MakeStringToken(const std::string& value) {
    Token t; t.type = TokenType::T_STRING; 
    t.value_str = value;
    return t;
}

MapReader::Token MapReader::MakeRawToken(TokenType tt) {
    Token t; t.type = tt;
    return t;
}

MapReader::Token MapReader::ReadNextToken(std::istream& is)
{
    EatWhitespace(is);
    char c = is.peek();

    if (c == '"') {
        auto s = ReadString(is);
        return MakeStringToken(s);
    } else if (isdigit(c) || (c == '+') || (c == '-')) {
        auto s = ReadInteger(is);
        return MakeIntegerToken(s);
    } else if (is_letter(c)) {
        auto s = ReadLiteral(is);
        if (s == "true") {
            return MakeRawToken(TokenType::T_TRUE);
        } else if (s == "false") {
            return MakeRawToken(TokenType::T_FALSE);
        } else if (s == "null") {
            Assert(false, "NULL is not supported");
        } else {
            std::string err = "Unexpected literal " + s;
            Assert(false, err.c_str());
      }
    } else if (c == '[') {
        is.get();
      return MakeRawToken(TokenType::T_LSQUARE);
    } else if (c == ']') {
        is.get();
      return MakeRawToken(TokenType::T_RSQUARE);
    } else if (c == '{') {
        is.get();
        return MakeRawToken(TokenType::T_LCURLY);
    } else if (c == '}') {
        is.get();
        return MakeRawToken(TokenType::T_RCURLY);
    } else if (c == ',') {
        is.get();
        return MakeRawToken(TokenType::T_COMMA);
    } else if (c == ':') {
        is.get();
        return MakeRawToken(TokenType::T_COLON);
    }

    if (is.eof()) return MakeRawToken(TokenType::T_EOF);

    std::cerr << "Bad next char: [" << c << "]\n"; 
    Assert(false, "Illegal next token");

    return {};
}
