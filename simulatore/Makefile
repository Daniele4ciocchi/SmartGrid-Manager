# Makefile per il simulatore di rete elettrica

# Compilatore e flag
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g

# Directory
CLASSDIR = Classi
OBJDIR = obj

# Nome del programma eseguibile
TARGET = programma

# File sorgente
MAIN_SRC = main.cpp
CLASS_SOURCES = $(CLASSDIR)/Sorgente.cpp \
                $(CLASSDIR)/Consumatore.cpp \
                $(CLASSDIR)/Rinnovabile.cpp \
                $(CLASSDIR)/ReteElettrica.cpp

# File oggetto
MAIN_OBJ = $(OBJDIR)/main.o
CLASS_OBJECTS = $(OBJDIR)/Sorgente.o \
                $(OBJDIR)/Consumatore.o \
                $(OBJDIR)/Rinnovabile.o \
                $(OBJDIR)/ReteElettrica.o

ALL_OBJECTS = $(MAIN_OBJ) $(CLASS_OBJECTS)

# Regola principale
all: $(TARGET)

# Creazione dell'eseguibile
$(TARGET): $(ALL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilazione del main
$(OBJDIR)/main.o: $(MAIN_SRC) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilazione delle classi
$(OBJDIR)/%.o: $(CLASSDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Creazione della directory obj se non esiste
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Pulizia dei file compilati
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Pulizia completa (include anche file temporanei)
distclean: clean
	rm -f *~ $(CLASSDIR)/*~

# Ricompilazione completa
rebuild: clean all

# Esecuzione del programma
run: $(TARGET)
	./$(TARGET)

# Debug con gdb
debug: $(TARGET)
	gdb ./$(TARGET)

# Regole che non corrispondono a file
.PHONY: all clean distclean rebuild run debug

# Dipendenze dei file header
$(OBJDIR)/main.o: $(CLASSDIR)/Rinnovabile.h $(CLASSDIR)/Consumatore.h $(CLASSDIR)/ReteElettrica.h
$(OBJDIR)/Consumatore.o: $(CLASSDIR)/Consumatore.h
$(OBJDIR)/Rinnovabile.o: $(CLASSDIR)/Rinnovabile.h $(CLASSDIR)/Sorgente.h
$(OBJDIR)/ReteElettrica.o: $(CLASSDIR)/ReteElettrica.h $(CLASSDIR)/Sorgente.h
$(OBJDIR)/Sorgente.o: $(CLASSDIR)/Sorgente.h
