#include<bits/stdc++.h>
#include <pthread.h>

std::vector<int> vetor;

typedef struct arguments{
    int low;
    int high;
}args;


void merge(int l, int m, int r){
    int i, j, k;
    int leftSize = m - l + 1;
    int rightSize = r - m;
    
    std::vector<int> vecLeft;
    std::vector<int> vecRight;

    //copia dos arrays
    for(int i = 0; i < leftSize; i++){
        vecLeft.push_back(vetor[i + l]);
    }
    for(int i = 0; i < rightSize; i++){
        vecRight.push_back(vetor[m + i + 1]);
    }


    i = 0, j = 0, k = l;
    //processo de ordenação
    while(i < leftSize && j < rightSize){
        if(vecLeft[i] <= vecRight[j]){
            vetor[k] = vecLeft[i];
            i++;
        }else{
            vetor[k] = vecRight[j];
            j++;
        }
        k++;
    }
    while(i < leftSize){
        vetor[k] = vecLeft[i];
        i++;
        k++;
    }
    while(j < rightSize){
        vetor[k] = vecRight[j];
        j++;
        k++;
    }
}


void *mergeSortT(void *ar){
    args *argumentos = (args *) ar;

    pthread_t direito;
    pthread_t esquerdo;

    args argDireito;
    args argEsquerdo;
    int low = argumentos->low;
    int mid = argumentos->low + ((argumentos->high-argumentos->low)/2);
    int high = argumentos->high;  
    argEsquerdo.low=0;
    argEsquerdo.high = mid;
    argDireito.low = mid + 1;
    argDireito.high = argumentos->high;

    if (low < high){

        //criação das threads que farão a nova partição do array para criar o lado esquerdo e direito que devem ser ordenados
        pthread_create(&direito, NULL, mergeSortT, &argDireito);
        pthread_create(&esquerdo, NULL, mergeSortT, &argEsquerdo);
        pthread_join(direito, NULL);
        pthread_join(esquerdo, NULL);
        merge(low,mid, high);

    }

}

int main(){

    
    pthread_t thread;
    arguments arg;

    // fazendoa interface com o usuario
    int tam = 0;
    std::cout<<"digite o tamanho do array desejado:"<<std::endl;

    std::cin>>tam;
    std::cout<<"digite os valores que você deseja colocar no array: "<<std::endl;
    for (int i=0; i<tam;i++){
        int value;
        std::cin>>value;
        vetor.push_back(value);
    }
    // preenchendo a struct de argumentos para a criação da primeira thread. as demais serão criadas a medida que foram necessarias 
    arg.low = 0;
    arg.high = tam - 1;

    // criação da thread
    pthread_create(&thread, NULL, mergeSortT, &arg);
    pthread_join(thread, NULL);

    // print do array ordenado
    printf("Array ordenado:\n");
    for(int i = 0; i < tam; i++){
        std::cout << vetor[i] << " ";
    
    }
    std::cout << std::endl;
    pthread_exit(NULL);
    return 0;


}