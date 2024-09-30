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

//Estructura para cada jugador con un identificatorio, y su fila y columna
typedef struct {

	int id;
	int pos_x;
	int pos_y;

} jugador;
typedef struct {
    int pos_x;
    int pos_y;
} pelota;


/*Modificamos desde aca
 *
 */

void imprimirCancha(char cancha[tamY][tamX], pelota *p) {
    // Imprimir los índices de las columnas
    printf("   ");
    for (int j = 0; j < tamX; j++) {
        printf("%d ", j);
    }
    printf("\n");

    for (int i = tamY - 1; i >= 0; i--) {
        // Imprimir el índice de la fila
        printf("%2d ", i);
        for (int j = 0; j < tamX; j++) {
            if ((i == 0 || i == tamY - 1) && (j == 0 || j == tamX - 1)) {
                printf(". "); // Esquinas
            } else if (i == p->pos_x && j == p->pos_y) {
                printf("0 "); // Pelota
            } else {
                printf("%c ", cancha[i][j]);
            }
        }
        printf("\n");
    }
}
void pedirMovimiento(char cancha[tamY][tamX], jugador team[], int tamaño_equipo) {
    int id, nueva_x, nueva_y;

    printf("Ingrese el ID del jugador que desea mover: ");
    scanf("%d", &id);

    // Buscar el jugador por ID
    jugador *jugador_a_mover = NULL;
    for (int i = 0; i < tamaño_equipo; i++) {
        if (team[i].id == id) {
            jugador_a_mover = &team[i];
            break;
        }
    }

    if (jugador_a_mover == NULL) {
        printf("Jugador no encontrado.\n");
        return;
    }

    printf("Ingrese nuevas coordenadas (fila y columna): ");
    scanf("%d %d", &nueva_x, &nueva_y);

    // Intentar mover al jugador
    if (moverJugador(cancha, jugador_a_mover, nueva_x, nueva_y)) {
        printf("Movimiento exitoso.\n");
    } else {
        printf("Movimiento inválido.\n");
    }
}

void pedirMovimientoPelota(char cancha[tamY][tamX], pelota *p) {
    int nueva_x, nueva_y;

    printf("Ingrese nuevas coordenadas para mover la pelota (fila y columna): ");
    scanf("%d %d", &nueva_x, &nueva_y);

    // Intentar mover la pelota
    if (moverPelota(cancha, p, nueva_x, nueva_y)) {
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
    // Verificar si la nueva posición está dentro de los límites
    if (nueva_x < 1 || nueva_x > 13 || // Restringir filas 0 y 14
        nueva_y < 0 || nueva_y >= tamX) {
        return 0; // Movimiento inválido
    }

    // Calcular la diferencia en filas y columnas
    int delta_x = abs(nueva_x - j->pos_x);
    int delta_y = abs(nueva_y - j->pos_y);

    // Verificar que el movimiento no exceda dos lugares en cualquier dirección
    if (delta_x > 2 || delta_y > 2 || (delta_x + delta_y) > 2) {
        return 0; // Movimiento inválido
    }

    // Verificar si la nueva posición está vacía
    if (cancha[nueva_x][nueva_y] != ' ') {
        return 0; // Movimiento inválido
    }

    // Actualizar la cancha
    cancha[j->pos_x][j->pos_y] = '_'; // Vaciar la posición original
    cancha[nueva_x][nueva_y] = (j->id <= 5) ? 'R' : 'B'; // Marcar la nueva posición del jugador
    j->pos_x = nueva_x; // Actualizar la posición del jugador
    j->pos_y = nueva_y;
    return 1; // Movimiento exitoso
}

int moverPelota(char cancha[tamY][tamX], pelota *p, int nueva_x, int nueva_y) {
    // Verificar si la nueva posición está dentro de los límites
    if (nueva_x < 1 || nueva_x > 13 || // Restringir filas 0 y 14
        nueva_y < 0 || nueva_y >= tamX) {
        return 0; // Movimiento inválido
    }

    // Calcular la diferencia en filas y columnas
    int delta_x = abs(nueva_x - p->pos_x);
    int delta_y = abs(nueva_y - p->pos_y);

    // Verificar que el movimiento no exceda dos lugares en cualquier dirección
    if (delta_x > 2 || delta_y > 2 || (delta_x + delta_y) > 2) {
        return 0; // Movimiento inválido
    }

    // Verificar si la nueva posición está vacía
    if (cancha[nueva_x][nueva_y] != ' ') {
        return 0; // Movimiento inválido
    }

    // Actualizar la cancha
    cancha[p->pos_x][p->pos_y] = '_'; // Vaciar la posición original
    cancha[nueva_x][nueva_y] = '0'; // Marcar la nueva posición de la pelota
    p->pos_x = nueva_x; // Actualizar la posición de la pelota
    p->pos_y = nueva_y;
    return 1; // Movimiento exitoso
}
void inicializarTablero(char cancha[tamY][tamX], jugador team_rojo[], jugador team_blanco[], int tamaño_equipo, pelota *p) {
    // Inicializar la cancha con espacios
    for (int i = 0; i < tamY; i++) {
        for (int j = 0; j < tamX; j++) {
            cancha[i][j] = '_';
        }
    }

    // Inicializar jugadores del equipo rojo
    team_rojo[0].id = 1; team_rojo[0].pos_x = 8; team_rojo[0].pos_y = 8; // R1
    team_rojo[1].id = 2; team_rojo[1].pos_x = 8; team_rojo[1].pos_y = 2; // R2
    team_rojo[2].id = 3; team_rojo[2].pos_x = 12; team_rojo[2].pos_y = 5; // R3
    team_rojo[3].id = 4; team_rojo[3].pos_x = 10; team_rojo[3].pos_y = 3; // R4
    team_rojo[4].id = 5; team_rojo[4].pos_x = 10; team_rojo[4].pos_y = 7; // R5

    // Inicializar jugadores del equipo blanco
    team_blanco[0].id = 1; team_blanco[0].pos_x = 3; team_blanco[0].pos_y = 5; // B1
    team_blanco[1].id = 2; team_blanco[1].pos_x = 4; team_blanco[1].pos_y = 3; // B2
    team_blanco[2].id = 3; team_blanco[2].pos_x = 4; team_blanco[2].pos_y = 7; // B3
    team_blanco[3].id = 4; team_blanco[3].pos_x = 6; team_blanco[3].pos_y = 2; // B4
    team_blanco[4].id = 5; team_blanco[4].pos_x = 6; team_blanco[4].pos_y = 8; // B5

    // Colocar los jugadores en la cancha
    for (int i = 0; i < tamaño_equipo; i++) {
        cancha[team_rojo[i].pos_x][team_rojo[i].pos_y] = 'R'; // 'R' representa al jugador rojo
        cancha[team_blanco[i].pos_x][team_blanco[i].pos_y] = 'B'; // 'B' representa al jugador blanco
    }

    // Colocar la pelota en el centro de la cancha
    p->pos_x = 7; // Inicializar posición de la pelota
    p->pos_y = 5;
}

/*
 * Hasta aca
 */

int main(void) {
    jugador team_rojo[5];
    jugador team_blanco[5];
    char cancha[tamY][tamX];
    pelota p;

    // Inicializar la cancha y los jugadores
    inicializarTablero(cancha, team_rojo, team_blanco, 5, &p);

    imprimirCancha(cancha, &p);

    // Pedir movimiento al usuario
    pedirMovimiento(cancha, team_rojo, 5);
    pedirMovimiento(cancha, team_blanco, 5);
    pedirMovimientoPelota(cancha, &p);

    imprimirCancha(cancha, &p);

    return 0;
}
