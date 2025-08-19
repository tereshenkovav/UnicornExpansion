#include "SeedStore.h"

std::map<int, std::map<LaserType, float>> SeedStore::seeds;

float SeedStore::getSeedByUIDAndLaserType(int uid, LaserType lasertype)
{
    if (seeds.count(uid) == 0)
        seeds[uid]=std::map<LaserType, float>();
    
    if (seeds[uid].count(lasertype) == 0)
        seeds[uid][lasertype] = 0.01f*(rand() % 1000);

    return seeds[uid][lasertype];
}
