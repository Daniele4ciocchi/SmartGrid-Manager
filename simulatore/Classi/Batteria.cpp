#include <iostream>
#include "Batteria.h"

Batteria::Batteria(int capacita){
    this->capacita = capacita;
    livello = 0;
}

Batteria::~Batteria(){}

int Batteria::getLivello() const {
    return this->livello;
}

// modificare carica e scarica per controlli più chiari 
// scarica accetta valori negativi ?

void Batteria::carica(int carica){
    if (this->livello + carica < this->capacita) this->livello = this->livello + carica;
    else this->livello = capacita;
}

int Batteria::scarica(int scarica){
    if (this->livello - scarica > 0){
        this->livello = this->livello - scarica;
        return scarica;
    }
    else{
        int tmp = this->livello;
        this->livello = 0;
        return tmp;
    }  
}