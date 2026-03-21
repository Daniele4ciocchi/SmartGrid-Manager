#!/usr/bin/env bash
set -euo pipefail

# run.sh - trova ed esegue l'eseguibile principale generato nella build
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
  # eseguibile di default generato da CMake
  EXE_PATH="$PROJ_ROOT/build/simulatore/main/simulatore"
fi

if [[ ! -x "$EXE_PATH" ]]; then
  echo "Impossibile trovare l'eseguibile principale ($EXE_PATH)." >&2
  echo "Esegui ./run.sh --build oppure specifica il percorso con --exe <path>." >&2
  exit 1
fi

echo "Eseguo: $EXE_PATH $*"
# esegui dall root del progetto per preservare path relativi ai dati
cd "$PROJ_ROOT"
exec "$EXE_PATH" "$@"
