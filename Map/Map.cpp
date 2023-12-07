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
}

void Map::CreatePoint(float posX, float posY)
{
    sf::CircleShape* shape = new sf::CircleShape(10);
    shape->setOrigin(sf::Vector2f(10.0f, 10.0f));
    shape->setFillColor(sf::Color(255, 0, 150));
    shape->setPosition(sf::Vector2f(posX, posY));
    points.push_back(new Point(points.size(), shape));
}

void Map::CreateEdge(u32 origin, u32 dest, u32 weight)
{
    edges.push_back(new Edge(points[origin], points[dest], weight));
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
            if (tokens[i].value_str == "points")
            {
                i = CreatePoints(i+1);
            }
            if (tokens[i].value_str == "edges")
            {
                i = CreateEdges(i+1);
            }
        }
        i++;
    }
}



u32 MapReader::BuildMap(u32 j)
{
    u32 wigth, height, zoom_max, zoom_min;
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
        }
        j++;
    }

    map->width = wigth;
    map->height = height;
    map->zoom_max = zoom_max;
    map->zoom_min = zoom_min;

    return j + 1;
}

u32 MapReader::CreatePoints(u32 j)
{
    u32 posX, posY;
    std::string current_param;
    u32 counter = 0;

    while (tokens[j].type != TokenType::T_RSQUARE)
    {
        if (tokens[j].type == TokenType::T_STRING)
        {
            current_param = tokens[j].value_str;
        }
        else if (tokens[j].type == TokenType::T_INTEGER)
        {
            counter++;
            if (current_param == "posX") {posX = tokens[j].value_int;}
            else if (current_param == "posY") {posY = tokens[j].value_int;}

            if (counter == 2)
            {
                map->CreatePoint(static_cast<float>(posX), static_cast<float>(posY));
                counter = 0;
            }
        }
        j++;
    }
    return j + 1;
}

u32 MapReader::CreateEdges(u32 j)
{
    u32 origin, dest, weight;
    std::string current_param;
    u32 counter = 0;

    while (tokens[j].type != TokenType::T_RSQUARE)
    {
        if (tokens[j].type == TokenType::T_STRING)
        {
            current_param = tokens[j].value_str;
        }
        else if (tokens[j].type == TokenType::T_INTEGER)
        {
            counter++;
            if (current_param == "origin") {origin = tokens[j].value_int;}
            else if (current_param == "dest") {dest = tokens[j].value_int;}
            else if (current_param == "weight") {weight = tokens[j].value_int;}

            if (counter == 3)
            {
                map->CreateEdge(origin, dest, weight);
                counter = 0;
            }
        }
        j++;
    }
    return j + 1;
}

i64 MapReader::ReadInteger(std::istream& is) {
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
      std::cout << "processing: c = " << c << " flag = " << flag << "\n"; 
      
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
