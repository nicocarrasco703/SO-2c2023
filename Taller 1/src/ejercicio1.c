#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int n;
int numero_maldito;
int generate_random_number(){
    srand(time(NULL));
	return (rand() % n);
}

void handler(int sig){
    int num = generate_random_number();
    if (num == numero_maldito){
        printf("AGUANTE RIVEEEER, PLAAM!");
        exit(EXIT_SUCCESS); // devuelve pid
    }
}

void halt2(){
    while(1){
        sleep(1);
    } 
}

int main(int argc, char const *argv[]){
    //Funcion para cargar nueva semilla para el numero aleatorio
    srand(time(NULL));
    
    n = atoi(argv[1]);
	int rondas = atoi(argv[2]);
	numero_maldito = atoi(argv[3]);

    pid_t procIDs[n];
    signal(SIGTERM, handler);

    for (int i = 0; i < n; i++){
        procIDs[i] = fork();
        if (procIDs[i] == -1) exit(EXIT_FAILURE);
        else if (procIDs[i] == 0) {
            halt2();
        }
    }
    for (int i = 0; i < rondas; i++)
    {
        int status;
        for (int j = 0; j < n; j++)
        {
            if (procIDs[j] != -1){
                kill(procIDs[j], 15);
                sleep(1);
                if (waitpid(procIDs[j], &status, WNOHANG) == procIDs[j]){
                    procIDs[j] = -1;
                }
            }
        }
        
    }
    
    for (int i = 0; i < n; i++)
    {
        int id = procIDs[i];
        if (procIDs[i] != -1){
            printf("Proceso sobrevivió ID %d", id);
        }
    }
    

    exit(0);
}