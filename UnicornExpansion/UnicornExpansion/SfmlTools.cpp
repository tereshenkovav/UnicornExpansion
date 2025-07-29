#include "SfmlTools.h"

sf::String SfmlTools::utf2text(const std::string& str)
{
    return sf::String::fromUtf8(str.begin(), str.end());
}

sf::Color SfmlTools::getColorAsBright(const sf::Color& color, float bright)
{
    sf::Color c;
    c.r = (uint8_t)(color.r * bright>255?255:color.r * bright);
    c.g = (uint8_t)(color.g * bright>255?255:color.g * bright);
    c.b = (uint8_t)(color.b * bright>255?255:color.b * bright);
    c.a = color.a;
    return c;
}