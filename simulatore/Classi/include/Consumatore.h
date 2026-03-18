#ifndef CONSUMATORE_H
#define CONSUMATORE_H

class Consumatore{

    private:
        float max_consumo;

    public:
        Consumatore(float max_consumo);
        ~Consumatore();

        float consuma();
};

#endif