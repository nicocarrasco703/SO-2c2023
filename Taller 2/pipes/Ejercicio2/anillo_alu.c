#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "constants.h"

int generate_random_number() { return (rand() % 50); }

void sigchld_handler() { read printf }

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

  /* COMPLETAR */
  printf(
      "Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n",
      n, buffer, start);

  /* COMPLETAR */

  pid_t *children = malloc(sizeof(*children) * n);

  // init pipes
  int pipes[n + 1][2];

  for (int i = 0; i < n; i++) {
    pipe(pipes[i]);
  }

  close(pipes[0][0]);
  write(pipes[0][1], &n, sizeof(n));  // escribo el mensaje por el pipe

  // signal();

  // primer hijo
  children[0] = fork();
  if (children[0] < 0)
    exit(EXIT_FAILURE);
  else if (children[0] == 0) {
  }

  // hijos intermedios
  for (int i = 0; i < n; i++) {
    children[i] = fork();
    if (children[i] < 0)
      exit(EXIT_FAILURE);
    else if (children[i] == 0) {
      close(pipes[i][0]);  // cierro los extremos de lectura al sucesor

      for (int k = 0; k < n; k++) {  // cierro los demas pipes
        if (k != i) {
          close(pipes[k][0]);
          close(pipes[k][1]);
        }
      }

      if (i == start) {  // si soy el proceso que inicia
        int numero = 0;
        read(pipes[0][0], &numero,
             sizeof(numero));  // del pipe padre leo el numero
        int num_secreto = generate_random_number();  // genero numero secreto
        write(pipes[i][1], &num_secreto,
              sizeof(int));  // escribo el numero secreto al pipe sucesor
      }

      for (int j = 0; j < count; j++) {
        if
      }
    }
  }
}
