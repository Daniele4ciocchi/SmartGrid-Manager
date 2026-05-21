import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

CSV_PATH = "data/esperimenti.csv"


def load_data(filepath: str = CSV_PATH) -> pd.DataFrame | None:
    """Carica il CSV degli esperimenti in un DataFrame pandas."""
    try:
        df = pd.read_csv(filepath)
    except FileNotFoundError:
        print(f"Errore: il file '{filepath}' non è stato trovato. Esegui prima il simulatore.")
        return None

    if df.empty:
        print("Il file è vuoto: nessun esperimento da mostrare.")
        return None

    # Elimina eventuali righe di header duplicate
    df = df[df["Strategia"] != "Strategia"].copy()

    # Converte le colonne numeriche
    num_cols = [
        "Finestra Geometrica",
        "Finestra Simulazione",
        "Budget",
        "Initial Balance",
        "Set Aside %",
        "Buy Threshold",
        "Sell Threshold",
        "Average Yield (%)",
        "Standard Deviation (%)",
    ]
    for col in num_cols:
        if col in df.columns:
            df[col] = pd.to_numeric(df[col], errors="coerce")

    # Elimina eventuali righe non numeriche
    df = df.dropna(subset=["Average Yield (%)", "Buy Threshold", "Sell Threshold", "Budget"])

    if df.empty:
        print("Dopo la pulizia non rimangono righe valide.")
        return None

    return df


def plot_results(df: pd.DataFrame, out_path: str = "data/grafico_setaside.png") -> None:
    """Grafico elegante incentrato sul ruolo della percentuale di set aside."""

    sns.set_theme(style="whitegrid", palette="muted")

    plot_df = df.reset_index(drop=True).copy()

    fig, ax = plt.subplots(figsize=(10, 6))

    if "Set Aside %" in plot_df.columns:
        # Crea un pointplot (linea + punti con aggregazione media e intervallo di confidenza)
        # Sull'asse X mettiamo la percentuale di Set Aside
        sns.pointplot(
            data=plot_df,
            x="Set Aside %",
            y="Average Yield (%)",
            hue="Buy Threshold",
            dodge=True,
            markers=["o", "s", "D", "v"],
            linestyles=["-", "--", "-.", ":"],
            ax=ax
        )
        
        ax.set_title("Impatto del Set Aside % sul Rendimento Medio", fontsize=14, weight='bold')
        ax.set_xlabel("Set Aside (%)", fontsize=12)
        ax.set_ylabel("Rendimento Medio (%)", fontsize=12)
        ax.legend(title="Soglia Acquisto (Buy Threshold)")
    else:
        # Fallback al grafico originale se non c'è Set Aside %
        plot_df["Esperimento"] = plot_df.index + 1
        ax.plot(
            plot_df["Esperimento"],
            plot_df["Average Yield (%)"],
            color="#1f77b4",
            linewidth=1.8,
            marker="o"
        )
        ax.set_title("Rendimento medio per esperimento")
        ax.set_xlabel("Esperimento")
        ax.set_ylabel("Rendimento medio (%)")

    plt.tight_layout()
    fig.savefig(out_path, dpi=300, bbox_inches='tight')
    print(f"Grafico salvato in: {out_path}")
    plt.show()


if __name__ == "__main__":
    df = load_data()
    if df is not None:
        plot_results(df)
