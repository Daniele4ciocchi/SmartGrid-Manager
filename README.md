# SmartGridManager

this project talk about the buy/selling of energy. This is a simple experiment to dimostrate that a structured algorithm based 
on smart choise can make more money than a random algorithm

to build and start the simulator you can run this command in the main directory 

./run.sh --build

## random choice 
prende valori a caso e casualmente sul momento decide se vendere o comprare valori casuali di energia, il profitto medio di questa funzione si trova sui 3000 €

## personal choice
si basa sulla media e la deviazione standard, principalmente ogni volta che si riceve un nuovo prezzo si guarda la media e la deviazione standard precedenti per capire se vendere o comprare energia, 
dopodichè viene ricalcolata la media e la devizione standard per il prezzo attuale e viene salvata, il profitto di questa funzione si trova sui 1500€

## considerazioni personali 
si può notare come la compravendita casuale porti ad un profitto maggiore della compravendita pensata tramite valori calcolati