#include "ReteElettrica.h"
#include <cstdlib>  // per rand()

ReteElettrica::ReteElettrica(float max_costo) : max_costo(max_costo) {}

ReteElettrica::~ReteElettrica() {}

float ReteElettrica::getCosto() {
    // Genera un numero casuale float tra 0.1 e max_costo
    return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (max_costo - 0.1f) + 0.1f;
}
