#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "constants.h"

int generate_random_number() { return (rand() % 50); }

int main(int argc, char **argv) {
  // Funcion para cargar nueva semilla para el numero aleatorio
  srand(time(NULL));

  int status, pid, n, start, buffer;
  n = atoi(argv[1]);
  buffer = atoi(argv[2]);
  start = atoi(argv[3]);

  if (argc != 4) {
    printf("Uso: anillo <n> <c> <s> \n");
    exit(0);
  }

  printf(
      "Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n",
      n, buffer, start);

  pid_t *children = malloc(sizeof(*children) * n);

  // init pipes
  int pipes[n + 1][2];

  for (int i = 0; i < n; i++) {
    pipe(pipes[i]);
  }

  write(pipes[n][PIPE_WRITE], &n, sizeof(int));  // escribo el mensaje por el pipe

  for (int i = 0; i < n; i++) {
    children[i] = fork();
    if (children[i] < 0) exit(EXIT_FAILURE);
    else if (children[i] == 0) {
      int pre = i % n;
      int suc = (i+1) % n;

      close(pipes[suc][PIPE_READ]);  // cierro el extremo de lectura al sucesor
      close(pipes[pre][PIPE_WRITE]); // cierro el extremo de escritura del predecesor
      for (int k = 0; k < n; k++) {  // cierro los demas pipes
        if (k != pre && k != suc) {
          close(pipes[k][PIPE_READ]);
          close(pipes[k][PIPE_WRITE]);
        }
      }
      if(i != start){ // si no soy el proceso inicial cierro los pipes padre
        close(pipes[n][PIPE_READ]);
        close(pipes[n][PIPE_WRITE]);
      }
      int numero;
      int it = 1;
      
      while(1){
        if (i == start){ // si soy el proceso que inicia, leo el pipe n (del padre) y arranco
          int num_secreto = generate_random_number();  // genero numero secreto
          write(pipes[n][PIPE_WRITE], &num_secreto, sizeof(int)); // escribo el pipe del padre con el numero secreto
          if (it == 1){ // si es la primera iteracion
            int num_inicial;
            it++;
            read(pipes[n][PIPE_READ], &num_inicial, sizeof(int)); // leo el pipe del padre para obtener el numero inicial
            write(pipes[suc][PIPE_WRITE], &num_inicial, sizeof(int)); // escribo el pipe del sucesor con el nuevo valor
          } else {
            read(pipes[pre][PIPE_READ], &numero, sizeof(int)); // leo el pipe del predecesor
            if (numero >= num_secreto){ // si el numero es mayor o igual al secreto
              printf("Proceso %d: Numero secreto alcanzado (%d >= %d)\n", i, numero, num_secreto);
              break;
            } else {
              numero++;
              write(pipes[suc][PIPE_WRITE], &numero, sizeof(int)); // escribo el pipe del sucesor con el nuevo valor
            }
          }
        } else { // si no soy el proceso inicial
          read(pipes[pre][PIPE_READ], &numero, sizeof(int)); // leo el pipe del predecesor
          numero++;
          write(pipes[suc][PIPE_WRITE], &numero, sizeof(int)); // escribo el pipe del sucesor con el nuevo valor
        }
      }
      exit(EXIT_SUCCESS);
    }
  }

  for (int i = 0; i < n; i++){ // cierro los pipes del padre
    close(pipes[i][PIPE_READ]);
    close(pipes[i][PIPE_WRITE]);  
  }
  // mato a todos los hijos una vez que terminen
  for (int i = 0; i < n; i++) {
    waitpid(children[i], &status, 0);
  }

  int num_secreto;
  read(pipes[n][PIPE_READ], &num_secreto, sizeof(int)); // leo el pipe del padre para obtener el numero secreto del proceso inicial
  printf("Numero secreto: %d\n", num_secreto);
  
  free(children);

  exit(EXIT_SUCCESS);
}
