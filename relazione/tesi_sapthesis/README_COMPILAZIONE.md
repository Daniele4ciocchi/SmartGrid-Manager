# Relazione di tirocinio — Sapthesis

Questa cartella contiene la relazione convertita alla classe ufficiale `sapthesis` di Sapienza.

## Prima di compilare

Nel file `relazione.tex` sostituire:

```latex
\IDnumber{INSERIRE-MATRICOLA}
\authoremail{INSERIRE-EMAIL}
```

con la propria matricola e il proprio indirizzo e-mail.

## Compilazione da terminale

La relazione usa `pdflatex` e BibTeX. Dalla cartella del progetto:

```bash
pdflatex relazione.tex
bibtex8 relazione
pdflatex relazione.tex
pdflatex relazione.tex
```

In alternativa, se è installato `latexmk`:

```bash
latexmk -pdf relazione.tex
```

## VS Code

Il progetto contiene `.vscode/settings.json` per LaTeX Workshop. Aprire **la cartella del progetto**, non soltanto il file `.tex`, e compilare `relazione.tex`.

La recipe configurata è:

`pdflatex -> bibtex8 -> pdflatex x2`

## Errore `newtxtext.sty not found`

L'errore presente nel log fornito riguarda `sapthesis-doc.tex`, cioè la compilazione della documentazione della classe, non la relazione stessa. Quel file usa il pacchetto `newtxtext`.

Su Arch Linux il pacchetto che normalmente contiene `newtxtext.sty` è `texlive-fontsextra`:

```bash
sudo pacman -S texlive-fontsextra
```

Non è necessario compilare `sapthesis-doc.tex` per compilare la relazione. La relazione usa direttamente `sapthesis.cls`.
