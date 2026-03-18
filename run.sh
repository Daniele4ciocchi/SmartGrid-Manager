#!/usr/bin/env bash
set -euo pipefail

# run.sh - trova ed esegue l'eseguibile 'programma' generato nella build
# Uso:
#   ./run.sh [--build] [--exe <path>] [--] [args...]
# - --build : esegue ./build.sh prima di lanciare
# - --exe <path> : percorso esplicito dell'eseguibile
# - args... : argomenti passati all'eseguibile

PROJ_ROOT="$(cd "$(dirname "${0}")" && pwd)"

DO_BUILD=false
EXE_OVERRIDE=""

POSITIONAL=()
while [[ ${#} -gt 0 ]]; do
  case "$1" in
    --build)
      DO_BUILD=true; shift ;;
    --initdb)
      POSITIONAL+=("--initdb"); shift ;;
    --simulator)
      POSITIONAL+=("--simulator"); shift ;;
    --exe)
      EXE_OVERRIDE="$2"; shift 2 ;;
    --)
      shift; POSITIONAL+=("${@}"); break ;;
    -*|--*)
      echo "Unknown option: $1" >&2; exit 2 ;;
    *)
      POSITIONAL+=("$1"); shift ;;
  esac
done

set -- "${POSITIONAL[@]}"

if $DO_BUILD ; then
  echo "Eseguo build..."
  (cd "$PROJ_ROOT" && ./build.sh)
fi

if [[ -n "$EXE_OVERRIDE" ]]; then
  EXE_PATH="$EXE_OVERRIDE"
else
  # predefinito: eseguibile nella sottocartella build/simulatore/main
  DEFAULT_EXE_A="$PROJ_ROOT/build/simulatore/main/programma"
  DEFAULT_EXE_B="$PROJ_ROOT/build/simulatore/main/simulatore"
  if [[ -x "$DEFAULT_EXE_A" ]]; then
    EXE_PATH="$DEFAULT_EXE_A"
  elif [[ -x "$DEFAULT_EXE_B" ]]; then
    EXE_PATH="$DEFAULT_EXE_B"
  else
    # fallback: cerca in percorsi comuni
    CANDIDATES=(
      "$PROJ_ROOT/build/simulatore/main/programma"
      "$PROJ_ROOT/build/simulatore/programma"
      "$PROJ_ROOT/build/programma"
    )
    EXE_PATH=""
    for c in "${CANDIDATES[@]}"; do
      if [[ -x "$c" ]]; then
        EXE_PATH="$c"
        break
      fi
    done
  fi
fi

if [[ -z "$EXE_PATH" ]]; then
  echo "Impossibile trovare l'eseguibile 'programma'. Prova a lanciare con --build o passare --exe <path>" >&2
  exit 1
fi

echo "Eseguo: $EXE_PATH $*"
# esegui dall root del progetto per preservare path relativi ai dati
cd "$PROJ_ROOT"
exec "$EXE_PATH" "$@"
