import os
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import pandas as pd

ROOT = Path(__file__).resolve().parent
CSV_PATHS = [ROOT / "data" / "esperimenti.csv", ROOT / "data" / "esperimenti.old.csv"]
OUTPUT_PATH = ROOT / "data" / "random_vs_smart_confronto.png"


def load_experiments() -> pd.DataFrame:
    """Carica i risultati da uno o più file CSV di esperimenti."""
    frames = []
    for csv_path in CSV_PATHS:
        if not csv_path.exists():
            continue

        df = pd.read_csv(csv_path)
        if df.empty:
            continue

        df = df[df["Strategia"] != "Strategia"].copy()

        numeric_cols = [
            "Finestra Geometrica",
            "Finestra Simulazione",
            "Budget",
            "Initial Balance",
            "Set Aside %",
            "Buy Threshold",
            "Sell Threshold",
            "Average Yield (%)",
            "Standard Deviation (%)",
            "Guadagno Finale Medio",
        ]
        for col in numeric_cols:
            if col in df.columns:
                df[col] = pd.to_numeric(df[col], errors="coerce")

        df = df.dropna(subset=["Strategia", "Set Aside %", "Average Yield (%)"])
        frames.append(df)

    if not frames:
        raise FileNotFoundError("Nessun file di esperimenti trovato. Controlla data/esperimenti.csv o data/esperimenti.old.csv.")

    return pd.concat(frames, ignore_index=True)


def build_comparison_table(df: pd.DataFrame) -> pd.DataFrame:
    """Confronta random e smart aggregando per lo stesso valore di Set Aside."""
    random_df = df[df["Strategia"].str.lower().str.contains("random", na=False)].copy()
    smart_df = df[df["Strategia"].str.lower().str.contains("smart", na=False)].copy()

    if random_df.empty or smart_df.empty:
        raise ValueError("Serve almeno un risultato random e uno smart per costruire il confronto.")

    random_by_setaside = (
        random_df.groupby("Set Aside %", as_index=False)["Average Yield (%)"].mean().rename(columns={"Average Yield (%)": "Average Yield (%)_random"})
    )
    smart_by_setaside = (
        smart_df.groupby("Set Aside %", as_index=False)["Average Yield (%)"].mean().rename(columns={"Average Yield (%)": "Average Yield (%)_smart"})
    )

    comparison = random_by_setaside.merge(smart_by_setaside, on="Set Aside %", how="inner").sort_values("Set Aside %").reset_index(drop=True)
    if comparison.empty:
        raise ValueError("Nessuna coppia random-smart trovata per lo stesso set aside.")

    comparison["delta_percentuale"] = comparison["Average Yield (%)_smart"] - comparison["Average Yield (%)_random"]
    comparison["delta_pct_label"] = comparison["delta_percentuale"].map(lambda v: f"{v:+.2f}%")
    return comparison


def plot_comparison(comparison: pd.DataFrame) -> None:
    """Crea un grafico a barre con il confronto tra random e smart per ogni set aside."""
    plt.style.use("seaborn-v0_8-whitegrid")

    set_aside_values = comparison["Set Aside %"].astype(int).tolist()
    random_values = comparison["Average Yield (%)_random"].tolist()
    smart_values = comparison["Average Yield (%)_smart"].tolist()
    delta_values = comparison["delta_percentuale"].tolist()

    x = range(len(set_aside_values))
    width = 0.35

    fig, axes = plt.subplots(2, 1, figsize=(12, 10), constrained_layout=True)
    axes[0].bar([i - width / 2 for i in x], random_values, width=width, label="Random", color="#4C78A8")
    axes[0].bar([i + width / 2 for i in x], smart_values, width=width, label="Smart", color="#F58518")
    axes[0].set_xticks(list(x))
    axes[0].set_xticklabels([str(v) for v in set_aside_values])
    axes[0].set_xlabel("Set Aside (%)")
    axes[0].set_ylabel("Average Yield (%)")
    axes[0].set_title("Confronto Random vs Smart per Set Aside")
    axes[0].legend()

    axes[1].bar(list(x), delta_values, color=["#54A24B" if v >= 0 else "#E45756" for v in delta_values])
    axes[1].axhline(0, color="black", linewidth=1, linestyle="--")
    axes[1].set_xticks(list(x))
    axes[1].set_xticklabels([str(v) for v in set_aside_values])
    axes[1].set_xlabel("Set Aside (%)")
    axes[1].set_ylabel("Delta Smart - Random (%)")
    axes[1].set_title("Vantaggio del metodo Smart rispetto al Random")

    for ax in axes:
        ax.grid(True, axis="y", linestyle="--", alpha=0.4)

    fig.suptitle("Confronto per Set Aside", fontsize=16, weight="bold")
    fig.savefig(OUTPUT_PATH, dpi=300, bbox_inches="tight")
    print(f"Grafico salvato in: {OUTPUT_PATH}")


def print_summary(comparison: pd.DataFrame) -> None:
    """Stampa il confronto random-vs-smart per ogni set aside."""
    print("\nConfronto Random vs Smart per stesso Set Aside")
    print("-" * 90)
    print(f"{'Set Aside %':>12} {'Random (%)':>12} {'Smart (%)':>12} {'Delta (%)':>12}")
    for _, row in comparison.iterrows():
        set_aside = int(row["Set Aside %"])
        random_val = row["Average Yield (%)_random"]
        smart_val = row["Average Yield (%)_smart"]
        delta = row["delta_percentuale"]
        print(f"{set_aside:>12} {random_val:>12.2f} {smart_val:>12.2f} {delta:>12.2f}")


if __name__ == "__main__":
    df = load_experiments()
    comparison = build_comparison_table(df)
    print_summary(comparison)
    plot_comparison(comparison)
