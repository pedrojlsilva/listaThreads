
#include<bits/stdc++.h>
#include<pthread.h>
#include<sstream>

#define qt_threads 3

using namespace std;


string senha = "0000000769";
string resposta = "";
int threadID = -1;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

bool achou = false;

typedef struct arguments{
    int id;
    long int iterations;
    string initialString;
} args;


pthread_t threads_testadoras[qt_threads];

string generate_new_senha(string password_received){

    string generated_password;
    stringstream ss;
    long i = atol(password_received.c_str());
    i++;
    
    ss << std::setw(10) << std::setfill('0') << i;
    
    generated_password=ss.str();

    return generated_password;
}

void *checker(void *ar){
    args *argumentos = (args *) ar;
    std::cout<<"thread "<<argumentos->id<< " alocada"<<std::endl;
    
    string initial_string = argumentos->initialString;
    int iterations = argumentos->iterations;

    string nova_senha="";
    while(!achou && argumentos->iterations >= 0){
        nova_senha = generate_new_senha(initial_string);
        
        if(nova_senha == senha){
            
            achou = true;
            resposta = nova_senha;
            threadID = argumentos->id;
            
            break;
        }
        iterations--;
        initial_string = nova_senha;
    }
}



int main(){
    long int initialSenha = 0;
    stringstream ss;

    long int resto = 10000000000%qt_threads;
    long int quociente = 10000000000/qt_threads;

    ss << std::setw(10) <<std::setfill('0');
    for(int x = 0; x < qt_threads; x++){
        args *argumentos = new args;
        ss.str(std::string());
         if(x==qt_threads-1) {
             
             ss  << (initialSenha + resto-1);
             argumentos->iterations = quociente + resto;
        }else{
            
            ss  << initialSenha;
            argumentos->iterations = quociente;
            initialSenha = initialSenha + quociente;
        }

        argumentos->initialString = ss.str();
        argumentos->id = x;

        
        pthread_create(&threads_testadoras[x], NULL, checker, argumentos);
    }
    for(int x = 0; x < qt_threads; x++) pthread_join(threads_testadoras[x], NULL);
    std::cout<< "senha eh: "<<resposta<< " thread que encontrou foi a "<<threadID<<std::endl;
    return 0;

}
