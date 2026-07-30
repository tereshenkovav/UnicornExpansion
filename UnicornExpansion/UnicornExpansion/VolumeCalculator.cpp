#include "VolumeCalculator.h"
#include "math.h"

VolumeCalculator::VolumeCalculator(sf::Vector2f poslt, sf::Vector2f posrb, float minvol) {
	this->minvol = minvol;
    dist = sqrt((posrb.x - poslt.x) * (posrb.x - poslt.x) + (posrb.y - poslt.y) * (posrb.y - poslt.y)) / 2;
    cx = (poslt.x + posrb.x) / 2;
    cy = (poslt.y + posrb.y) / 2;
    dx = (posrb.x - poslt.x) / 2;
    dy = (posrb.y - poslt.y) / 2;
}

float VolumeCalculator::getVolume(sf::Vector2f pos) const {
    if ((poslt.x <= pos.x) && (pos.x <= posrb.x) && (poslt.y <= pos.y) && (pos.y <= posrb.y)) return 100.0f;
    
    float maxdist = std::max(abs(cx - pos.x) - dx, abs(cy - pos.y) - dy);
    if (maxdist > 0.0)
        return std::max(minvol, -(100.0f - minvol) * maxdist / dist + 100.0f);
    else
        return 100.0f;
}
