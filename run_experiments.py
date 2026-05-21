import os
import subprocess
import pandas as pd
import itertools

CSV_PATH = "data/esperimenti.csv"
EXECUTABLE = "./build/simulatore/main/simulatore"

# Definisci qui lo spazio dei parametri da testare
BUDGET_VALS = [10]
BUY_VALS = [1.0, 0.90, 0.80, 0.70]
SELL_VALS = [1.0]
GEOM_VALS = [10, 20, 50, 100]
SET_ASIDE_VALS = [10, 20, 30, 50]

def get_completed_experiments():
    if not os.path.exists(CSV_PATH):
        return set()
    
    try:
        df = pd.read_csv(CSV_PATH)
        # Rimuovi eventuali righe di header rimaste in mezzo (se appese da script di shell vecchi)
        df = df[df["Strategia"] != "Strategia"]
        
        completed = set()
        for _, row in df.iterrows():
            try:
                # Arrotondiamo leggermente i parametri per evitare problemi di virgola mobile quando li confrontiamo
                geom = round(float(row.get("Finestra Geometrica", 0)), 2)
                budget = round(float(row.get("Budget", 0)), 2)
                set_aside = round(float(row.get("Set Aside %", 0)), 2)
                buy = round(float(row.get("Buy Threshold", 0)), 2)
                sell = round(float(row.get("Sell Threshold", 0)), 2)
                
                completed.add((budget, geom, set_aside, buy, sell))
            except (ValueError, KeyError):
                continue
                
        return completed
    except Exception as e:
        print(f"Errore nella lettura del CSV o file non formattato correttamente: {e}")
        return set()

def main():
    if not os.path.exists(EXECUTABLE):
        print(f"Errore: l'eseguibile {EXECUTABLE} non esiste. Compila il progetto prima (es. usando make).")
        return

    # Controlla quali sono già stati fatti
    completed = get_completed_experiments()
    print(f"Trovati {len(completed)} esperimenti unici già completati nel CSV.")

    # Genera tutte le combinazioni possibili
    all_combinations = list(itertools.product(
        BUDGET_VALS, GEOM_VALS, SET_ASIDE_VALS, BUY_VALS, SELL_VALS
    ))

    total_experiments = len(all_combinations)
    to_run = []

    # Filtra quelli già eseguiti
    for combo in all_combinations:
        budget, geom, set_aside, buy, sell = combo
        rounded_combo = (
            round(float(budget), 2),
            round(float(geom), 2),
            round(float(set_aside), 2),
            round(float(buy), 2),
            round(float(sell), 2)
        )
        
        if rounded_combo not in completed:
            to_run.append(combo)

    print(f"Da eseguire: {len(to_run)} / {total_experiments} totali (ne sono stati saltati {total_experiments - len(to_run)}).")
    print("Inizio batteria di test intelligente...")

    # Esegui solo quelli rimasti
    for i, combo in enumerate(to_run, 1):
        budget, geom, set_aside, buy, sell = combo
        print("==========================================================")
        print(f"[{i}/{len(to_run)}] Esecuzione test con: Budget={budget}, BuyTH={buy}, SellTH={sell}, Geom={geom}, SetAside={set_aside}")
        
        cmd = [
            EXECUTABLE,
            "--budget", str(budget),
            "--geom", str(geom),
            "--buy", str(buy),
            "--sell", str(sell),
            "--set-aside", str(set_aside),
            "--simulator"
        ]
        
        # Eseguiamo attendendo la fine (senza shell) e stampando a video eventuali errori se necessario
        subprocess.run(cmd)

    print("==========================================================")
    print("Tutti i test completati! Controlla il file data/esperimenti.csv e apri plot_results.py per visualizzarli.")

if __name__ == "__main__":
    main()
