#include "Consumatore.h"
#include <cstdlib>
#include <ctime>

Consumatore::Consumatore(float max_consumo){
    this->max_consumo = max_consumo;
}

Consumatore::~Consumatore(){}

float Consumatore::consuma(){
    // Genera un numero casuale tra 0 e max_consumo
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * this->max_consumo;
}