#ifndef BATTERIA_H
#define BATTERIA_H

class Batteria{

    private:
        int capacita;   // Espressi in Kwh 
        int livello;    // Espressi in Kwh
    public:
        Batteria(int capacita);
        ~Batteria();

        int getLivello() const;
        void carica(int carica);
        int scarica(int scarica);
};

#endif