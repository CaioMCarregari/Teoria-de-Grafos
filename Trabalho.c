#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

typedef struct {
    float slenght;
    float swidth;
    float plenght;
    float pwidth;
} flor;

typedef struct {
    double MSL;
    double MSW;
    double MPL;
    double MPW;
} Media;

Media media[2] = {0};
flor flores[150] = {0};
float distancia[150][150] = {0};
float matriz[150][150] = {0};
int matriz_adjacente[150][150] = {0};
bool visitado[150] = {false};
int component[150] = {0};
int **grupos = NULL;
int vet_tam[150] = {0};

void abre_arquivo();
void salva_arquivo(int matriz_adjacente[150][150], float *demaior, float *demenor, float *denmaior, float *denmenor);
void distancia_euclidiana();
void distancia_normalizada(float *maior, float *menor);
void maior_menor(float *maior, float *menor, float matrix[150][150]);
void DFS(int matriz_adjacente[150][150], int u, bool visitado[150], int **grupoX, int *tam);
void centro_grupo(int *grupo, int index, int tam);
double acuracia(int *grupo1, int *grupo2, int tam1, int tam2);
float distancia_comp(int a, int i, int j);

int main() {
    float denmenor = 0, denmaior = 0, demaior = 0, demenor = 0, aux0 = 0, aux2 = 0;
    int u = 0, tamA = 0, i = 0, j = 0, cont = 0, indice = -1, maior = 0, indice_maior = 0, aux = -1, contador_setosas = 0;
    double acurache = 0;

    abre_arquivo();
    distancia_euclidiana();
    distancia_normalizada(&demaior, &demenor);
    maior_menor(&denmaior, &denmenor, matriz);

    for (i = 0; i < 149; i++) {
        for (j = i + 1; j < 150; j++) {
            if (matriz[i][j] <= 0.1) {
                matriz_adjacente[i][j] = 1;
                matriz_adjacente[j][i] = 1;
            }
        }
    }

    grupos = malloc(1 * sizeof(int*));

    while (cont != 150) {
        indice++;
        tamA = 0;
        grupos = realloc(grupos, (indice + 1) * sizeof(int *));
        grupos[indice] = NULL;
        DFS(matriz_adjacente, u, visitado, &grupos[indice], &tamA);
        vet_tam[indice] = tamA;
        cont += tamA;

        for (u = 0; u < 150; u++)
            if (!visitado[u]) break;
    }

    printf("DeMaior: %f  | DeMenor: %f\nDenMaior: %f | DenMenor: %f\n\n", demaior, demenor, denmaior, denmenor);

    for(int i = 0; i < 2; i++){
        maior = 0;
        for(j = 0; j <= indice; j++){
            if(j != aux){
                if(maior < vet_tam[j]){
                    maior = vet_tam[j];
                    indice_maior = i;
                }
            }
        }
        if (i != 1)
            aux = indice_maior;
        centro_grupo(grupos[indice_maior], i, vet_tam[indice_maior]);
    }

    printf("Grupos não agrupados: \n");
    for (int k = 0; k <= indice; k++) {
        printf("Grupo %d:\n", k + 1);
        for (i = 0; i < vet_tam[k]; i++)
            printf("%d ", grupos[k][i]);
        printf("\n");
    }

    for(i = 0; i <= indice; i++) {
        for(j = 0; j < vet_tam[i]; j++){
            if (i != aux && i != indice_maior){
                aux0 = distancia_comp(0, i, j);
                aux2 = distancia_comp(1, i, j);

                if (aux0 > aux2){
                    vet_tam[aux] += 1;
                    grupos[aux] = realloc(grupos[aux], vet_tam[aux] * sizeof(int));
                    grupos[aux][vet_tam[aux] - 1] = grupos[i][j];
                    centro_grupo(grupos[aux], 0, vet_tam[aux]);
                } else {
                    vet_tam[indice_maior] += 1;
                    grupos[indice_maior] = realloc(grupos[indice_maior], vet_tam[indice_maior] * sizeof(int));
                    grupos[indice_maior][vet_tam[indice_maior] - 1] = grupos[i][j];
                    centro_grupo(grupos[indice_maior], 1, vet_tam[indice_maior]);
                }
            }
        }
    }

    printf("\n");
    printf("Novos grupos (agrupados)");
    printf("\n");
    for(i = 0; i < 2; i ++){
        if(i == 0)
            printf("Grupo %d (Setosas): ", i+1);
        else
            printf("Grupo %i (Não setosas): ", i + 1);

        for(int j = 0; j < vet_tam[i]; j++)
            printf("%d ", grupos[i][j]);
        printf("\n");
    }

    for(int i = 0; i < vet_tam[indice_maior]; i++){
        if(grupos[indice_maior][i] < 50)
            contador_setosas++;
    }

    printf("\n");
    for (i = 0; i < 2; i++) {
        printf("Centro Grupo[%i]: media sepala length: %lf media sepala width: %lf media petala length: %lf media petala width: %lf\n", 
               i + 1, media[i].MSL, media[i].MSW, media[i].MPL, media[i].MPW);
    }

    if (contador_setosas > 25)
        acurache = acuracia(grupos[indice_maior], grupos[aux], vet_tam[indice_maior], vet_tam[aux]);
    else
        acurache = acuracia(grupos[aux], grupos[indice_maior], vet_tam[aux], vet_tam[indice_maior]);

    printf("acuracia %lf\n", acurache);

    salva_arquivo(matriz_adjacente, &demaior, &demenor, &denmaior, &denmenor);

    for (int i = 0; i <= indice; i++) {
        free(grupos[i]);
    }
    free(grupos);

    return 0;
}

void abre_arquivo() {
    FILE *arquivo;
    arquivo = fopen("IrisDataset.csv", "r");
    char buffer[50];
    if (arquivo) {
        fgets(buffer, sizeof(buffer), arquivo);
        for (int i = 0; i < 150; i++) {
            flor f;
            fscanf(arquivo, "%*[^,],");
            fscanf(arquivo, "%f,%f,%f,%f", &f.slenght, &f.swidth, &f.plenght, &f.pwidth);
            flores[i] = f;
        }
        fclose(arquivo);
    } else {
        printf("Erro ao abrir arquivo!\n");
    }
}


void salva_arquivo(int matriz_adjacente[150][150], float *demaior, float *demenor, float *denmaior, float *denmenor) {
    FILE *arquivo;
    arquivo = fopen("TEG.csv", "w");
    if (arquivo) {
        fprintf(arquivo, "DeMaior: %f (13, 118) | DeMenor: %f (101, 142) | Denmaior: %f (101, 142) | Denmenor: %f (101, 142) \n", (*demaior), (*demenor), (*denmaior), (*denmenor));
        for (int i = 0; i < 150; i++) {
            for (int j = 0; j < 150; j++) {
                fprintf(arquivo, "%d", matriz_adjacente[i][j]);
                if (j < 149) {
                    fprintf(arquivo, ",");
                }
            }
            fprintf(arquivo, "\n");
        }

        fclose(arquivo);
    } else {
        printf("Erro ao abrir arquivo!\n");
    }
}


void distancia_euclidiana() {
    int i = 0, j = 0; 
    for (i = 0; i < 149; i++) {
        for (j = i + 1; j < 150; j++) {
            distancia[i][j] = sqrt(pow((flores[i].slenght - flores[j].slenght), 2) + 
                                   pow((flores[i].swidth - flores[j].swidth), 2) + 
                                   pow((flores[i].plenght - flores[j].plenght), 2) + 
                                   pow((flores[i].pwidth - flores[j].pwidth), 2));
        }
    }
}


void maior_menor(float *maior, float *menor, float matrix[150][150]) {
    int i = 0, j = 0; 

    *maior = matrix[0][1];
    *menor = matrix[0][1];

    for (i = 0; i < 149; i++) {
        for (j = i + 1; j < 150; j++) {
            if (matrix[i][j] > *maior) {
                *maior = matrix[i][j];
            }
            if (matrix[i][j] < *menor) {
                *menor = matrix[i][j];
            }
        }
    }
}


void distancia_normalizada(float *maior, float *menor) {
    int i = 0, j = 0;

    maior_menor(maior, menor, distancia);

    for (i = 0; i < 149; i++) {
        for (j = 0; j < 150; j++) {
            matriz[i][j] = (distancia[i][j] - *menor) / (*maior - *menor);
        }
    }
}


void DFS(int matriz_adjacente[150][150], int u, bool visitado[150], int **grupoX, int *tam) {
    visitado[u] = true;
    *tam += 1;
    *grupoX = realloc(*grupoX, (*tam) * sizeof(int));
    (*grupoX)[(*tam) - 1] = u;

    for (int v = 0; v < 150; v++) {
        if (matriz_adjacente[u][v] == 1 && !visitado[v]) {
            DFS(matriz_adjacente, v, visitado, grupoX, tam);
        }
    }
}


void centro_grupo(int *grupo, int index, int tam) {
    media[index].MSL = 0;
    media[index].MSW = 0;
    media[index].MPL = 0;
    media[index].MPW = 0;

    for (int i = 0; i < tam; i++) {
        int vertice = grupo[i];
        media[index].MSL += flores[vertice].slenght;
        media[index].MSW += flores[vertice].swidth;
        media[index].MPL += flores[vertice].plenght;
        media[index].MPW += flores[vertice].pwidth;
    }

    media[index].MSL /= tam;
    media[index].MSW /= tam;
    media[index].MPL /= tam;
    media[index].MPW /= tam;
}


double acuracia(int *grupo1, int *grupo2, int tam1, int tam2) {
    int tp = 0, tn = 0, fp = 0, fn = 0;

    for (int i = 0; i < tam1; i++) {
    if (grupo1[i] < 50) 
        tp++;            
    else
        fp++; 
    }

    for (int i = 0; i < tam2; i++) {
        if (grupo2[i] >= 50) 
            tn++;           
        else
            fn++;
    }

    double acu = (double)(tp + tn) / (tp + tn + fp + fn) * 100;
    printf("TP: %i | FP: %i\nFN: %i  | TN: %i\n", tp, fp, fn, tn);

    return acu;
}


float distancia_comp(int a, int i, int j) {
    return sqrt(pow((flores[grupos[i][j]].slenght - media[a].MSL), 2) + 
                pow((flores[grupos[i][j]].swidth - media[a].MSW), 2) + 
                pow((flores[grupos[i][j]].plenght - media[a].MPL), 2) + 
                pow((flores[grupos[i][j]].pwidth - media[a].MPW), 2));
}