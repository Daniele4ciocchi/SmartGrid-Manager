#include "Batteria.h"

/**
 * @brief Carica energia nella batteria.
 * @param carica Quantità di energia da caricare (kWh).
 * @return Se la carica è andata a buon fine restituisce la quantità effettivamente caricata;
 *         altrimenti restituisce l'eccedenza non caricabile (kWh).
 */
int Batteria::carica(int carica)
{
    if (carica <= getSpazioDisponibile())
    {
        livello += carica;
        return carica;
    }
    else
    {
        int eccedenza = carica - getSpazioDisponibile();
        livello = capacita;
        return eccedenza;
    }
}

/**
 * @brief Scarica energia dalla batteria.
 * @param scarica Quantità di energia da scaricare (kWh).
 * @return Quantità effettivamente scaricata (kWh).
 */
int Batteria::scarica(int scarica)
{
    if (scarica <= livello)
    {
        livello -= scarica;
        return scarica;
    }
    else
    {
        int erogato = livello;
        livello = 0;
        return erogato;
    }
}