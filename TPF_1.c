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

//Estructura para cada jugador con un identificatorio, y su fila y columna
typedef struct {

	int id;
	int pos_x;
	int pos_y;

} jugador;

int main(void) {

	//Vector de estructuras para cada equipo
	jugador team_rojo[5];
	jugador team_azul[5];

	//El estado inicial de la cancha
	char cancha[15][11]= {
			{'-','-','-',' ',' ',' ',' ',' ','-','-','-'}, //Arco
			{'.',' ',' ','.','.','.','.','.',' ',' ','.'}, //Puntos especiales
			{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
			{' ',' ',' ',' ',' ','R',' ',' ',' ',' ',' '},
			{' ',' ',' ','R',' ',' ',' ','R',' ',' ',' '},
			{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
			{' ',' ','R',' ',' ',' ',' ',' ','R',' ',' '},
			{' ',' ',' ',' ',' ','O',' ',' ',' ',' ',' '}, //Pelota
			{' ',' ','B',' ',' ',' ',' ',' ','B',' ',' '},
			{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
			{' ',' ',' ','B',' ',' ',' ','B',' ',' ',' '},
			{' ',' ',' ',' ',' ','B',' ',' ',' ',' ',' '},
			{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
			{'.',' ',' ','.','.','.','.','.',' ',' ','.'}, //Puntos especiales
			{'-','-','-',' ',' ',' ',' ',' ','-','-','-'}, //Arco

	};

	// Imprimir la matriz
	    for (int i = 0; i < 15; i++) {
	        for (int j = 0; j < 11; j++) {
	            printf("%c  ", cancha[i][j]);
	        }
	        printf("\n");
	    }


	return EXIT_SUCCESS;
}
