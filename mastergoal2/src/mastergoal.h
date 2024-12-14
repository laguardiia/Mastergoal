#ifndef MASTERGOAL_H
#define MASTERGOAL_H

#include <gtk/gtk.h>

// Constantes
#define FILAS 15
#define COLUMNAS 11
#define MAX_MOVIMIENTOS_BALON 4
#define MAX_MOVIMIENTOS_JUGADOR 2




// Representaciones
extern const char *VACIO;
extern const char *BALON;
extern const char *JUGADOR1;
extern const char *JUGADOR2;
extern const char *ARCO;

// Estructura del juego
typedef struct {
    const char *tablero[FILAS][COLUMNAS];
    int turno;
    int goles_equipo1;
    int goles_equipo2;
    int movimientos_restantes;
    char jugador1_nombre[50];
    char jugador2_nombre[50];
    const char *color_jugador1;
    const char *color_jugador2;

    // Widgets para actualizar la interfaz
    GtkWidget *label_turno;
    GtkWidget *label_goles;
} Juego;



// Funciones
void inicializar_tablero(Juego *juego);
void graficar_tablero(GtkWidget *grid, Juego *juego);
int es_movimiento_valido(int x, int y, int nuevo_x, int nuevo_y, int max_distancia);
int contar_jugadores_adyacentes(Juego *juego, int x, int y, const char *jugador);
void mover_balón(Juego *juego, int x, int y, int nuevo_x, int nuevo_y);
void mover_jugador(Juego *juego, int x, int y, int nuevo_x, int nuevo_y);
void cambiar_turno(Juego *juego);
int verificar_fin_partida(Juego *juego);
int verificar_gol(Juego *juego, int x, int y);
void actualizar_info(GtkWidget *label_turno, GtkWidget *label_goles, Juego *juego);


#endif
