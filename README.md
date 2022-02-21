# Progetto di Algoritmi e Strutture Dati | A.A. 2020-2021

## Descrizione

L'obiettivo del progetto è la gestione di una classifica tra grafi diretti pesati. La classifica tiene traccia dei `k` "migliori" grafi.

Il programma da realizzare riceve in ingresso due parametri, una sola volta (sulla prima riga del file, separati da spazio)

- `d`: il numero di nodi dei grafi
- `k`: la lunghezza della classifica

ed una sequenza di comandi tra

- `AggiungiGrafo [matrice-di-adiacenza]`
- `TopK`

Nota: `d`, `k` e il numero di grafi sono rappresentabili con interi a 32 bit.

### `AggiungiGrafo`

Richiede di aggiungere un grafo a quelli considerati per stilare la classifica. È seguito dalla matrice di adiacenza del grafo stesso, stampata una riga per ogni rigo, con gli elementi separati da virgole.

I nodi del grafo sono da considerarsi etichettati logicamente con un indice intero tra `0` e `d - 1`; il nodo in posizione `0` è quello la cui stella uscente è descritta dalla prima riga della matrice.

I pesi degli archi del grafo elementi sono interi nell'intervallo `[0, 2^32 – 1]`.

### `TopK`

Si consideri ogni grafo dall'inizio del programma fino al comando `TopK` etichettato con un indice intero corrispondente al numero di grafi letti prima di esso (partendo da `0`).

`TopK` richiede al programma di stampare gli indici interi dei `k` grafi aventi i `k` valori più piccoli della seguente metrica: somma dei cammini più brevi tra il nodo `0` e tutti gli altri nodi del grafo raggiungibili da `0`.

Se ci sono più grafi con lo stesso valore della metrica, si dà la precedenza ai primi arrivati.

Le distanze dei nodi non raggiungibili da `0` sono considerate nulle.

I `k` indici interi sono stampati, su un unico rigo, separati da uno spazio, in un qualunque ordine.

## Panoramica

Sfida    | Limite di tempo | Limite di memoria | Punteggio totalizzato
-------- | --------------- | ----------------- | ---------------------
UpTo18   | 2.400 secondi   | 90.0 MiB          | 18/18
UpTo30   | 2.080 secondi   | 82.0 MiB          | 12/12
CumLaude | 1.700 secondi   |  1.0 MiB          | 1/1

Per la compilazione, il verificatore automatico ha eseguito questo comando:

    /usr/bin/gcc -DEVAL -Wall -Werror -std=gnu11 -O2 -pipe -static -s -o output input.c -lm

## Struttura

[`tests`](tests) contiene i casi di test pubblici che ci sono stati forniti direttamente dai docenti del corso.

[`commands`](commands) contiene alcuni script utili per l'analisi del programma. Sono presi e riadattati da quelli che ci sono stati suggeriti durante l'incontro di tutorato avvenuto il 2 giugno 2021.

[`testsuite.py`](testsuite.py) invece è uno script che ho realizzato per effettuare alcune analisi sul codice, verificando come risponde a determinati input, e per analizzare il tempo medio di esecuzione sulla propria macchina. La sintassi base per l'utilizzo è la seguente:

    testsuite.py source.c test_case

Tutte le opzioni sono disponibli eseguendo:

    testsuite.py -h

## Licenza

Tutto il codice è distributo in [licenza MIT](LICENSE).
