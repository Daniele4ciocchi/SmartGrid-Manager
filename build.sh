#!/bin/bash

# Script di build semplice
mkdir -p build
cd build
cmake ..
cmake --build .

# Esegui se richiesto
if [ "$1" = "run" ]; then
    ./simulatore/programma
fi
