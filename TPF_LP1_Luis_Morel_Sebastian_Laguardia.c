/*
 ============================================================================
 Name        : TPF_1.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define tamX 11
#define tamY 15

/*
 * Estructura para cada jugador con un identificatorio, y su fila y columna
 */
typedef struct {

	int id;
	int pos_x;
	int pos_y;

} jugador;

/*
 * Estructura que representa la pocision de la pelota dentro de la matriz
 */
typedef struct {
    int pos_x;
    int pos_y;
} pelota;


void imprimirCancha(char cancha[tamY][tamX], pelota *p);
int pedirMovimiento(char cancha[tamY][tamX], jugador team[], int tamaño_equipo, int turno);
void pedirMovimientoPelota(char cancha[tamY][tamX], pelota *p, int turno);
int pase (char cancha[tamY][tamX], pelota *p, int nueva_x, int nueva_y, int turno);
int moverJugador(char cancha[tamY][tamX], jugador *j, int nueva_x, int nueva_y);
int moverPelota(char cancha[tamY][tamX], pelota *p, int nueva_x, int nueva_y, int turno);
int verificarPosesion(char cancha[tamY][tamX], pelota *p, int turno);
void inicializarTablero(char cancha[tamY][tamX], jugador team_rojo[], jugador team_blanco[], int tamaño_equipo, pelota *p);
void moverComp(char cancha[tamY][tamX], jugador team[], int tamaño_equipo, pelota *p);
int moverPelotaComp(char cancha[tamY][tamX], pelota *p, int turno);
int verificarGol(pelota *p);
void limpiarBuffer();
int elegirModoDeJuego();
int elegirColor();
int elegirEquipoInicial();



void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int elegirModoDeJuego() {
    int modo;
    printf("Elige el modo de juego:\n1 - Jugador vs Jugador\n2 - Jugador vs Computadora\n");
    while (scanf("%d", &modo) != 1 || (modo < 1 || modo > 2)) {
        limpiarBuffer();
        printf("Entrada no válida. Elige 1 para Jugador vs Jugador o 2 para Jugador vs Computadora: ");
    }
    return modo;
}

int elegirColor() {
    int color;
    printf("Elige tu equipo (1 - Rojos, 2 - Blancos): ");
    while (scanf("%d", &color) != 1 || (color != 1 && color != 2)) {
        limpiarBuffer();
        printf("Entrada no válida. Elige 1 para Rojos o 2 para Blancos: ");
    }
    return color;
}

int elegirEquipoInicial() {
    int opcion;
    printf("Elige el equipo que comenzará (1 - Rojos, 2 - Blancos, 3 - Aleatorio): ");
    while (scanf("%d", &opcion) != 1 || (opcion < 1 || opcion > 3)) {
        limpiarBuffer();
        printf("Entrada no válida. Elige 1 (Rojos), 2 (Blancos) o 3 (Aleatorio): ");
    }
    if (opcion == 3) {
        srand(time(NULL));
        opcion = (rand() % 2) + 1;
        printf("El equipo inicial ha sido elegido aleatoriamente: %s\n", (opcion == 1) ? "Rojos" : "Blancos");
    }
    return opcion - 1;
}

/*
 * Funcion que imprime el tablero (cancha) con sus piezas colocadas
 * Parametros>
 * char que representa la cancha
 * puntero al struct que representa la pelota
 * Retorno>
 * vacio
 */

void imprimirCancha(char cancha[tamY][tamX], pelota *p) {

    printf("   ");
    for (int j = 0; j < tamX; j++) {
        printf("%d ", j);
    }
    printf("\n");

    for (int i = tamY - 1; i >= 0; i--) {

        printf("%2d ", i);
        for (int j = 0; j < tamX; j++) {
            if ((i == 0 || i == tamY - 1) && (j == 0 || j == tamX - 1)) {
                printf("= ");
            } else if (i == p->pos_x && j == p->pos_y) {
                printf("0 ");
            } else {
                printf("%c ", cancha[i][j]);
            }
        }
        printf("\n");
    }
}

/*Función que pide al usuario las coordenadas para mover el jugador
 *
 * Parametros>
 * matriz de char que representa la cancha
 * struct del jugador
 * tamanho del equipo
 * entero que representa el turno
 * retorno>
 * vacio
 */
int pedirMovimiento(char cancha[tamY][tamX], jugador team[], int tamaño_equipo, int turno) {
    int id, nueva_x, nueva_y;

    printf("Turno de las %s %s. Ingrese el ID del jugador que desea mover: %s\n",
           (turno == 0) ? "Rojas" : "Blancas",
           (turno == 0) ? "ID: 1 al 5, de izq a derecha (ID:3 corresponde al arquero)" : "ID: 6 al 10, de izq a derecha (ID:8 corresponde al arquero)",
           (turno == 0) ? "R" : "B");

    if (scanf("%d", &id) != 1) {
        limpiarBuffer();
        printf("Entrada no válida. Intente de nuevo.\n");
        return 0;
    }

    jugador *jugador_a_mover = NULL;
    for (int i = 0; i < tamaño_equipo; i++) {
        if (team[i].id == id) {
            jugador_a_mover = &team[i];
            break;
        }
    }

    if (jugador_a_mover == NULL || (turno == 0 && jugador_a_mover->id > 5) || (turno == 1 && jugador_a_mover->id <= 5)) {
        printf("Jugador no válido para este turno.\n");
        return 0;
    }

    printf("Ingrese nuevas coordenadas (fila y columna): ");
    if (scanf("%d %d", &nueva_x, &nueva_y) != 2) {
        limpiarBuffer();
        printf("Entrada no válida. Intente de nuevo.\n");
        return 0;
    }

    if (moverJugador(cancha, jugador_a_mover, nueva_x, nueva_y)) {
        printf("Movimiento exitoso.\n");
        return 1;
    } else {
        printf("Movimiento inválido.\n");
        return 0;
    }
}
/*Función para mover una ficha
 *
 * Parametros>
 * matriz de char que representa la cancha
 * el jugador a mover
 * la pocision de destino
 * retorno>
 * 0 si no se pudo mover la ficha y 1 si es que si se pudo
 */
int moverJugador(char cancha[tamY][tamX], jugador *j, int nueva_x, int nueva_y) {

    if (nueva_x < 1 || nueva_x > 13 || //
        nueva_y < 0 || nueva_y >= tamX) {
        return 0;
    }


    int delta_x = abs(nueva_x - j->pos_x);
    int delta_y = abs(nueva_y - j->pos_y);


    if (delta_x > 2 || delta_y > 2 || (delta_x + delta_y) > 2) {
        return 0;
    }


    if ((cancha[nueva_x][nueva_y] != '_' ) && (cancha[nueva_x][nueva_y] != '.')) {
        return 0;
    }


    cancha[j->pos_x][j->pos_y] = '_';
    cancha[nueva_x][nueva_y] = (j->id <= 5) ? 'R' : 'B';
    j->pos_x = nueva_x;
    j->pos_y = nueva_y;
    return 1;
}

/*
 * Funcion que elige los movimientos de la computadora
 */
void moverComp(char cancha[tamY][tamX], jugador team[], int tamaño_equipo, pelota *p) {
    int jugador_a_mover_i = rand() % tamaño_equipo;
    jugador *jugador_a_mover = &team[jugador_a_mover_i];

    int nueva_x = jugador_a_mover->pos_x + (rand() % 3 - 1);
    int nueva_y = jugador_a_mover->pos_y + (rand() % 3 - 1);


    if (moverJugador(cancha, jugador_a_mover, nueva_x, nueva_y)) {
        printf("La IA movió al jugador %d a (%d, %d).\n", jugador_a_mover->id, nueva_x, nueva_y);
    } else {
        moverComp(cancha, team, tamaño_equipo, p);
    }
}

/*Función pque verifica si el movimiento de la pelota es a un jugador (pase)
 *
 * Parametros>
 * matriz de char que representa la cancha
 * puntero al struct pelota
 * la pocision de destino
 * retorno>
 * 0 si no es un pase y 1 si es un pase
 */
int pase (char cancha[tamY][tamX], pelota *p, int nueva_x, int nueva_y, int turno){
	char equipo = (turno == 0) ? 'R' : 'B';

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;

            int x_adyacente = nueva_x + dx;
            int y_adyacente = nueva_y + dy;

            if (x_adyacente >= 0 && x_adyacente < tamY && y_adyacente >= 0 && y_adyacente < tamX) {
                if (cancha[x_adyacente][y_adyacente] == equipo) {
                    return 1;
                }

            }
        }
    } return 0;
}

/*Función que pide al usuario las coordenadas para mover la pelota
 *
 * Parametros>
 * matriz de char que representa la cancha
 * puntero al struct que representa la pelota
 * entero que representa el turno actual
 * retorno>
 * vacio
 */
void pedirMovimientoPelota(char cancha[tamY][tamX], pelota *p, int turno) {
    int nueva_x, nueva_y;

    printf("Ingrese nuevas coordenadas para mover la pelota (fila y columna): ");
    scanf("%d %d", &nueva_x, &nueva_y);
    limpiarBuffer();

    if (moverPelota(cancha, p, nueva_x, nueva_y, turno)) {
        printf("Movimiento de la pelota exitoso.\n");
    } else {
        printf("Movimiento de la pelota inválido.\n");
    }
}
/*Función para mover la pelota
 *
 * Parametros>
 * matriz de char que representa la cancha
 * puntero pelota
 * la pocision de destino
 * entero que representa el turno actual
 * retorno>
 * 0 si no se pudo mover la ficha y 1 si es que si se pudo
 */
int moverPelota(char cancha[tamY][tamX], pelota *p, int nueva_x, int nueva_y, int turno) {
    if (nueva_x < 0 || nueva_x > tamY || nueva_y < 0 || nueva_y >= tamX) {
        return 0;
    }

    int delta_x = abs(nueva_x - p->pos_x);
    int delta_y = abs(nueva_y - p->pos_y);



    if (delta_x > 4 || delta_y > 4 || (delta_x + delta_y) > 4) {
        return 0;
    }


    cancha[p->pos_x][p->pos_y] = '_'; // Vaciar la posición original
    cancha[nueva_x][nueva_y] = '0'; // Marcar la nueva posición de la pelota
    p->pos_x = nueva_x; // Actualizar la posición de la pelota
    p->pos_y = nueva_y;
    return 1;
}


/*
 * Funcion que realiza movimientos de la pelota al azar por la computadora
 * parametros>
 * matriz cancha
 * puntero al struct de la pelota
 * entero que indica el turno
 *
 * retorno>
 * 0 si no pudo mover la pelota y 1 si es que si lo pudo hacer
 */
int moverPelotaComp(char cancha[tamY][tamX], pelota *p, int turno) {
    int nueva_x = p->pos_x+ (rand() % 5 - 1);
    int nueva_y = p->pos_y + (rand() % 5 - 1);
    if (nueva_x < 0 || nueva_x > tamY || nueva_y < 0 || nueva_y >= tamX) {
        return 0;
    }

    int delta_x = abs(nueva_x - p->pos_x);
    int delta_y = abs(nueva_y - p->pos_y);

    if (delta_x > 4 || delta_y > 4 || (delta_x + delta_y) > 4) {
        return 0;
    }


    cancha[p->pos_x][p->pos_y] = '_'; 
    cancha[nueva_x][nueva_y] = '0'; 
    p->pos_x = nueva_x; 
    p->pos_y = nueva_y;
    return 1;
}

/*Función que verifica quien tiene la posesion de la pelota con los cuadros adyacentes a la misma
 *
 * Parametros>
 * matriz de char que representa la cancha
 * puntero pelota
 * entero que representa el turno actual
 * retorno>
 * 0 si el equipo en turno no tiene la posesion y 1 si es que si la tiene
 */

int verificarPosesion(char cancha[tamY][tamX], pelota *p, int turno) {
    char equipo = (turno == 0) ? 'R' : 'B';
    char equipoCont = (turno == 0) ? 'B' : 'R';
    int equipoActual = 0, equipoContrario = 0;

    // Comprobar las posiciones adyacentes
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;

            int x_adyacente = p->pos_x + dx;
            int y_adyacente = p->pos_y + dy;

            if (x_adyacente >= 0 && x_adyacente < tamY && y_adyacente >= 0 && y_adyacente < tamX) {
                if (cancha[x_adyacente][y_adyacente] == equipo) {
                    equipoActual++;
                } else if (cancha[x_adyacente][y_adyacente] == equipoCont){
                	equipoContrario++;
                }
            }
        }
    }
    if(equipoActual>equipoContrario){
    	return 1;
    }else
    return 0; // No se encontró un jugador adyacente
}





/*Función para inicializar el tablero
 *
 * Parametros>
 * matriz de char que representa la cancha
 * puntero pelota
 * dos struct que representan a los jugadores de ambos equipos
 * cantidad de jugadores
 * retorno>
 * vacio
 */
void inicializarTablero(char cancha[tamY][tamX], jugador team_rojo[], jugador team_blanco[], int tamaño_equipo, pelota *p) {

    for (int i = 0; i < tamY; i++) {
        for (int j = 0; j < tamX; j++) {
            cancha[i][j] = '_';
        }
    }
    team_rojo[1].id = 1; team_rojo[1].pos_x = 8; team_rojo[1].pos_y = 2; // R4
    team_rojo[3].id = 2; team_rojo[3].pos_x = 10; team_rojo[3].pos_y = 3; // R2
    team_rojo[2].id = 3; team_rojo[2].pos_x = 11; team_rojo[2].pos_y = 5; // R3
    team_rojo[4].id = 4; team_rojo[4].pos_x = 10; team_rojo[4].pos_y = 7; // R1
    team_rojo[0].id = 5; team_rojo[0].pos_x = 8; team_rojo[0].pos_y = 8; // R5

    cancha[14][0] = '=';
    cancha[14][10] = '=';
    cancha[14][1] = '=';
    cancha[14][2] = '=';
    cancha[14][8] = '=';
    cancha[14][9] = '=';
    cancha[0][1] = '=';
    cancha[0][2] = '=';
    cancha[0][8] = '=';
    cancha[0][9] = '=';
    cancha[0][0] = '=';
    cancha[0][10] = '=';

    cancha[13][0] = '.';
    cancha[13][10] = '.';
    for(int aux1=3; aux1<=7; aux1++){
        cancha[13][aux1] = '.';
    }
    cancha[1][0] = '.';
    cancha[1][10] = '.';
    for(int aux2=3; aux2<=7; aux2++){
        cancha[1][aux2] = '.';
    }



    team_blanco[3].id = 6; team_blanco[3].pos_x = 6; team_blanco[3].pos_y = 2; // B1
    team_blanco[1].id = 7; team_blanco[1].pos_x = 4; team_blanco[1].pos_y = 3; // B2
    team_blanco[0].id = 8; team_blanco[0].pos_x = 3; team_blanco[0].pos_y = 5; // B3
    team_blanco[2].id = 9; team_blanco[2].pos_x = 4; team_blanco[2].pos_y = 7; // B4
    team_blanco[4].id = 10; team_blanco[4].pos_x = 6; team_blanco[4].pos_y = 8; // B5


    for (int i = 0; i < tamaño_equipo; i++) {
        cancha[team_rojo[i].pos_x][team_rojo[i].pos_y] = 'R'; // 'R'
        cancha[team_blanco[i].pos_x][team_blanco[i].pos_y] = 'B'; // 'B'
    }


    p->pos_x = 7;
    p->pos_y = 5;
}


/*Función que verifica si la pelota entro en zona de gol
 *
 * Parametros>
 * puntero pelota
 * retorno>
 * 0 si no es gol y 1 si es gol
 */
int verificarGol(pelota *p) {

    if ( (p->pos_x == 14 || p->pos_x == 0) && (p->pos_y== 3 || p->pos_y== 4 || p->pos_y== 5 || p->pos_y== 6 || p->pos_y== 7) ) {
        return 1; // Gol
    }
    return 0; // No gol
}



int main(void) {
    jugador team_rojo[5];
    jugador team_blanco[5];
    char cancha[tamY][tamX];
    pelota p;
    int golesR = 0, golesB = 0;
    srand(time(NULL));


    int modo = elegirModoDeJuego();


    int color_equipo = elegirColor();


    int turno = elegirEquipoInicial();

    while ((golesR < 2 && golesB < 2)){
        inicializarTablero(cancha, team_rojo, team_blanco, 5, &p);
        printf("\nMarcador: \nRojos: %d \nBlancos: %d \n", golesR, golesB);


        while (1) {
            imprimirCancha(cancha, &p);
            printf("\nMarcador: \nRojos: %d \nBlancos: %d \n", golesR, golesB);

            if (modo == 1) {
                if (turno == 0) {

                    int movimiento_valido = pedirMovimiento(cancha, team_rojo, 5, turno);

                    if (movimiento_valido) {
                        imprimirCancha(cancha, &p);
                        printf("\nMarcador: \nRojos: %d \nBlancos: %d \n", golesR, golesB);
                        if (verificarPosesion(cancha, &p, turno)) {
                            pedirMovimientoPelota(cancha, &p, turno);
                        } else {
                            printf("No hay jugadores adyacentes para mover la pelota. El turno sigue.\n");
                        }


                        if (verificarGol(&p)) {
                            printf("¡Gol! Reiniciando el tablero...\n");
                            inicializarTablero(cancha, team_rojo, team_blanco, 5, &p);
                            golesR++;
                        }

                        turno = 1 - turno;
                    } else {
                        printf("Movimiento inválido. El turno sigue.\n");
                    }
                } else {

                    int movimiento_valido = pedirMovimiento(cancha, team_blanco, 5, turno);

                    if (movimiento_valido) {
                        imprimirCancha(cancha, &p);
                        printf("\nMarcador: \nRojos: %d \nBlancos: %d \n", golesR, golesB);
                        if (verificarPosesion(cancha, &p, turno)) {
                            pedirMovimientoPelota(cancha, &p, turno);
                        } else {
                            printf("No hay jugadores adyacentes para mover la pelota. El turno sigue.\n");
                        }


                        if (verificarGol(&p)) {
                            printf("¡Gol! Reiniciando el tablero...\n");
                            inicializarTablero(cancha, team_rojo, team_blanco, 5, &p);
                            golesB++;
                        }

                        turno = 1 - turno;
                    } else {
                        printf("Movimiento inválido. El turno sigue.\n");
                    }
                }
            } else {
                if (turno == 0) {

                    int movimiento_valido = pedirMovimiento(cancha, team_rojo, 5, turno);

                    if (movimiento_valido) {
                        imprimirCancha(cancha, &p);
                        printf("\nMarcador: \nRojos: %d \nBlancos: %d \n", golesR, golesB);
                        if (verificarPosesion(cancha, &p, turno)) {
                            pedirMovimientoPelota(cancha, &p, turno);
                        } else {
                            printf("No hay jugadores adyacentes para mover la pelota. El turno sigue.\n");
                        }


                        if (verificarGol(&p)) {
                            printf("¡Gol! Reiniciando el tablero...\n");
                            inicializarTablero(cancha, team_rojo, team_blanco, 5, &p);
                            golesR++;
                        }

                        turno = 1 - turno;
                    } else {
                        printf("Movimiento inválido. El turno sigue.\n");
                    }
                } else {

                    moverComp(cancha, team_blanco, 5, &p);
                    imprimirCancha(cancha, &p);
                    printf("\nMarcador: \nRojos: %d \nBlancos: %d \n", golesR, golesB);
                    if (verificarPosesion(cancha, &p, turno)) {
                    	moverPelotaComp(cancha, &p, turno);
                        //pedirMovimientoPelota(cancha, &p, turno);
                    } else {
                        printf("No hay jugadores adyacentes para mover la pelota. El turno sigue.\n");
                    }


                    if (verificarGol(&p)) {
                        printf("¡Gol! Reiniciando el tablero...\n");
                        inicializarTablero(cancha, team_rojo, team_blanco, 5, &p);
                        golesB++;
                    }


                    turno = 1 - turno;
                }
            }
        }
    }
    printf("Juego finalizado!!!.\n");
    if (golesR==2){
    	printf("Ganador: Rojos.\n");
    }else if (golesB==2){
    	printf("Ganador: Blancos.\n");
    }

    return 0;
}

