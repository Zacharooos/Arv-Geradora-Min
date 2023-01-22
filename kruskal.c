#include <stdlib.h>
#include <stdio.h>

//==============================================//
// Structs                                      //
//==============================================//

// Struct que guarda a aresta, contém origem, destino e o peso
typedef struct aresta{
    int origem;
    int destino;
    int peso;
}aresta;

// Struct que define os conjuntos
typedef struct conjunto{
    int pai;
    int rank;
}conjunto;

// Struct que guarda as informações do grafo e um ponteiro para um grupo de arestas.
typedef struct grafo{
    int numVertices;
    int numArestas;
    struct aresta *arestas;

}grafo;

//==============================================//
// QuickSort                                    //
//==============================================//

// Função de swap (troca as posições)
void swap(aresta *v, int i1, int i2){
    aresta aux = v[i1];
    v[i1] = v[i2];
    v[i2] = aux;
}

// Função Partição
int particao(aresta *v, int iEsq, int iDir){ //Função Auxiliar partição, responsável
    int pivot = v[(iEsq+iDir)/2].peso; // Salva o valor do pivot, para fins comparativos.
    
    while (iEsq <= iDir){ // Laço que deslocará o marcador da esquerda para direita até achar um número >= ao pivot.
        while(v[iEsq].peso < pivot){
            iEsq++;
        }
        
        while(v[iDir].peso > pivot){ // Laço que deslocará o marcador da direita para esquerda até achar um número <= ao pivot.
            iDir--;
        }
        
        if (iEsq < iDir){ // Checa se o indíce da esquerda é menor que o da direita, se sim, realiza a troca.
            swap(v, iEsq, iDir);
            iEsq++;
            iDir--;
        } else {
            break; // Caso não seja, significa que todas as trocas pertinentes foram realizadas.
        }
    }
    return iDir; // Retorna o novo pivot.
}

// Função principal
void quicksort(aresta *v, int inicio, int fim){ //Função principal
    if (inicio < fim){
        int pivot = particao(v, inicio, fim); //Realiza uma sequencia de trocas e retorna e vetor para continuar a divisão.
        quicksort(v, inicio, pivot); // Realiza o mesmo processo até a primeira metade.
        quicksort(v, pivot + 1, fim); // Realiza o mesmo processo a partir da segunda metade.
        //Continua recursivamente até encontrar subvetores de tamanho 1.
    }
}

//==============================================//
// Funções de conjuntos disjuntos               //
//==============================================//

// Função união, recebe todos os conjuntos e liga o conjunto 1 ao 2 ou 2 ao 1, dependendo do tamanho.
void uniaoDisjuntos(conjunto *conjuntos, int raiz1, int raiz2){
    // Se o rank do conjunto 1 for menor que o do 2, o pai do conjunto 1 passa a ser o conjunto 2.
    if (conjuntos[raiz1].rank < conjuntos[raiz2].rank){
        conjuntos[raiz1].pai = raiz2;
    
    // Caso contrário
    } else if (conjuntos[raiz1].rank > conjuntos[raiz2].rank){
        conjuntos[raiz2].pai = raiz1;
    
    // Caso em que a ambos tem o mesmo rank, então adoramos a 1 como principal, ligando o 2 a ele e subimos o seu rank.
    } else {
        conjuntos[raiz2].pai = raiz1;
        conjuntos[raiz1].rank++;
    }

}


// Função que busca nos conjuntos o node específicado
int buscaDisjuntos(conjunto *conjuntos, int node){
    // Caso o pai do nó atual não seja o próprio nó, define o pai atual como o pai de seu pai.
    if(conjuntos[node].pai != node){
        conjuntos[node].pai = buscaDisjuntos(conjuntos, conjuntos[node].pai);
    }

    return conjuntos[node].pai;
}

//==============================================//
// Funções de grafos                            //
//==============================================//

// Função que guarda o grafo
grafo *guardarGrafo(int numVertices, int numArestas){
    grafo *grf = (grafo*) malloc (sizeof(grafo));

    grf->numVertices = numVertices;
    grf->numArestas = numArestas;

    grf->arestas = (aresta*) malloc (numArestas * sizeof(aresta));
    return grf;
}

// Função que computa as arestas
void computarArestas(grafo *grf){
	int vertice1 , vertice2, peso;

	for (int i = 0; i < grf->numArestas; ++i){
		scanf("%d%d%d", &vertice1, &vertice2, &peso);

        grf->arestas[i].origem = vertice1;
        grf->arestas[i].destino = vertice2;
        grf->arestas[i].peso = peso;
	}
	return;
}

void mostrarArestas(int numArestas, aresta *arestas){
	for (int i = 0; i < numArestas; ++i){
		printf("Aresta [%d] -> [%d] | peso = %d\n", arestas[i].origem, arestas[i].destino, arestas[i].peso); 
	}
	puts("=-=-= Fim das arestas =-=-=");
}

//==============================================//
// Kruskal                                      //
//==============================================//

int kruskal(aresta *arestas, int numArestas, int numNodes){

    // Estrutura que irá guardar os conjuntos
	conjunto *conjuntos = (conjunto*) malloc (numNodes * sizeof(conjunto));

    for(int i = 0; i < numNodes; ++i){
        conjuntos[i].pai = i;
        conjuntos[i].rank = 0;
    }

    // Definir variáveis que sejam
	int agm = 0, diffSet[2] = {-1, -1};
	int v1, v2;

	for (int i = 0; i < numArestas; ++i){
		v1 = arestas[i].origem;
		v2 = arestas[i].destino;

        diffSet[0] = buscaDisjuntos(conjuntos, v1);
        diffSet[1] = buscaDisjuntos(conjuntos, v2);

        //printf("diffset1 = %d, diffset2 = %d\n", diffSet[0], diffSet[1]);
        if (diffSet[0] != diffSet[1]){
            //printf("diffset!, peso = %d\n", arestas[i].peso);
            uniaoDisjuntos(conjuntos, diffSet[0], diffSet[1]);
            agm += arestas[i].peso;
        }
	}
    free(conjuntos);
    return agm;
}

/*
int main(){

	int numVertices, numArestas, agm;

	scanf("%d%d", &numVertices, &numArestas);
	
	do {
        grafo *grf = guardarGrafo(numVertices, numArestas);
		computarArestas(grf);
		quicksort(grf->arestas, 0, numArestas-1);
        //mostrarArestas(numArestas, grf->arestas);
		agm = kruskal(grf->arestas, numArestas, numVertices);
		printf("%d\n", agm);

        free(grf->arestas);
        free(grf);

		scanf("%d%d", &numVertices, &numArestas);
	} while((numVertices != 0) && (numArestas != 0));
	return 0;
}
*/
