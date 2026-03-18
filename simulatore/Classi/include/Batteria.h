#ifndef BATTERIA_H
#define BATTERIA_H

#include <iostream>

/**
 * @class Batteria
 * @brief Classe che rappresenta una batteria per l'immagazzinamento di energia.
 */
class Batteria{

    private:
        int capacita;   // Espressi in Kwh 
        int livello;    // Espressi in Kwh
    public:
        Batteria(int capacita) : capacita(capacita), livello(0) {};
        ~Batteria() = default;

        int getLivello() const { return this->livello; };
        int getCapacita() const { return this->capacita; };
        int getSpazioDisponibile() const { return this->capacita - this->livello; };

        int carica(int carica);
        int scarica(int scarica);
};

#endif