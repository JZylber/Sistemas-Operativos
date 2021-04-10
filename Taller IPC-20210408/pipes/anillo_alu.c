#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int anterior(int a,int n){
    if(a == 0){
        return n - 1;
    }
    return a - 1;
}


int main(int argc, char **argv)
{
    int status, pid, n;
    int buffer;

    if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}

    n = atoi(argv[1]);
    buffer = atoi(argv[2]);
    int start = atoi(argv[3]);

    pid_t pids[n];
    int pipes[n][2];

    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer, start);

    int parent_child_pipe[2];
    int pipe_creado = pipe(parent_child_pipe);
    if(pipe_creado == -1){
        perror("Posta no pudo crear el pipe?");
        exit(1);
    }

    for(int i= 0;i < n;i++) {
        pipe_creado = pipe(pipes[i]);
        if(pipe_creado == -1){
            perror("Posta no pudo crear el pipe?");
            exit(1);
        }
    }

    for(int i = 0;i < n;i++){
        pids[i] = fork();
        if (pids[i] == 0)
        {
            int previous_child = anterior(i,n);
            //Código Hijo
            //Cerrar pipes inútiles
            for(int j = 0;j<n;j++){
                if (j != previous_child && j != i)
                {
                    //printf("Soy %i y cierro a %i\n",i,j);
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

            }
            //Cerrar cosas que no uso
            close(pipes[previous_child][1]);
            //printf("Soy %i y cierro a %i de escritura\n",i,previous_child);
            close(pipes[i][0]);
            //printf("Soy %i y cierro a %i de lectura\n",i,i);

            //Cerrar pipe especial
            if(i != start){
                close(parent_child_pipe[1]);

            }
            close(parent_child_pipe[0]);

            //Escribir en el siguiente
            //Cadena de cosas
            //Te devuelvo padre
            //Manda mensaje distinguido?
            //Cuando recibo señal, cierro pipes y exiteo
            /*if(i == start){
                write(pipes[start][1],&buffer,1);
            }*/
            int bytes_read = read(pipes[previous_child][0],&buffer,1);
            //printf("Soy %i y mi read inicial es %i\n",i,bytes_read);
            while(bytes_read > 0){
                if(i == start){
                    //Cosas que hace el inicio
                    int secret_number = 6;
                    if (buffer > secret_number){
                        write(parent_child_pipe[1],&buffer,1);
                        close(parent_child_pipe[1]);
                        break;
                    }
                }
                buffer++;
                //printf("Soy %i y mi buffer es %i\n",i,buffer);
                int written = write(pipes[i][1],&buffer,1);
                //printf("Soy %i y escribí %i bytes\n",i,written);
                bytes_read = read(pipes[previous_child][0],&buffer,1);
            }
            //Cierro mis pipes
            //printf("Soy %i y cierro mis pipes con buffer %i\n",i,buffer);
            close(pipes[previous_child][0]);
            close(pipes[i][1]);
            exit(0);
            //Escribir en el siguiente
            //Esperar a un cierre -> Cerrar mis pipes
        }
    }
    close(parent_child_pipe[1]);
    write(pipes[anterior(start,n)][1],&buffer,1);
    for (int i = 0; i < n; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    read(parent_child_pipe[0],&buffer,1);
    /* COMPLETAR */
    printf("Resultado final: %i\n",buffer);
    close(parent_child_pipe[0]);
    for (int i = 0; i < n; i++)
    {
        wait(NULL);
    }
    exit(0);
}



