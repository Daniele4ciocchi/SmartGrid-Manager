#ifndef RETEELETTRICA_H
#define RETEELETTRICA_H

#include "Sorgente.h"
#include <vector>
#include <utility>
#include <cstdlib>
#include <iostream>

/**
 * @class ReteElettrica
 * @brief Classe derivata da Sorgente che rappresenta una rete elettrica con una lista di prezzi.
 */
class ReteElettrica : public Sorgente
{
private:
    std::vector<double> prezzi;
    std::vector<double> medie = std::vector<double>(10);

public:
    ReteElettrica() = default;
    ~ReteElettrica() = default;

    void addPrezzo(double prezzo);
    double getPrezzoByTs(int ts);
    void addMedia(double medie);
    double getMediaByTs(int ts);
};

#endif
