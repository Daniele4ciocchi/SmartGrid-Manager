#ifndef RETEELETTRICA_H
#define RETEELETTRICA_H

#include "Sorgente.h"   // includi la classe base

class ReteElettrica : public Sorgente {
private:
    float max_costo; // modificato per supportare valori decimali

public:
    ReteElettrica(float max_costo);
    ~ReteElettrica();

    float getCosto();
};

#endif
