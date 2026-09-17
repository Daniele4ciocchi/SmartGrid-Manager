from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import pandas as pd

ROOT = Path(__file__).resolve().parent
CSV_PATHS = [ROOT / "data" / "esperimenti.csv", ROOT / "data" / "esperimenti.old.csv"]


def load_data() -> pd.DataFrame:
    frames = []
    for csv_path in CSV_PATHS:
        if not csv_path.exists():
            continue
        df = pd.read_csv(csv_path)
        if df.empty:
            continue
        df = df[df["Strategia"] != "Strategia"].copy()
        for col in [
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
        ]:
            if col in df.columns:
                df[col] = pd.to_numeric(df[col], errors="coerce")
        df = df.dropna(subset=["Strategia", "Set Aside %", "Average Yield (%)"])
        df["Strategia"] = df["Strategia"].astype(str)
        df["Algoritmo"] = df["Strategia"].str.lower().map(
            lambda s: "Random" if "random" in s else "Smart" if "smart" in s else "Altro"
        )
        frames.append(df)
    if not frames:
        raise FileNotFoundError("Nessun file di esperimenti trovato.")
    return pd.concat(frames, ignore_index=True)


def save_boxplot(df: pd.DataFrame) -> str:
    by_strategy = [
        df[df["Algoritmo"] == "Random"]["Average Yield (%)"],
        df[df["Algoritmo"] == "Smart"]["Average Yield (%)"],
    ]
    fig, ax = plt.subplots(figsize=(8, 6))
    box = ax.boxplot(by_strategy, patch_artist=True, widths=0.5)
    colors = ["#4C78A8", "#F58518"]
    for patch, color in zip(box["boxes"], colors):
        patch.set_facecolor(color)
        patch.set_alpha(0.8)
    ax.set_xticks([1, 2])
    ax.set_xticklabels(["Random", "Smart"])
    ax.set_title("Distribuzione del rendimento medio per algoritmo")
    ax.set_ylabel("Average Yield (%)")
    ax.grid(True, axis="y", linestyle="--", alpha=0.4)
    out = ROOT / "data" / "boxplot_random_vs_smart.png"
    fig.tight_layout()
    fig.savefig(out, dpi=300, bbox_inches="tight")
    plt.close(fig)
    return str(out)


def save_by_setaside(df: pd.DataFrame) -> str:
    summary = (
        df.groupby(["Algoritmo", "Set Aside %"], as_index=False)["Average Yield (%)"].mean()
    )
    fig, ax = plt.subplots(figsize=(10, 6))
    for alg, color in [("Random", "#4C78A8"), ("Smart", "#F58518")]:
        subset = summary[summary["Algoritmo"] == alg].sort_values("Set Aside %")
        ax.plot(
            subset["Set Aside %"],
            subset["Average Yield (%)"],
            marker="o",
            linewidth=2,
            label=alg,
            color=color,
        )
    ax.set_title("Rendimento medio al variare del Set Aside")
    ax.set_xlabel("Set Aside (%)")
    ax.set_ylabel("Average Yield (%)")
    ax.legend()
    ax.grid(True, linestyle="--", alpha=0.4)
    out = ROOT / "data" / "setaside_random_vs_smart.png"
    fig.tight_layout()
    fig.savefig(out, dpi=300, bbox_inches="tight")
    plt.close(fig)
    return str(out)


def save_by_buy_threshold(df: pd.DataFrame) -> str:
    summary = (
        df.groupby(["Algoritmo", "Buy Threshold"], as_index=False)["Average Yield (%)"].mean()
        .sort_values(["Algoritmo", "Buy Threshold"])
    )
    fig, ax = plt.subplots(figsize=(10, 6))
    for alg, color in [("Random", "#4C78A8"), ("Smart", "#F58518")]:
        subset = summary[summary["Algoritmo"] == alg]
        ax.plot(
            subset["Buy Threshold"],
            subset["Average Yield (%)"],
            marker="o",
            linewidth=2,
            label=alg,
            color=color,
        )
    ax.set_title("Sensibilità del rendimento medio alla soglia di acquisto")
    ax.set_xlabel("Buy Threshold")
    ax.set_ylabel("Average Yield (%)")
    ax.legend()
    ax.grid(True, linestyle="--", alpha=0.4)
    out = ROOT / "data" / "buythreshold_random_vs_smart.png"
    fig.tight_layout()
    fig.savefig(out, dpi=300, bbox_inches="tight")
    plt.close(fig)
    return str(out)


def save_delta_summary(df: pd.DataFrame) -> str:
    random = df[df["Algoritmo"] == "Random"].groupby("Set Aside %", as_index=False)["Average Yield (%)"].mean().rename(columns={"Average Yield (%)": "Random"})
    smart = df[df["Algoritmo"] == "Smart"].groupby("Set Aside %", as_index=False)["Average Yield (%)"].mean().rename(columns={"Average Yield (%)": "Smart"})
    comparison = random.merge(smart, on="Set Aside %").sort_values("Set Aside %")
    comparison["Delta"] = comparison["Smart"] - comparison["Random"]

    fig, ax = plt.subplots(figsize=(9, 6))
    ax.bar(comparison["Set Aside %"], comparison["Delta"], color=["#54A24B" if v >= 0 else "#E45756" for v in comparison["Delta"]])
    ax.axhline(0, color="black", linestyle="--", linewidth=1)
    ax.set_title("Vantaggio del metodo Smart rispetto al Random per Set Aside")
    ax.set_xlabel("Set Aside (%)")
    ax.set_ylabel("Delta Smart - Random (%)")
    ax.grid(True, axis="y", linestyle="--", alpha=0.4)
    out = ROOT / "data" / "delta_random_vs_smart.png"
    fig.tight_layout()
    fig.savefig(out, dpi=300, bbox_inches="tight")
    plt.close(fig)
    return str(out)


if __name__ == "__main__":
    df = load_data()
    outputs = [
        save_boxplot(df),
        save_by_setaside(df),
        save_by_buy_threshold(df),
        save_delta_summary(df),
    ]
    print("Grafici generati:")
    for p in outputs:
        print(" -", p)
