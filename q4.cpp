#include<bits/stdc++.h>
#include<pthread.h>
#include<sstream>
#include <unistd.h>




#define qt_threads_leitoras 3
#define qt_threads_escritoras 3

auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
std::mt19937 mt_rand(seed);
int writers = 0;
int readers = 0;
int array[100];


typedef struct arguments{
    int id;
    
} args;


pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t writerCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t readerCond = PTHREAD_COND_INITIALIZER;

pthread_t threads_leitoras[qt_threads_leitoras];
pthread_t threads_escritoras[qt_threads_escritoras];



void *leitora(void *ar){

    args *argumentos = (args *) ar;
    while(true){
        pthread_mutex_lock(&mut);
        std::cout<<"leitor "<<argumentos->id<< " começou"<<std::endl;
        

        

        // escolhe uma posição aleatoria do array para ler
        while(!(writers == 0)) pthread_cond_wait(&readerCond, &mut);
        readers ++; 
        pthread_mutex_unlock(&mut);
            
                
        
        int pos = mt_rand()%100;
        int readed_value = array[pos];
        std::cout<<"leitor "<<argumentos->id<< " leu "<<readed_value<<std::endl;
        pthread_mutex_lock(&mut);
        if (--readers == 0) pthread_cond_signal(&writerCond);
        pthread_mutex_unlock(&mut);
    }

}

void *escritora(void *ar){
   
    args *argumentos = (args *) ar;
    while(true){
        pthread_mutex_lock(&mut);
        std::cout<<"escritor "<<argumentos->id<< " começou"<<std::endl;
        pthread_mutex_unlock(&mut);

        while(!(readers ==0) && (writers ==0)) pthread_cond_wait(&writerCond,&mut);
        writers++;
        pthread_mutex_unlock(&mut);
        int pos = mt_rand()%100;
        int value = mt_rand()%277;
        array[pos] = value;
        std::cout<<"escritor "<<argumentos->id<< " escreveu"<<std::endl;

        pthread_mutex_lock(&mut);
        writers--;
        pthread_cond_signal(&writerCond);
        pthread_cond_broadcast(&readerCond);
        pthread_mutex_unlock(&mut);
    }    

}


int main(){
    args *argumentos;
    for(int x = 0; x < 100; x++){
        array[x]=100-x;

    }
    for(int x = 0; x < qt_threads_leitoras; x++){
        argumentos = new args;
        argumentos->id = x;  
        pthread_create(&threads_leitoras[x], NULL, leitora, argumentos);
    }

    for(int x = 0; x < qt_threads_escritoras; x++){
        argumentos = new args;
        argumentos->id = x;                
        pthread_create(&threads_escritoras[x], NULL, escritora, argumentos);
    }


    for(int x = 0; x < qt_threads_leitoras; x++) pthread_join(threads_leitoras[x], NULL);
    for(int x = 0; x < qt_threads_escritoras; x++) pthread_join(threads_escritoras[x], NULL);
    pthread_exit(NULL);
    return 0;

}