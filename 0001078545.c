/* Barzanti Francesco, 0001078545, gruppo A, francesco.barzanti@studio.unibo.it */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* Struttura per rappresentare un punto con coordinate x e y */
typedef struct {
    int x, y;
} Point;

/* Dichiarazione variabili globali */
int **H;                        /* Matrice delle altezze */
long int **cost;                /* Matrice dei costi per raggiungere ogni punto */
Point **parent;                 /* Matrice per tracciare il percorso */
int n, m, Ccell, Cheight;       /* Dimensioni della matrice e costi di percorso */
int **visited;                  /* Matrice per tracciare i punti visitati */

/* Funzione per leggere il file di input */
void leggi_file(char *filename)
{
    FILE *file;
    int i, j;
    
    /* Apertura del file */
    file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Impossibile aprire il file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    /* Lettura dei costi e delle dimensioni della matrice */
    fscanf(file, "%d", &Ccell);
    fscanf(file, "%d", &Cheight);
    fscanf(file, "%d", &n);
    fscanf(file, "%d", &m);

    /* Allocazione dinamica delle matrici */
    H = (int **)malloc(n * sizeof(int *));
    cost = (long int **)malloc(n * sizeof(long int *));
    parent = (Point **)malloc(n * sizeof(Point *));
    visited = (int **)malloc(n * sizeof(int *));
    for (i = 0; i < n; i++) {
        H[i] = (int *)malloc(m * sizeof(int));
        cost[i] = (long int *)malloc(m * sizeof(long int));
        parent[i] = (Point *)malloc(m * sizeof(Point));
        visited[i] = (int *)malloc(m * sizeof(int));
    }

    /* Lettura delle altezze nella matrice */
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            fscanf(file, "%d", &H[i][j]);
        }
    }

    /* Chiusura del file */
    fclose(file);
}

/* Funzione per calcolare il costo del percorso tra due punti */
long int costo_percorso(Point from, Point to) 
{
    int heightDiff = H[from.x][from.y] - H[to.x][to.y];
    return Ccell + Cheight * heightDiff * heightDiff;
}

/* Funzione che implementa l'algoritmo di Dijkstra */
void dijkstra() 
{
    int dx[] = {1, 0, -1, 0};        /* Spostamenti (destra, su , sinistra, giù) */
    int dy[] = {0, -1, 0, 1};
    int x, y, dir, nx, ny;           /* Variabili per le coordinate attuali per le direzioni e per le nuove posizioni */
    Point from, to;                  /* Struttura Point per rappresentare da che punto a che punto */
    long int nuovoCostoPercorso;     /* Variabile per il nuovo costo del percorso */
    
    /* Inizializzazione delle matrici */
    for (x = 0; x < n; x++) {
        for (y = 0; y < m; y++) {
            cost[x][y] = LONG_MAX;
            visited[x][y] = 0;
        }
    }

    cost[0][0] = Ccell;  /* Il costo del punto di partenza */

    while (1) {
        /* Trova il punto non visitato con il costo minimo */
        Point minPoint = {-1, -1};
        long int minCost = LONG_MAX;
        for (x = 0; x < n; x++) {
            for (y = 0; y < m; y++) {
                if (!visited[x][y] && cost[x][y] < minCost) {
                    minCost = cost[x][y];
                    minPoint.x = x;
                    minPoint.y = y;
                }
            }
        }

        if (minPoint.x == -1) break; /* Se non ci sono punti non visitati, esci dal ciclo */

        x = minPoint.x;
        y = minPoint.y;
        visited[x][y] = 1;    /* Segna il punto come visitato */
        /* Aggiorna i costi dei punti adiacenti */
        for (dir = 0; dir < 4; dir++) {
            nx = x + dx[dir];
            ny = y + dy[dir];
            if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
                from.x = x;
                from.y = y;
                to.x = nx;
                to.y = ny;
                nuovoCostoPercorso = cost[x][y] + costo_percorso(from, to);
                if (nuovoCostoPercorso < cost[nx][ny]) {
                    cost[nx][ny] = nuovoCostoPercorso;
                    parent[nx][ny] = from;    /* Aggiorna il percorso */
                }
            }
        }
    }
}

/* Funzione per stampare il percorso minimo e il costo totale */
void stampa_percorso() 
{
    Point *path = (Point *)malloc(n * m * sizeof(Point));
    int lunghezzaPercorso = 0;
    Point at;
    int i;
    
    /* Partendo dal punto finale, risaliamo fino al punto di partenza */
    at.x = n - 1;
    at.y = m - 1;

    while (at.x != 0 || at.y != 0) {
        path[lunghezzaPercorso++] = at;
        at = parent[at.x][at.y];
    }
    at.x = 0;
    at.y = 0;
    path[lunghezzaPercorso++] = at;

    /* Stampa il percorso in ordine inverso (dal punto di partenza al punto finale) */
    for (i = lunghezzaPercorso - 1; i >= 0; i--) {
        printf("%d %d\n", path[i].x, path[i].y);
    }
    printf("-1 -1\n");                    /* Indica la fine del percorso */
    printf("%ld\n", cost[n-1][m-1]);       /* Stampa il costo totale del percorso */

    free(path);  /* Libera la memoria allocata per il percorso */
}

/* Funzione principale */
int main(int argc, char *argv[]) 
{
	int i=0;
	
    if (argc != 2) {
        fprintf(stderr, "Errore: %s <input file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    leggi_file(argv[1]);    /* Legge i dati dal file di input */
    dijkstra();             /* Esegue l'algoritmo di Dijkstra per trovare il percorso minimo */ 
    stampa_percorso();      /* Stampa il percorso e il costo totale */

    /* Dealloca la memoria */
    for (i = 0; i < n; i++) {
        free(H[i]);
        free(cost[i]);
        free(parent[i]);
        free(visited[i]);
    }
    free(H);
    free(cost);
    free(parent);
    free(visited);

    return EXIT_SUCCESS;
}
