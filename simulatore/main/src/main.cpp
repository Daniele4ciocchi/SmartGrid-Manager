#include "Rinnovabile.h"
#include "Consumatore.h"
#include "ReteElettrica.h"
#include <ctime>
#include <chrono>  
#include <stdlib.h>
#include <iostream>

using namespace std;
using namespace std::chrono;

int main(){

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
    
}