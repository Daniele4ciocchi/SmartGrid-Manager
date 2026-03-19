#include "strategy.h"

void randomChoise(ElectricityGrid *r, Wallet *w, Battery *b, int j)
{
    // Random number generator for thread safety
    static thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dis_bs(0, 1);
    int avaiable = b->getFreeSpace();
    int livello = b->getLevel();

    int g = std::min(avaiable, dis_bs(gen) + 1); // compra poco

    if (dis_bs(gen) == 0 && avaiable > g) // compra
    {
        w->buy(r, r->getPriceByTs(j), b->charge(g));
    }
    else if (livello > g) // vendi
    {
        w->sell(r, r->getPriceByTs(j), b->discharge(g));
    }
}

void personalChoise(ElectricityGrid *r, Wallet *w, Battery *b, int j)
{
    int livello = b->getLevel();
    int spazio = b->getFreeSpace();

    if (j < 5)
        return; // serve un minimo di storico

    // Calcolo media mobile semplice sugli ultimi 5 timestep
    double media = 0.0;
    for (int k = j - 5; k < j; ++k)
        media += r->getPriceByTs(k);
    media /= 5.0;

    double prezzo = r->getPriceByTs(j);

    // soglia per evitare overtrading
    const double soglia = 0.05 * media; // 5%

    // COMPRA
    if (prezzo < media - soglia && spazio > 0)
    {
        int g = std::min(spazio, 5); // compra poco → meno rischio
        w->buy(r, prezzo, b->charge(g));
    }
    // VENDI
    else if (prezzo > media + soglia && livello > 0)
    {
        int g = std::min(livello, 5);
        w->sell(r, prezzo, b->discharge(g));
    }
    // ALTRIMENTI: niente
}

void geometricChoise(ElectricityGrid *r, Wallet *w, Battery *b, int i, int j)
{

    int livello = b->getLevel();
    int spazio = b->getFreeSpace();

    double media = r->getMediaByTs(j-1);
    double prezzo = r->getPriceByTs(j);

    double rapporto = prezzo / media;

    double moltiplicatore = rapporto < 1 ? 1.0 - rapporto : rapporto - 1.0;

    //int g = std::max(1, static_cast<int>(moltiplicatore * 30)); // scala 0-30, minimo 1

    int g = 10  * prezzo; 
    // COMPRA
    if (rapporto < 1 && spazio > g)
    {
        w->buy(r, prezzo, b->charge(g));
    }
    // VENDI
    else if (rapporto > 1 && livello > g)
    {
        w->sell(r, prezzo, b->discharge(g));
    }
}
