#include "Battery.h"


double Battery::charge(double charge)
{
    if (charge > (capacity - level))
        throw std::out_of_range("La carica supera la capacità residua della batteria");

    level += charge;
    return charge;
}

double Battery::discharge(double discharge)
{
    if (discharge < level)
        throw std::out_of_range("La scarica supera il livello attuale della batteria");
    level -= discharge;
    return discharge;
}