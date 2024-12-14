#include "mastergoal.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Representaciones
const char *VACIO = "⬜";
const char *BALON = "⚽";
const char *JUGADOR1 = "🔴";
const char *JUGADOR2 = "🅾️";
const char *ARCO = "🟩";
const char *INHABITABLE = "🟦"; // Espacios no válidos (costados de los arcos)

void inicializar_tablero(Juego *juego) {
    // Inicializar todas las casillas como vacías
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            juego->tablero[i][j] = VACIO;
        }
    }

    // Colocar los espacios no válidos en los costados de los arcos
    for (int j = 0; j < COLUMNAS; j++) {
        if (j < 3 || j > 7) {
            juego->tablero[0][j] = INHABITABLE;            // Espacios superiores
            juego->tablero[FILAS - 1][j] = INHABITABLE;    // Espacios inferiores
        }
    }

    // Colocar los arcos en las filas superiores e inferiores
    for (int j = 3; j <= 7; j++) {
        juego->tablero[0][j] = ARCO;            // Arco superior
        juego->tablero[FILAS - 1][j] = ARCO;    // Arco inferior
    }

    // Colocar las fichas del Equipo 1 (🔴)
    juego->tablero[4][3] = JUGADOR1;
    juego->tablero[4][7] = JUGADOR1;
    juego->tablero[6][2] = JUGADOR1;
    juego->tablero[6][8] = JUGADOR1;
    juego->tablero[3][5] = JUGADOR1; // Portero del Equipo 1

    // Colocar las fichas del Equipo 2 (⚪)
    juego->tablero[FILAS - 5][3] = JUGADOR2;
    juego->tablero[FILAS - 5][7] = JUGADOR2;
    juego->tablero[FILAS - 7][2] = JUGADOR2;
    juego->tablero[FILAS - 7][8] = JUGADOR2;
    juego->tablero[FILAS - 4][5] = JUGADOR2; // Portero del Equipo 2

    // Colocar el balón en el centro del tablero
    juego->tablero[FILAS / 2][COLUMNAS / 2] = BALON;

    // Inicializar los datos del juego
    juego->turno = 0; // Empieza el equipo 1 por defecto
    juego->goles_equipo1 = 0;
    juego->goles_equipo2 = 0;
    juego->movimientos_restantes = 1;
}

int es_arco(int x, int y) {
    // Asume que las filas superiores (0) y las inferiores (FILAS - 1) son los arcos
    // Ajusta las columnas si el arco ocupa una región específica
    if ((x == 0 || x == FILAS - 1) && (y >= 3 && y <= 7)) { // Ejemplo: columnas del 3 al 7
        return 1;
    }
    return 0;
}


int verificar_gol(Juego *juego, int x, int y) {
    // Verificar si la casilla corresponde al arco
    if (x == 0 && juego->tablero[x][y] == BALON) {
        juego->goles_equipo2++; // Equipo rojo mete gol
        return 1;
    }
    if (x == FILAS - 1 && juego->tablero[x][y] == BALON) {
        juego->goles_equipo1++; // Equipo blanco mete gol
        return 2;
    }
    return 0;
}

void graficar_tablero(GtkWidget *grid, Juego *juego) {
    gtk_container_foreach(GTK_CONTAINER(grid), (GtkCallback)gtk_widget_destroy, NULL);

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            GtkWidget *label = gtk_label_new(juego->tablero[i][j]);
            gtk_grid_attach(GTK_GRID(grid), label, j, i, 1, 1);
        }
    }

    gtk_widget_show_all(grid);
}

int es_movimiento_valido(int x, int y, int nuevo_x, int nuevo_y, int max_distancia) {
    int dx = abs(nuevo_x - x);
    int dy = abs(nuevo_y - y);
    return (dx <= max_distancia && dy <= max_distancia && (dx == 0 || dy == 0 || dx == dy));
}

int contar_jugadores_adyacentes(Juego *juego, int x, int y, const char *jugador) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int nx = x + i;
            int ny = y + j;
            if (nx >= 0 && nx < FILAS && ny >= 0 && ny < COLUMNAS) {
                if (juego->tablero[nx][ny] == jugador) {
                    count++;
                }
            }
        }
    }
    return count;

}

void mover_balón(Juego *juego, int x, int y, int nuevo_x, int nuevo_y) {
    if (juego->movimientos_restantes == 0 || !es_movimiento_valido(x, y, nuevo_x, nuevo_y, MAX_MOVIMIENTOS_BALON)) {
        return;
    }

    int adyacentes_j1 = contar_jugadores_adyacentes(juego, x, y, JUGADOR1);
    int adyacentes_j2 = contar_jugadores_adyacentes(juego, x, y, JUGADOR2);

    if ((juego->turno == 0 && adyacentes_j1 <= adyacentes_j2) || (juego->turno == 1 && adyacentes_j2 <= adyacentes_j1)) {
        return; // Sin posesión del balón
    }

    juego->tablero[x][y] = VACIO;
    juego->tablero[nuevo_x][nuevo_y] = BALON;
    juego->movimientos_restantes--;

    // Si el balón llega a un arco
    if (es_arco(nuevo_x, nuevo_y)) {
        if (juego->turno == 0) {
            juego->goles_equipo1++;
        } else {
            juego->goles_equipo2++;
        }

        // Mostrar el mensaje de "GOOOL"
        GtkWidget *dialog = gtk_message_dialog_new(
            NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "¡GOOOL! El equipo de %s ha anotado.",
            juego->turno == 0 ? juego->jugador1_nombre : juego->jugador2_nombre
        );
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        // Actualizar el marcador en la interfaz
        actualizar_info(juego->label_turno, juego->label_goles, juego);

        // Verificar si alguno de los equipos ha ganado
        if (juego->goles_equipo1 == 2 || juego->goles_equipo2 == 2) {
            const char *ganador = (juego->goles_equipo1 == 2) ? juego->jugador1_nombre : juego->jugador2_nombre;
            GtkWidget *dialog_final = gtk_message_dialog_new(
                NULL,
                GTK_DIALOG_MODAL,
                GTK_MESSAGE_INFO,
                GTK_BUTTONS_OK,
                "¡El equipo de %s ha ganado el juego!",
                ganador
            );
            gtk_dialog_run(GTK_DIALOG(dialog_final));
            gtk_widget_destroy(dialog_final);

            // Reinicializar el juego
            inicializar_tablero(juego);
            actualizar_info(juego->label_turno, juego->label_goles, juego);
        } else {
            // Reinicializar el tablero si no se termina el juego
            inicializar_tablero(juego);
            actualizar_info(juego->label_turno, juego->label_goles, juego);
        }
    }
}






void mover_jugador(Juego *juego, int x, int y, int nuevo_x, int nuevo_y) {
    if (juego->movimientos_restantes == 0 || !es_movimiento_valido(x, y, nuevo_x, nuevo_y, MAX_MOVIMIENTOS_JUGADOR)) {
        return;
    }

    const char *jugador = juego->turno == 0 ? JUGADOR1 : JUGADOR2;
    if (juego->tablero[x][y] != jugador || juego->tablero[nuevo_x][nuevo_y] != VACIO) {
        return;
    }

    juego->tablero[x][y] = VACIO;
    juego->tablero[nuevo_x][nuevo_y] = jugador;
    juego->movimientos_restantes--;
}

void cambiar_turno(Juego *juego) {
    juego->turno = (juego->turno == 0) ? 1 : 0;  // Cambiar turno entre jugador 1 y jugador 2
    juego->movimientos_restantes = MAX_MOVIMIENTOS_JUGADOR;  // Reiniciar los movimientos
}



int verificar_fin_partida(Juego *juego) {
    if (juego->goles_equipo1 == 2 || juego->goles_equipo2 == 2) {
        return 1;
    }

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (juego->tablero[i][j] == BALON) {
                return 0;
            }
        }
    }
    return 1;
}
