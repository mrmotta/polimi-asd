// +---------------------------------------+
// |                                       |
// | Progetto realizzato da Riccardo Motta |
// |                                       |
// +---------------------------------------+

#include <stdio.h>
#include <stdlib.h>

// Numero di grafi oltre cui controllare se i grafi sono messi in ordine decrescente
#define CONTROLLO_DECRESCENTE 16

// Definizione del tipo boolean
typedef enum {false, true} boolean;

// Struttura dati per la classifica e per i nodi dei grafi
typedef struct {
	int identificativo;
	unsigned long long int peso;
} nodo_t;

// Prototipi delle funzioni legate ad 'AggiungiGrafo'
void AggiungiGrafo (void);
void Inizializzazione (void);
void Dijkstra (void);
void MinMucchifica (unsigned int padre);

// Prototipi delle funzioni legate a 'TopK'
void TopK (void);
void MaxMucchifica (unsigned int padre);
void Inserimento (unsigned long long int peso);

// Funzioni per ordinare l'output del programma
#ifdef SORT
void Quicksort (int inizio, int fine);
int Partition (int inizio, int fine);
#endif

// Variabili globali di configurazione
boolean decrescente = false;
unsigned int lunghezzaClassifica = 0U;
unsigned int lunghezzaMassimaClassifica;
unsigned int lunghezzaMassimaClassificaCorrente;
unsigned int numeroGrafi = 0U;
unsigned int numeroNodi;
unsigned int numeroNodiDaProcessare;
unsigned int colonne;
unsigned int penultimaColonna;

// Puntatori ai vettori utilizzati
nodo_t *classifica;
nodo_t *nodiDaProcessare;
unsigned long long int *percorsiMinimi;
unsigned long int *matrice;
unsigned int *posizionePercorsiMinimi;

// Puntatore alla classifica per la stampa ordinata dei valori
#ifdef SORT
unsigned int *classificaOrdinata;
#endif

int main () {

	char *indiceLettura;
	char carattereLetto;
	char numero[11];

	// Configurazione dell'ambiente: lettura del numero di nodi
	indiceLettura = numero;
	carattereLetto = getchar_unlocked();

	do {
		*indiceLettura = carattereLetto;
		++ indiceLettura;
		carattereLetto = getchar_unlocked();
	} while (carattereLetto != ' ');

	*indiceLettura = '\0';

	numeroNodi = atoi(numero);

	// Configurazione dell'ambiente: lettura della lunghezza massima della classifica
	indiceLettura = numero;
	carattereLetto = getchar_unlocked();

	do {
		*indiceLettura = carattereLetto;
		++ indiceLettura;
		carattereLetto = getchar_unlocked();
	} while (carattereLetto != '\n');

	*indiceLettura = '\0';

	lunghezzaMassimaClassifica = atoi(numero);

	// Calcolo della lunghezza iniziale della classifica
	if (lunghezzaMassimaClassifica <= 16U)
		lunghezzaMassimaClassificaCorrente = lunghezzaMassimaClassifica;
	else
		if (lunghezzaMassimaClassifica > 16U)
			lunghezzaMassimaClassificaCorrente = 16U;

	// Inizializzazione di variabili ricorrenti e statiche
	colonne = numeroNodi - 1;
	penultimaColonna = colonne - 1;

	// Creazione dei vettori utili durante il processo
	classifica = malloc(sizeof(nodo_t) * lunghezzaMassimaClassificaCorrente);
	nodiDaProcessare = malloc(sizeof(nodo_t) * colonne);
	percorsiMinimi = malloc(sizeof(unsigned long long int) * colonne);
	matrice = malloc(sizeof(unsigned long int) * colonne * colonne);
	posizionePercorsiMinimi = malloc(sizeof(int) * colonne);

	#ifdef SORT
	classificaOrdinata = malloc(sizeof(unsigned int) * lunghezzaMassimaClassifica);
	#endif

	// Lettura ed esecuzione delle istruzioni
	carattereLetto = getchar_unlocked();
	do {
		if (carattereLetto == 'A')
			AggiungiGrafo();
		else
			TopK();
		carattereLetto = getchar_unlocked();
	} while (carattereLetto != EOF);

	return 0;
}

void AggiungiGrafo () {

	int indice = 0;
	unsigned long long int pesoTotale = 0U;

	if (decrescente) {

		// Aggiornamento della lunghezza della classifica (se necessario)
		if (lunghezzaClassifica < lunghezzaMassimaClassifica)
			++ lunghezzaClassifica;
		
		// Spostamento al prossimo comando da eseguire
		for (int indice = 0; indice <= numeroNodi; ++ indice)
			while (getchar_unlocked() != '\n');

	} else {

		// Inizializzazione dei vettori e calcolo del primo nodo da processare
		Inizializzazione();

		// Dijkstra, eseguito solo se parte almeno un arco dal nodo sorgente
		if (nodiDaProcessare[0].peso != 0U) {

			Dijkstra();

			// Calcolo del peso totale del grafo
			do {
				pesoTotale += percorsiMinimi[indice];
				++ indice;
			} while (indice < colonne);

		} else
			pesoTotale = 0U;

		// Aggiunta del nodo alla classifica
		if (lunghezzaClassifica == lunghezzaMassimaClassifica) {

			if (pesoTotale < classifica[0].peso) {

				// Cancellazione del massimo
				classifica[0] = classifica[-- lunghezzaClassifica];
				MaxMucchifica(0);
				
				// Inserimento del nodo
				Inserimento(pesoTotale);
			}
		} else Inserimento(pesoTotale);
	}

	// Aggiornamento del numero di grafi letti fin'ora
	++ numeroGrafi;
}

void Inizializzazione () {

	int riga = 0;
	int indiceMatrice = 0;
	unsigned long int pesoArco;
	char *indiceLettura;
	char carattereLetto;
	char numero[11];

	numeroNodiDaProcessare = colonne;

	// Inizializzazione vettore dei percorsi minimi
	while (getchar_unlocked() != ',');
	for (int indice = 0; indice < numeroNodiDaProcessare; ++ indice) {

		// Lettura del numero
		indiceLettura = numero;
		carattereLetto = getchar_unlocked();

		if (indice != penultimaColonna)

			do {
				*indiceLettura = carattereLetto;
				++ indiceLettura;
				carattereLetto = getchar_unlocked();
			} while (carattereLetto != ',');

		else

			do {
				*indiceLettura = carattereLetto;
				++ indiceLettura;
				carattereLetto = getchar_unlocked();
			} while (carattereLetto != '\n');

		*indiceLettura = '\0';

		pesoArco = atol(numero);

		// Inizializzazione dei vettori
		percorsiMinimi[indice] = pesoArco;
		nodiDaProcessare[indice].peso = pesoArco;
		nodiDaProcessare[indice].identificativo = indice;
		posizionePercorsiMinimi[indice] = indice;
	}

	// Costruzione del MinHeap dei nodi
	for (int indice = (penultimaColonna - 1) / 2; indice >= 0; -- indice)
		MinMucchifica(indice);
	
	// Inizializzazione della matrice di adiacenza
	do {

		// Spostamento al primo nodo d'interesse
		while (getchar_unlocked() != ',');

		for (int colonna = 0; colonna < colonne; ++ colonna, ++ indiceMatrice) {

			// Lettura del numero
			indiceLettura = numero;
			carattereLetto = getchar_unlocked();

			if (colonna != penultimaColonna)

				do {
					*indiceLettura = carattereLetto;
					++ indiceLettura;
					carattereLetto = getchar_unlocked();
				} while (carattereLetto != ',');

			else

				do {
					*indiceLettura = carattereLetto;
					++ indiceLettura;
					carattereLetto = getchar_unlocked();
				} while (carattereLetto != '\n');

			*indiceLettura = '\0';

			matrice[indiceMatrice] = atol(numero);
		}

		++ riga;

	} while (riga < colonne);
}

void Dijkstra () {

	nodo_t tmp;
	unsigned int iniziale;
	unsigned int corrente;
	unsigned long int pesoArco;

	do {

		// Per ogni nodo ancora nella lista controllo se devo aggiornare il percorso minimo
		for (int nodo = 1; nodo < numeroNodiDaProcessare; ++ nodo) {

			iniziale = nodiDaProcessare[0].identificativo;
			corrente = nodiDaProcessare[nodo].identificativo;
			pesoArco = matrice[iniziale * colonne + corrente];

			// Controllo se devo aggiornare il percorso minimo
			if (pesoArco != 0 && (percorsiMinimi[corrente] == 0 || percorsiMinimi[iniziale] + pesoArco < percorsiMinimi[corrente])) {

				percorsiMinimi[corrente] = percorsiMinimi[iniziale] + pesoArco;

				// Decremento la priorità del nodo
				nodiDaProcessare[posizionePercorsiMinimi[corrente]].peso = percorsiMinimi[corrente];

				for (int indice = posizionePercorsiMinimi[corrente]; indice > 0 && (nodiDaProcessare[(indice - 1) / 2].peso > nodiDaProcessare[indice].peso || nodiDaProcessare[(indice - 1) / 2].peso == 0); indice = (indice - 1) / 2) {

					// Aggiustamento delle posizioni
					posizionePercorsiMinimi[nodiDaProcessare[indice].identificativo] = (indice - 1) / 2;
					posizionePercorsiMinimi[nodiDaProcessare[(indice - 1) / 2].identificativo] = indice;

					// Scambio dei valori
					tmp = nodiDaProcessare[(indice - 1) / 2];
					nodiDaProcessare[(indice - 1) / 2] = nodiDaProcessare[indice];
					nodiDaProcessare[indice] = tmp;
				}
			}
		}

		// Cancellazione del minimo
		nodiDaProcessare[0] = nodiDaProcessare[-- numeroNodiDaProcessare];
		MinMucchifica(0);

	} while (numeroNodiDaProcessare != 0 && nodiDaProcessare[0].peso != 0);
}

void MinMucchifica (unsigned int padre) {

	nodo_t tmp;
	unsigned int minimo = padre;
	unsigned int sinistra = 2 * padre + 1;
	unsigned int destra = 2 * padre + 2;

	// Ricerca del massimo tra i figli
	if (sinistra < numeroNodiDaProcessare && nodiDaProcessare[sinistra].peso != 0 && (nodiDaProcessare[sinistra].peso < nodiDaProcessare[minimo].peso || nodiDaProcessare[minimo].peso == 0))
		minimo = sinistra;
	if (destra < numeroNodiDaProcessare && nodiDaProcessare[destra].peso != 0 && (nodiDaProcessare[destra].peso < nodiDaProcessare[minimo].peso || nodiDaProcessare[minimo].peso == 0))
		minimo = destra;

	// Se non c'è nulla da scambiare ho finito
	if (minimo == padre)
		return;
	
	// Aggiustamento delle posizioni
	posizionePercorsiMinimi[nodiDaProcessare[padre].identificativo] = minimo;
	posizionePercorsiMinimi[nodiDaProcessare[minimo].identificativo] = padre;

	// Scambio dei valori
	tmp = nodiDaProcessare[padre];
	nodiDaProcessare[padre] = nodiDaProcessare[minimo];
	nodiDaProcessare[minimo] = tmp;

	// Chiamata ricorsiva per sistemare gli eventuali problemi
	MinMucchifica(minimo);
}

void TopK () {

	int indice = 0;

	// Aggiustamento della posizione del cursore per evitare problemi
	while (getchar_unlocked() != '\n');

	// Stampa dei valori all'interno della classifica a seconda che i grafi siano in ordine decrescente o no
	if (decrescente) {

		if (numeroGrafi - lunghezzaClassifica > 0)
			indice = numeroGrafi - lunghezzaClassifica;

		while (indice < numeroGrafi - 1) {
			printf("%d ", indice);
			++ indice;
		}
		printf("%d\n", indice);

	} else {

		// Stampa senza ordinamento
		#ifndef SORT

		// Se la classifica è vuota stampa soltanto un '\n', altrimenti li stampa nell'ordine in cui sono nel vettore
		if (lunghezzaClassifica != 0) {

			do {
				printf("%d ", classifica[indice].identificativo);
				++ indice;
			} while (indice < lunghezzaClassifica - 1);

			printf("%d\n", classifica[lunghezzaClassifica - 1].identificativo);

		} else
			printf("\n");

		#endif

		// Stampa con ordinamento
		#ifdef SORT

		// Copia gli identificativi dei nodi in un nuovo vettore
		do {
			classificaOrdinata[indice] = classifica[indice].identificativo;
			++ indice;
		} while (indice < lunghezzaClassifica);

		// Ordina questo nuovo vettore
		Quicksort(0, lunghezzaClassifica - 1);

		// Se la classifica è vuota stampa soltanto un '\n', altrimenti li stampa in ordine
		if (lunghezzaClassifica != 0) {

			indice = 0;

			do {
				printf("%d ", classificaOrdinata[indice]);
				++ indice;
			} while (indice < lunghezzaClassifica - 1);

			printf("%d\n", classificaOrdinata[lunghezzaClassifica - 1]);

		} else
			printf("\n");

		#endif
	}
}

void MaxMucchifica (unsigned int padre) {

	nodo_t tmp;
	unsigned int massimo = padre;
	unsigned int sinistra = 2 * padre + 1;
	unsigned int destra = 2 * padre + 2;

	// Ricerca del massimo tra i figli
	if (sinistra < lunghezzaClassifica && classifica[sinistra].peso > classifica[padre].peso)
		massimo = sinistra;
	if (destra < lunghezzaClassifica && classifica[destra].peso > classifica[massimo].peso)
		massimo = destra;
	
	// Se non c'è nulla da scambiare ho finito
	if (massimo == padre)
		return;

	// Scambio dei valori
	tmp = classifica[padre];
	classifica[padre] = classifica[massimo];
	classifica[massimo] = tmp;

	// Chiamata ricorsiva per sistemare gli eventuali problemi
	MaxMucchifica(massimo);
}

void Inserimento (unsigned long long int peso) {

	nodo_t tmp;

	// Controllo se i grafi hanno peso decrescente, così da ottimizzare la classifica
	if (numeroGrafi == CONTROLLO_DECRESCENTE) {

		// Dico che è decrescente e vedo se è da smentire
		decrescente = true;

		for (int indice = 0; indice < lunghezzaClassifica - 1; ++ indice)
			if (classifica[indice].identificativo > classifica[indice + 1].identificativo) {
				decrescente = false;
				return;
			}

		++ lunghezzaClassifica;

	} else {

		// Controlla che ci sia spazio, altrimenti rialloca
		if (lunghezzaClassifica == lunghezzaMassimaClassificaCorrente) {

			// Calcolo della lunghezza della nuova classifica
			if (lunghezzaMassimaClassificaCorrente * 2 <= lunghezzaMassimaClassifica)
				lunghezzaMassimaClassificaCorrente *= 2;
			else
				if (lunghezzaMassimaClassificaCorrente * 2 > lunghezzaMassimaClassifica)
					lunghezzaMassimaClassificaCorrente = lunghezzaMassimaClassifica;

			// Riallocazione dello spazio
			classifica = realloc(classifica, sizeof(nodo_t) * lunghezzaMassimaClassificaCorrente);
		}

		// Inserimento del nuovo valore
		classifica[lunghezzaClassifica].peso = peso;
		classifica[lunghezzaClassifica].identificativo = numeroGrafi;
		++ lunghezzaClassifica;

		// Risoluzione degli eventuali problemi
		for (int indice = lunghezzaClassifica-1; indice > 0 && classifica[(indice - 1) / 2].peso < classifica[indice].peso; indice = (indice - 1) / 2) {
			tmp = classifica[(indice - 1) / 2];
			classifica[(indice - 1) / 2] = classifica[indice];
			classifica[indice] = tmp;
		}
	}
}

#ifdef SORT

// Implementazione del Quicksort
void Quicksort (int inizio, int fine) {

	int pivot;

	if (inizio < fine) {

		pivot = Partition(inizio, fine);

		Quicksort(inizio, pivot);
		Quicksort(pivot + 1, fine);
	}
}

// Schema di partizione di Hoare
int Partition (int inizio, int fine) {

	unsigned int pivot = classificaOrdinata[(fine + inizio) / 2];
	int indice1 = inizio - 1;
	int indice2 = fine + 1;
	unsigned int tmp;

	while (true) {

		do
			++ indice1;
		while (classificaOrdinata[indice1] < pivot);

		do
			-- indice2;
		while (classificaOrdinata[indice2] > pivot);

		if (indice1 < indice2) {

			tmp = classificaOrdinata[indice1];
			classificaOrdinata[indice1] = classificaOrdinata[indice2];
			classificaOrdinata[indice2] = tmp;

		} else
			return indice2;
	}
}

#endif
