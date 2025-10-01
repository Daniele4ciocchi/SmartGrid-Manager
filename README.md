# SmartGrid-Manager
Automatic management of electric grids with renewables and energy storage systems

## Build Ultra-Semplice

```bash
# Compila
./build.sh

# Compila ed esegue
./build.sh run
```

### Alternativo (manuale)

```bash
mkdir build && cd build
cmake ..
cmake --build .
./simulatore/programma
```

**Totale: 11 righe di CMake per tutto il progetto!** 🎯

### Compilazione Rapida

```bash
# Script di build automatico
./build.sh

# Oppure manualmente
mkdir build && cd build
cmake ..
cmake --build .
```

### Comandi Disponibili

```bash
# Compila tutto
cmake --build .

# Compila ed esegue
cmake --build . --target run

# Esegue direttamente
./simulatore/programma

# Build di debug
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .

# Pulizia
rm -rf build
```

### Struttura del Progetto

```
SmartGrid-Manager/
├── CMakeLists.txt          # Configurazione principale
├── build.sh                # Script di build automatico
└── simulatore/
    ├── CMakeLists.txt       # Configurazione simulatore
    ├── Classi/
    │   ├── CMakeLists.txt   # Libreria delle classi
    │   └── src/             # File .cpp e .h delle classi
    └── main/
        ├── CMakeLists.txt   # Configurazione main
        └── src/             # File main.cpp
```

### Caratteristiche

- ✅ **CMake moderno** (versione 3.16+)
- ✅ **C++17 standard**
- ✅ **Build gerarchica** organizzata
- ✅ **Libreria statica** per le classi
- ✅ **Target personalizzati** (run, clean)
- ✅ **Supporto Debug/Release**
