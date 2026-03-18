#include "ReteElettrica.h"


/**
 * @brief Aggiunge un prezzo alla lista dei prezzi della rete elettrica.
 * @param prezzo Il prezzo da aggiungere.
 */
void ReteElettrica::addPrezzo(double prezzo){
    prezzi.push_back(prezzo);
}

/**
 * @brief Restituisce il prezzo corrispondente a un timestamp specifico.
 * @param ts Il timestamp per cui si desidera ottenere il prezzo.
 * @return Il prezzo corrispondente al timestamp, o 0.0 se il timestamp è fuori dai limiti.
 */
double ReteElettrica::getPrezzoByTs(int ts) {
    if (ts < 0 || ts >= (int)prezzi.size()) {
        
        return 0.0;
    }
    return prezzi[ts];
}

/**
 * @brief Aggiunge una media alla lista delle medie della rete elettrica.
 * @param media La media da aggiungere.
 */
void ReteElettrica::addMedia(double media){
    medie.push_back(media);
}

/**
 * @brief Restituisce la media corrispondente a un timestamp specifico.
 * @param ts Il timestamp per cui si desidera ottenere la media.
 * @return La media corrispondente al timestamp, o 0.0 se il timestamp è fuori dai limiti.
 */
double ReteElettrica::getMediaByTs(int ts) {
    if (ts < 0 || ts >= (int)medie.size()) {
        return 0.0;
    }
    return medie[ts];
}
