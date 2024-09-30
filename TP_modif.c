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
void pedirMovimiento(char cancha[tamY][tamX], jugador team[], int tamaño_equipo, int turno);
void pedirMovimientoPelota(char cancha[tamY][tamX], pelota *p, int turno);
int moverJugador(char cancha[tamY][tamX], jugador *j, int nueva_x, int nueva_y);
int moverPelota(char cancha[tamY][tamX], pelota *p, int nueva_x, int nueva_y, int turno);
void inicializarTablero(char cancha[tamY][tamX], jugador team_rojo[], jugador team_blanco[], int tamaño_equipo, pelota *p);







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
                printf(". ");
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
void pedirMovimiento(char cancha[tamY][tamX], jugador team[], int tamaño_equipo, int turno) {
    int id, nueva_x, nueva_y;

    printf("Turno de las %s. Ingrese el ID del jugador que desea mover: %s\n",
           (turno == 0) ? "Rojas" : "Blancas",
           (turno == 0) ? "R" : "B");

    scanf("%d", &id);

    jugador *jugador_a_mover = NULL;
    for (int i = 0; i < tamaño_equipo; i++) {
        if (team[i].id == id) {
            jugador_a_mover = &team[i];
            break;
        }
    }

    if (jugador_a_mover == NULL || (turno == 0 && jugador_a_mover->id > 5) || (turno == 1 && jugador_a_mover->id <= 5)) {
        printf("Jugador no válido para este turno.\n");
        return;
    }

    printf("Ingrese nuevas coordenadas (fila y columna): ");
    scanf("%d %d", &nueva_x, &nueva_y);

    if (moverJugador(cancha, jugador_a_mover, nueva_x, nueva_y)) {
        printf("Movimiento exitoso.\n");
    } else {
        printf("Movimiento inválido.\n");
    }
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

    if (moverPelota(cancha, p, nueva_x, nueva_y, turno)) {
        printf("Movimiento de la pelota exitoso.\n");
    } else {
        printf("Movimiento de la pelota inválido.\n");
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


    if (cancha[nueva_x][nueva_y] != ' ') {
        return 0;
    }


    cancha[j->pos_x][j->pos_y] = '_';
    cancha[nueva_x][nueva_y] = (j->id <= 5) ? 'R' : 'B';
    j->pos_x = nueva_x;
    j->pos_y = nueva_y;
    return 1;
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
    if (nueva_x < 1 || nueva_x > 13 || nueva_y < 0 || nueva_y >= tamX) {
        return 0; // Movimiento inválido
    }

    int delta_x = abs(nueva_x - p->pos_x);
    int delta_y = abs(nueva_y - p->pos_y);

    if (delta_x > 2 || delta_y > 2 || (delta_x + delta_y) > 2) {
        return 0; // Movimiento inválido
    }

    // Verificar si hay un jugador del equipo en las posiciones adyacentes
    char equipo = (turno == 0) ? 'R' : 'B'; // 'R' para rojas, 'B' para blancas
    int hay_jugador_adjacente = 0;

    // Comprobar las posiciones adyacentes
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue; // Ignorar la posición actual
            int x_adjacente = p->pos_x + dx;
            int y_adjacente = p->pos_y + dy;

            if (x_adjacente >= 0 && x_adjacente < tamY && y_adjacente >= 0 && y_adjacente < tamX) {
                if (cancha[x_adjacente][y_adjacente] == equipo) {
                    hay_jugador_adjacente = 1;
                    break;
                }
            }
        }
        if (hay_jugador_adjacente) break;
    }

    if (!hay_jugador_adjacente) {
        return 0; // No se puede mover la pelota si no hay jugadores adyacentes
    }

    cancha[p->pos_x][p->pos_y] = ' '; // Vaciar la posición original
    cancha[nueva_x][nueva_y] = '0'; // Marcar la nueva posición de la pelota
    p->pos_x = nueva_x; // Actualizar la posición de la pelota
    p->pos_y = nueva_y;
    return 1; // Movimiento exitoso
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


    team_rojo[0].id = 1; team_rojo[0].pos_x = 8; team_rojo[0].pos_y = 8; // R1
    team_rojo[1].id = 2; team_rojo[1].pos_x = 8; team_rojo[1].pos_y = 2; // R2
    team_rojo[2].id = 3; team_rojo[2].pos_x = 12; team_rojo[2].pos_y = 5; // R3
    team_rojo[3].id = 4; team_rojo[3].pos_x = 10; team_rojo[3].pos_y = 3; // R4
    team_rojo[4].id = 5; team_rojo[4].pos_x = 10; team_rojo[4].pos_y = 7; // R5


    team_blanco[0].id = 1; team_blanco[0].pos_x = 3; team_blanco[0].pos_y = 5; // B1
    team_blanco[1].id = 2; team_blanco[1].pos_x = 4; team_blanco[1].pos_y = 3; // B2
    team_blanco[2].id = 3; team_blanco[2].pos_x = 4; team_blanco[2].pos_y = 7; // B3
    team_blanco[3].id = 4; team_blanco[3].pos_x = 6; team_blanco[3].pos_y = 2; // B4
    team_blanco[4].id = 5; team_blanco[4].pos_x = 6; team_blanco[4].pos_y = 8; // B5


    for (int i = 0; i < tamaño_equipo; i++) {
        cancha[team_rojo[i].pos_x][team_rojo[i].pos_y] = 'R'; // 'R'
        cancha[team_blanco[i].pos_x][team_blanco[i].pos_y] = 'B'; // 'B'
    }


    p->pos_x = 7;
    p->pos_y = 5;
}



int main(void) {
    jugador team_rojo[5];
    jugador team_blanco[5];
    char cancha[tamY][tamX];
    pelota p;


    inicializarTablero(cancha, team_rojo, team_blanco, 5, &p);

    int turno = 0; // 0 para rojos, 1 para blancos
    while (1) {
        imprimirCancha(cancha, &p);
        pedirMovimiento(cancha, (turno == 0) ? team_rojo : team_blanco, 5, turno);
        pedirMovimientoPelota(cancha, &p, turno);
        turno = 1 - turno; // Cambiar turno
    }

    return 0;
}
