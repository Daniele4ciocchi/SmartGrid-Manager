#include <cstdlib>
#include "Rinnovabile.h"
#include "Sorgente.h"
 
using namespace std;


Rinnovabile::Rinnovabile(float max_produzione){
    this->max_produzione = max_produzione;
}

Rinnovabile::~Rinnovabile(){}

float Rinnovabile::produzione(){
    // Genera un numero casuale intero tra 1 e max_produzione
    return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (max_produzione - 0.1f) + 0.1f;
}