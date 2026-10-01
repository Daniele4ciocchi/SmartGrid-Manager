#!/usr/bin/env python3
"""Produce una tabella LaTeX compatta e una figura Smart/Random."""

from __future__ import annotations

import argparse
from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


STRATEGY_COL = "Strategia"
COMMON_KEYS = [
    "Finestra Simulazione",
    "Budget",
    "Initial Balance",
    "Set Aside %",
]
SMART_KEYS = COMMON_KEYS + [
    "Finestra Geometrica",
    "Buy Threshold",
    "Sell Threshold",
]
YIELD_COL = "Average Yield (%)"
WINDOW_STD_COL = "Standard Deviation (%)"
GAIN_COL = "Guadagno Finale Medio"

NUMERIC_COLS = [
    "Finestra Geometrica",
    *COMMON_KEYS,
    "Buy Threshold",
    "Sell Threshold",
    YIELD_COL,
    WINDOW_STD_COL,
    GAIN_COL,
]


def load_results(path: Path) -> pd.DataFrame:
    if not path.is_file():
        raise FileNotFoundError(f"CSV non trovato: {path}")

    frame = pd.read_csv(path, skip_blank_lines=True)
    missing = {STRATEGY_COL, *NUMERIC_COLS} - set(frame.columns)
    if missing:
        raise ValueError(
            "Nel CSV mancano le colonne richieste: " + ", ".join(sorted(missing))
        )

    frame[STRATEGY_COL] = frame[STRATEGY_COL].astype(str).str.strip().str.lower()
    for column in NUMERIC_COLS:
        frame[column] = pd.to_numeric(frame[column], errors="coerce")

    frame = frame.dropna(subset=[STRATEGY_COL, *NUMERIC_COLS])
    return frame[frame[STRATEGY_COL].isin(
        {"randomchoise", "smartgeometricchoise"}
    )].copy()


def build_summary(frame: pd.DataFrame) -> tuple[pd.DataFrame, pd.DataFrame]:
    smart = frame[frame[STRATEGY_COL] == "smartgeometricchoise"]
    random = frame[frame[STRATEGY_COL] == "randomchoise"]

    if smart.empty:
        raise ValueError("Il CSV non contiene risultati smartgeometricChoise.")
    if random.empty:
        raise ValueError("Il CSV non contiene risultati randomChoise.")

    smart_summary = (
        smart.groupby(SMART_KEYS, as_index=False, dropna=False)
        .agg(
            **{
                "Smart yield (%)": (YIELD_COL, "mean"),
                "Smart window SD (%)": (WINDOW_STD_COL, "mean"),
            }
        )
    )
    random_summary = (
        random.groupby(COMMON_KEYS, as_index=False, dropna=False)
        .agg(
            **{
                "Random yield (%)": (YIELD_COL, "mean"),
                "Random simulation SD (%)": (WINDOW_STD_COL, "mean"),
                "Random repetitions": (YIELD_COL, "size"),
            }
        )
    )

    detailed = smart_summary.merge(
        random_summary, on=COMMON_KEYS, how="left", validate="many_to_one"
    )

    rows = []
    for set_aside in sorted(detailed["Set Aside %"].unique()):
        candidates = detailed[
            (detailed["Set Aside %"] == set_aside)
            & detailed["Random yield (%)"].notna()
        ].copy()
        if candidates.empty:
            continue

        # Riporta il miglior risultato osservato, mantenendo nel grafico
        # l'andamento di tutte le configurazioni Smart provate.
        best = candidates.loc[candidates["Smart yield (%)"].idxmax()]
        rows.append(
            {
                "Set Aside (%)": set_aside,
                "Random yield (%)": best["Random yield (%)"],
                "Random simulation SD (%)": best["Random simulation SD (%)"],
                "Random repetitions": int(best["Random repetitions"]),
                "Best Smart yield (%)": best["Smart yield (%)"],
                "Best Smart window SD (%)": best["Smart window SD (%)"],
                "Best Smart geometry": int(best["Finestra Geometrica"]),
                "Best Smart buy threshold": best["Buy Threshold"],
                "Difference (percentage points)": (
                    best["Smart yield (%)"] - best["Random yield (%)"]
                ),
            }
        )

    if not rows:
        raise ValueError(
            "Non ci sono percentuali Set Aside con risultati sia Smart sia Random."
        )

    return pd.DataFrame(rows), detailed


def format_decimal(value: float, digits: int = 3) -> str:
    return f"{value:.{digits}f}".replace(".", ",")


def save_summary(summary: pd.DataFrame, output_dir: Path) -> None:
    output_dir.mkdir(parents=True, exist_ok=True)

    csv_path = output_dir / "sintesi_smart_random.csv"
    summary.to_csv(csv_path, index=False, float_format="%.6f")

    tex_path = output_dir / "tabella_sintesi_smart_random.tex"
    lines = [
        r"\begin{table}[htbp]",
        r"\centering",
        r"\small",
        (
            r"\caption{Confronto tra Random e Smart a parità di Set Aside. "
            r"Per Random si riporta il rendimento medio e la deviazione standard "
            r"deviazione standard; per Smart la migliore configurazione osservata "
            r"e la deviazione standard dei rendimenti sulle finestre.}"
        ),
        r"\label{tab:sintesi-smart-random}",
        r"\resizebox{\textwidth}{!}{%",
        r"\begin{tabular}{ccccc}",
        r"\toprule",
        (
            r"Set Aside & Random: rendimento medio $\pm$ DS simulata "
            r"& Smart: rendimento (DS finestre) & Migliore configurazione Smart "
            r"& Differenza (p.p.) \\"
        ),
        r"\midrule",
    ]

    for _, row in summary.iterrows():
        random_sd = row["Random simulation SD (%)"]
        random_value = format_decimal(row["Random yield (%)"])
        if pd.notna(random_sd):
            random_value += r" $\pm$ " + format_decimal(random_sd)
        random_value += f" (n={int(row['Random repetitions'])})"

        smart_value = (
            format_decimal(row["Best Smart yield (%)"])
            + " ("
            + format_decimal(row["Best Smart window SD (%)"])
            + ")"
        )
        config = (
            f"{int(row['Best Smart geometry'])}; "
            f"{format_decimal(row['Best Smart buy threshold'], 2)}"
        )
        lines.append(
            f"{int(row['Set Aside (%)'])}\\% & {random_value} & {smart_value} "
            f"& {config} & {format_decimal(row['Difference (percentage points)'])} \\\\"
        )

    lines.extend(
        [
            r"\bottomrule",
            r"\end{tabular}%",
            r"}",
            r"\end{table}",
            "",
        ]
    )
    tex_path.write_text("\n".join(lines), encoding="utf-8")

    print(f"Tabella CSV salvata in {csv_path}")
    print(f"Tabella LaTeX pronta per \\input{{{tex_path.as_posix()}}}")


def save_figure(detailed: pd.DataFrame, output_dir: Path) -> None:
    available = detailed.dropna(subset=["Random yield (%)"])
    set_asides = sorted(available["Set Aside %"].unique())
    geometries = sorted(available["Finestra Geometrica"].unique())
    if not set_asides or not geometries:
        print("Nessuna coppia Smart/Random disponibile per il grafico.")
        return

    colors = ["#1b4965", "#ca6702", "#6a4c93", "#2a9d8f"]

    for set_aside in set_asides:
        fig, ax = plt.subplots(figsize=(6.8, 4.6), constrained_layout=True)
        subset = available[available["Set Aside %"] == set_aside]
        thresholds = sorted(subset["Buy Threshold"].unique())
        positions = range(len(thresholds))
        bar_width = 0.18
        for color_index, geom in enumerate(geometries):
            curve = (
                subset[subset["Finestra Geometrica"] == geom]
                .set_index("Buy Threshold")
                .reindex(thresholds)
            )
            if curve.empty:
                continue
            offset = (color_index - (len(geometries) - 1) / 2) * bar_width
            bars = ax.bar(
                [position + offset for position in positions],
                curve["Smart yield (%)"],
                width=bar_width,
                yerr=curve["Smart window SD (%)"],
                capsize=2.5,
                color=colors[color_index % len(colors)],
                edgecolor="white",
                linewidth=0.5,
                label=f"Finestra {int(geom)}",
            )
            for bar, value in zip(bars, curve["Smart yield (%)"]):
                ax.text(
                    bar.get_x() + bar.get_width() / 2,
                    bar.get_height() + curve["Smart window SD (%)"].iloc[0] + 0.35,
                    f"{value:.2f}",
                    ha="center",
                    va="bottom",
                    fontsize=6.5,
                    rotation=90,
                )

        baseline = subset.iloc[0]
        x_min = -0.5
        x_max = len(thresholds) - 0.5
        ax.axhline(
            baseline["Random yield (%)"],
            color="black",
            linestyle=(0, (5, 2)),
            linewidth=1.2,
            label=f"Random: {baseline['Random yield (%)']:.2f}%",
        )
        if pd.notna(baseline["Random simulation SD (%)"]):
            ax.axhspan(
                baseline["Random yield (%)"] - baseline["Random simulation SD (%)"],
                baseline["Random yield (%)"] + baseline["Random simulation SD (%)"],
                color="#555555",
                alpha=0.08,
                label=(
                    "Random: DS "
                    f"{baseline['Random simulation SD (%)']:.2f} punti percentuali"
                ),
            )

        ax.set_title(
            f"Rendimento medio per capitale accantonato pari al {set_aside:g}%",
            fontsize=11,
            pad=8,
        )
        ax.set_xlabel("Soglia di acquisto Smart", fontsize=9)
        ax.set_ylabel("Rendimento medio (%)", fontsize=9)
        ax.set_xticks(list(positions))
        ax.set_xticklabels([f"{threshold:.2f}" for threshold in thresholds])
        ax.set_xlim(x_min, x_max)
        ax.tick_params(axis="both", labelsize=8)
        ax.grid(axis="y", color="#b0b0b0", linewidth=0.5, alpha=0.55)
        ax.spines["top"].set_visible(False)
        ax.spines["right"].set_visible(False)
        ax.legend(
            fontsize=7.5,
            loc="upper left",
            ncol=2,
            frameon=False,
        )
        image_path = output_dir / f"figura_smart_random_setaside_{set_aside:g}.png"
        fig.savefig(image_path, dpi=300, bbox_inches="tight")
        fig.savefig(image_path.with_suffix(".pdf"), bbox_inches="tight")
        plt.close(fig)
        print(f"Figura salvata in {image_path}")


def main() -> None:
    parser = argparse.ArgumentParser(
        description=(
            "Genera una sintesi compatta Smart/Random per la relazione: "
            "una tabella e una figura."
        )
    )
    parser.add_argument(
        "--input",
        type=Path,
        default=Path("data/esperimenti.csv"),
        help="CSV prodotto dal simulatore (default: data/esperimenti.csv)",
    )
    parser.add_argument(
        "--output-dir",
        type=Path,
        default=Path("data"),
        help="Cartella dei risultati (default: data)",
    )
    args = parser.parse_args()

    results = load_results(args.input)
    summary, detailed = build_summary(results)
    save_summary(summary, args.output_dir)
    save_figure(detailed, args.output_dir)


if __name__ == "__main__":
    main()
