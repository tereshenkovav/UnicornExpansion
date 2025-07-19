#include "MiniMap.h"

MiniMap::MiniMap()
{
    minimap_current.setOutlineThickness(2);
    minimap_current.setOutlineColor(sf::Color::White);
    minimap_current.setFillColor(sf::Color::Transparent);
}

void MiniMap::prepareMiniMap(int px, int py, int renderw, int renderh, int worldw, int worldh,
    int windoww, int windowh, int fullw, int fullh)
{
    this->x = px;
    this->y = py;
    this->w = renderw;
    this->h = renderh;
    this->worldw = worldw;
    this->worldh = worldh;
    this->windoww = windoww;
    this->windowh = windowh;
    this->fullw = fullw;
    this->fullh = fullh;

    // Здесь идут вычисления с учетом пропорций карты, окна и мира
    float scalex = (float)w / worldw;
    float scaley = (float)h / worldh;
    float scale = (scalex < scaley)?scalex:scaley;

    rect_w = (float)(worldw * scale) * windoww / fullw;
    rect_h = (float)(worldh * scale) * windowh / fullh;
    kx = 1.0f;
    ky = 1.0f;
    if (scalex < scaley)
        ky = scalex / scaley;
    else
        kx = scaley / scalex;
    minimap_current.setSize({ rect_w - 4, rect_h - 4 });

    if (worldw > worldh) y += 0.5f * h * (1.0f - (float)fullh / fullw);
    if (worldw < worldh) x += 0.5f * w * (1.0f - (float)fullw / fullh);

    // Заливка примитивами треугольников мини-карты
    minimap.setPrimitiveType(sf::PrimitiveType::Triangles);
    minimap.resize(2 * 3 * worldw * worldh);
    int p = 0;
    for (int i = 0; i < worldw; i++)
        for (int j = 0; j < worldh; j++) {
            minimap[p].position = { x + i * scale, y + j * scale };
            minimap[p + 1].position = { x + (i + 1) * scale, y + j * scale };
            minimap[p + 2].position = { x + (i + 1) * scale, y + (j + 1) * scale };
            minimap[p + 3].position = { x + i * scale, y + j * scale };
            minimap[p + 4].position = { x + i * scale, y + (j + 1) * scale };
            minimap[p + 5].position = { x + (i + 1) * scale, y + (j + 1) * scale };
            p += 6;
        }
}

void MiniMap::setCellColor(int i, int j, const sf::Color & c)
{
    int p = 6 * (i * worldh + j);
    for (int k = 0; k < 6; k++)
        minimap[p + k].color = c;
}

void MiniMap::setCenter(int cx, int cy)
{
    minimap_current.setPosition({ (float)(x + kx * w * cx / fullw - rect_w / 2 + 2),
        (float)(y + ky * h * cy / fullh - rect_h / 2 + 2 )});
}

void MiniMap::drawTo(sf::RenderTarget* target) const
{
    target->draw(minimap);
    target->draw(minimap_current);
}
