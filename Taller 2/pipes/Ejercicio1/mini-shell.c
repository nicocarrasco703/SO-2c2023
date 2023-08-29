#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "constants.h"
#include "mini-shell-parser.c"

static int run(char ***progs, size_t count) {
  int r, status;

  // Reservo memoria para el arreglo de pids
  pid_t *children = malloc(sizeof(*children) * count);

  int pipes[count - 1][2];
  for (int i = 0; i < count - 1; i++) {
    pipe(pipes[i]);
  }

  // primer proceso
  children[0] = fork();
  if (children[0] < 0)
    exit(EXIT_FAILURE);
  else if (children[0] == 0) {
    close(pipes[0][0]);                    // cierro la entrada
    dup2(pipes[0][1], STDOUT_FILENO);      // conecto a escritura el stoud
    for (int i = 1; i < count - 1; i++) {  // cierro todos los demas pipes
      close(pipes[i][0]);
      close(pipes[i][1]);
    }
    execvp(progs[0][0], progs[0]);
  }

  // procesos intermedios
  for (int i = 1; i < count - 1; i++) {
    children[i] = fork();
    if (children[i] < 0)
      exit(EXIT_FAILURE);
    else if (children[i] == 0) {
      for (int j = 0; j < count - 1; j++) {
        if (j != i - 1 && j != i) {
          close(pipes[j][0]);
          close(pipes[j][1]);
        }
      }
      close(pipes[i - 1][1]);
      close(pipes[i][0]);
      dup2(pipes[i - 1][0], STDIN_FILENO);
      dup2(pipes[i][1], STDOUT_FILENO);
      execvp(progs[i][0], progs[i]);
    }
  }

  // ultimo proceso
  children[count - 1] = fork();
  if (children[count - 1] < 0)
    exit(EXIT_FAILURE);
  else if (children[count - 1] == 0) {
    close(pipes[count - 2][1]);               // cierro la salida
    dup2(pipes[count - 2][0], STDIN_FILENO);  // conecto a escritura el stoud

    for (int i = count - 3; i >= 0; i--) {  // cierro todos los demas pipes
      close(pipes[i][0]);
      close(pipes[i][1]);
    }

    execvp(progs[count - 1][0], progs[count - 1]);
  }

  // cierro los pipes del padre
  for (int i = 0; i < count; i++) {
    close(pipes[i][0]);
    close(pipes[i][1]);
  }

  // Espero a los hijos y verifico el estado que terminaron
  for (int i = 0; i < count; i++) {
    waitpid(children[i], &status, 0);

    if (!WIFEXITED(status)) {
      fprintf(stderr,
              "proceso %d no terminó correctamente [%d]: ", (int)children[i],
              WIFSIGNALED(status));
      perror("");
      return -1;
    }
  }
  r = 0;
  free(children);

  return r;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf(
        "El programa recibe como parametro de entrada un string con la linea "
        "de comandos a ejecutar. \n");
    printf("Por ejemplo ./mini-shell 'ls -a | grep anillo'\n");
    return 0;
  }
  int programs_count;
  char ***programs_with_parameters = parse_input(argv, &programs_count);

  printf("status: %d\n", run(programs_with_parameters, programs_count));

  fflush(stdout);
  fflush(stderr);

  return 0;
}
