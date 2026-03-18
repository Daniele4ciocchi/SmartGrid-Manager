#ifndef BATTERIA_H
#define BATTERIA_H

#include <iostream>

/**
 * @class Batteria
 * @brief Classe che rappresenta una batteria per l'immagazzinamento di energia.
 */
class Battery{

    private:
        double capacity;   // Espressi in Kwh 
        double level;    // Espressi in Kwh
    public:
        Battery(double capacity) : capacity(capacity), level(0) {};
        ~Battery() = default;

        double getLevel() const { return this->level; };
        double getCapacity() const { return this->capacity; };
        double getFreeSpace() const { return this->capacity - this->level; };

        double charge(double charge);
        double discharge(double discharge);
};

#endif