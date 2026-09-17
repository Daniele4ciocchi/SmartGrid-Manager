#!/bin/bash

set -euo pipefail

STRATEGY="${1:-geometric}"
case "$STRATEGY" in
    random|geometric|smart) ;;
    *)
        echo "Uso: $0 [random|geometric|smart]" >&2
        exit 2
        ;;
esac

# Compila il progetto
echo "Compilazione in corso..."
cd build || exit
make -j4
cd .. || exit

# Array di parametri da testare
BUDGET_VALS=(10)
BUY_VALS=(0.90 0.80 0.70)
SELL_VALS=(1.00)
GEOM_VALS=(10 20 50 100)
SET_ASIDE_VALS=(10 20 30 50)

echo "Inizio batteria di test..."

for b in "${BUDGET_VALS[@]}"; do
    for g in "${GEOM_VALS[@]}"; do
        for buy in "${BUY_VALS[@]}"; do
            for sell in "${SELL_VALS[@]}"; do
                for sa in "${SET_ASIDE_VALS[@]}"; do
                    echo "=========================================================="
                    echo "Esecuzione test con: Strategy=${STRATEGY}, Budget=${b}, BuyTH=${buy}, SellTH=${sell}, Geom=${g}, SetAside=${sa}"
                    ./build/simulatore/main/simulatore --strategy "$STRATEGY" --budget "$b" --buy "$buy" --sell "$sell" --geom "$g" --set-aside "$sa" --simulator
                done
            done
        done
    done
done

echo "Tutti i test completati! Controlla il file data/esperimenti.csv"
