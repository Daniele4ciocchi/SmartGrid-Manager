#ifndef RINNOVABILE_H
#define RINNOVABILE_H

#include "Sorgente.h"

class Rinnovabile : public Sorgente{

    private:
        float max_produzione;

    public:
        Rinnovabile(float max_produzione);
        ~Rinnovabile();

        float produzione();
};

#endif