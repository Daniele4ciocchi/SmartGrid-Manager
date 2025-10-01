#include "Classi/Rinnovabile.h"
#include "Classi/Consumatore.h"
#include "Classi/ReteElettrica.h"
#include <ctime>
#include <chrono>  // Per misurare il tempo con alta precisione
#include <stdlib.h>
#include <iostream>

using namespace std;
using namespace std::chrono;

int main(){
    // Inizio misurazione del tempo
    auto start_time = high_resolution_clock::now();
    
    srand (time(NULL));

    Consumatore c1(2);
    Rinnovabile r1(3);
    ReteElettrica e1(15.0);

    for (int i = 0; i<10; i++){
        cout << "CONSUMATORE: " << c1.consuma() << " kwh" << endl;
        cout << "RINNOVABILE: " << r1.produzione() << " kwh" << endl;
        cout << "RETE ELETTRICA: " << e1.getCosto() << " kwh" << endl;
        cout << "---" << endl;
    }
    
    // Fine misurazione del tempo
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end_time - start_time);
    
    cout << "\n=== STATISTICHE TEMPO DI ESECUZIONE ===" << endl;
    cout << "Tempo di esecuzione: " << duration.count() << " microsecondi" << endl;
    cout << "Tempo di esecuzione: " << duration.count() / 1000.0 << " millisecondi" << endl;
    cout << "Tempo di esecuzione: " << duration.count() / 1000000.0 << " secondi" << endl;
}