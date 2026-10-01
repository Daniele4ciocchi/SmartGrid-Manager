#!/bin/bash

set -euo pipefail

STRATEGY="random"
if (( $# > 0 )) && [[ -n "$1" ]]; then
    STRATEGY="$1"
fi

case "$STRATEGY" in
    random|geometric|smart) ;;
    *)
        echo "Uso: $0 [random|geometric|smart] [ripetizioni]" >&2
        exit 2
        ;;
esac

REPETITIONS=1
if (( $# > 1 )); then
    REPETITIONS="$2"
fi
if ! [[ "$REPETITIONS" =~ ^[1-9][0-9]*$ ]]; then
    echo "Il numero di ripetizioni deve essere un intero positivo." >&2
    exit 2
fi

echo "Compilazione in corso..."
cd build
make -j4
cd ..

echo "Avvio di $STRATEGY: $REPETITIONS ripetizione/i per Set Aside."
echo "Il confronto con Smart viene effettuato a parità di Set Aside."

for (( rep=1; rep<=REPETITIONS; rep++ )); do
    for b in 10; do
        for sa in 10 20 30 50; do
            echo "=========================================================="
            echo "Ripetizione=$rep/$REPETITIONS, Strategy=$STRATEGY, Budget=$b, SetAside=$sa"
            ./build/simulatore/main/simulatore \
                --strategy "$STRATEGY" \
                --budget "$b" \
                --set-aside "$sa" \
                --simulator
        done
    done
done

echo "Tutte le esecuzioni completate. Risultati aggiunti a data/esperimenti.csv"
