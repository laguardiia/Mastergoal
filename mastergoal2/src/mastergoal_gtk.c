#include <gtk/gtk.h>
#include "mastergoal.h"


// Prototipos de funciones
void mostrar_autores(GtkWidget *widget, gpointer window);
void mostrar_configuracion(GtkWidget *widget, gpointer data);
void mostrar_juego(Juego *juego);
void actualizar_interfaz(GtkWidget *label_turno, GtkWidget *label_goles, GtkWidget *grid, Juego *juego);
void graficar_tablero_interactivo(GtkWidget *grid, Juego *juego);

// Variables globales para controlar la selección
static int ficha_seleccionada = 0; // 0: ninguna, 1: seleccionada
static int seleccion_x = -1, seleccion_y = -1;


void actualizar_info(GtkWidget *label_turno, GtkWidget *label_goles, Juego *juego) {
    char turno_text[100], goles_text[100];
    snprintf(turno_text, sizeof(turno_text), "Turno: %s",
             juego->turno == 0 ? juego->jugador1_nombre : juego->jugador2_nombre);
    snprintf(goles_text, sizeof(goles_text), "Goles: %s %d - %d %s",
             juego->color_jugador1, juego->goles_equipo1, juego->goles_equipo2, juego->color_jugador2);

    gtk_label_set_text(GTK_LABEL(label_turno), turno_text);
    gtk_label_set_text(GTK_LABEL(label_goles), goles_text);
}

void actualizar_interfaz(GtkWidget *label_turno, GtkWidget *label_goles, GtkWidget *grid, Juego *juego) {
    actualizar_info(label_turno, label_goles, juego);
    graficar_tablero_interactivo(grid, juego);
}




// Función para manejar los clics en los botones del tablero
void boton_clicado(GtkWidget *widget, gpointer data) {
    struct {
        Juego *juego;
        GtkWidget *label_turno;
        GtkWidget *label_goles;
        GtkWidget *grid;
    } *contexto = data;

    Juego *juego = contexto->juego;
    int x, y;

    sscanf(gtk_widget_get_name(widget), "%d,%d", &x, &y);

    if (!ficha_seleccionada) {
        const char *celda = juego->tablero[x][y];
        if ((juego->turno == 0 && strcmp(celda, JUGADOR1) == 0) ||
            (juego->turno == 1 && strcmp(celda, JUGADOR2) == 0) ||
            strcmp(celda, BALON) == 0) {
            ficha_seleccionada = 1;
            seleccion_x = x;
            seleccion_y = y;
        }
    } else {
        int gol = 0;
        if (strcmp(juego->tablero[seleccion_x][seleccion_y], BALON) == 0) {
            gol = verificar_gol(juego, x, y);
            mover_balón(juego, seleccion_x, seleccion_y, x, y);
        } else {
            mover_jugador(juego, seleccion_x, seleccion_y, x, y);
        }

        if (!gol) {
            cambiar_turno(juego);
        }

        actualizar_interfaz(contexto->label_turno, contexto->label_goles, contexto->grid, juego);

        if (gol && verificar_fin_partida(juego)) {
            const char *ganador = (juego->goles_equipo1 == 2) ? juego->jugador1_nombre : juego->jugador2_nombre;
            GtkWidget *dialog = gtk_message_dialog_new(
                NULL,
                GTK_DIALOG_MODAL,
                GTK_MESSAGE_INFO,
                GTK_BUTTONS_OK,
                "\u00a1El equipo de %s ha ganado el juego!",
                ganador
            );
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            inicializar_tablero(juego);
            actualizar_interfaz(contexto->label_turno, contexto->label_goles, contexto->grid, juego);
        }

        ficha_seleccionada = 0;
        seleccion_x = -1;
        seleccion_y = -1;
    }
}




// Graficar el tablero como botones
void graficar_tablero_interactivo(GtkWidget *grid, Juego *juego) {
    gtk_container_foreach(GTK_CONTAINER(grid), (GtkCallback)gtk_widget_destroy, NULL);

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            GtkWidget *button = gtk_button_new_with_label(juego->tablero[i][j]);
            char name[10];
            snprintf(name, sizeof(name), "%d,%d", i, j);
            gtk_widget_set_name(button, name);

            struct {
                Juego *juego;
                GtkWidget *label_turno;
                GtkWidget *label_goles;
                GtkWidget *grid;
            } *contexto = g_new0(typeof(*contexto), 1);

            contexto->juego = juego;
            contexto->label_turno = juego->label_turno;  // Usa las referencias existentes
            contexto->label_goles = juego->label_goles;  // Usa las referencias existentes
            contexto->grid = grid;

            g_signal_connect(button, "clicked", G_CALLBACK(boton_clicado), contexto);

            gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);
        }
    }

    gtk_widget_show_all(grid);
}




// Mostrar la ventana del juego
void mostrar_juego(Juego *juego) {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Tablero de Juego");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Información de los jugadores y turnos
    GtkWidget *info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *label_j1 = gtk_label_new(NULL);
    GtkWidget *label_j2 = gtk_label_new(NULL);
    GtkWidget *label_turno = gtk_label_new(NULL);
    GtkWidget *label_goles = gtk_label_new(NULL);

    char info_j1[100], info_j2[100];
    snprintf(info_j1, sizeof(info_j1), "Jugador 1: %s (%s)", juego->jugador1_nombre, juego->color_jugador1);
    snprintf(info_j2, sizeof(info_j2), "Jugador 2: %s (%s)", juego->jugador2_nombre, juego->color_jugador2);

    gtk_label_set_text(GTK_LABEL(label_j1), info_j1);
    gtk_label_set_text(GTK_LABEL(label_j2), info_j2);

    gtk_box_pack_start(GTK_BOX(info_box), label_j1, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(info_box), label_j2, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(info_box), label_turno, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(info_box), label_goles, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), info_box, FALSE, FALSE, 5);

    // Tablero interactivo
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    // Asignar los labels al struct del juego para acceso global
    juego->label_turno = label_turno;
    juego->label_goles = label_goles;

    graficar_tablero_interactivo(grid, juego);
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 5);

    // Botones inferiores
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *btn_reiniciar = gtk_button_new_with_label("Reiniciar");
    GtkWidget *btn_salir = gtk_button_new_with_label("Salir");

    g_signal_connect(btn_reiniciar, "clicked", G_CALLBACK(inicializar_tablero), juego);
    g_signal_connect(btn_salir, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    gtk_box_pack_start(GTK_BOX(button_box), btn_reiniciar, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(button_box), btn_salir, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, FALSE, FALSE, 5);

    actualizar_info(label_turno, label_goles, juego);  // Inicializar los valores de los labels

    gtk_widget_show_all(window);
}


// Mostrar ventana de configuración
void mostrar_configuracion(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Configurar Juego",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "Aceptar", GTK_RESPONSE_ACCEPT,
        "Cancelar", GTK_RESPONSE_REJECT,
        NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Configuración de nombres
    GtkWidget *label_j1 = gtk_label_new("Nombre Jugador 1:");
    GtkWidget *entry_j1 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_j1), "Jugador 1");
    gtk_box_pack_start(GTK_BOX(content_area), label_j1, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content_area), entry_j1, FALSE, FALSE, 5);

    GtkWidget *label_j2 = gtk_label_new("Nombre Jugador 2:");
    GtkWidget *entry_j2 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_j2), "Jugador 2");
    gtk_box_pack_start(GTK_BOX(content_area), label_j2, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content_area), entry_j2, FALSE, FALSE, 5);

    // Configuración de colores
    GtkWidget *label_color = gtk_label_new("Color Jugador 1:");
    GtkWidget *combo_color = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_color), NULL, "🔴 Rojo");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_color), NULL, "⚪ Blanco");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_color), NULL, "Aleatorio");
    gtk_box_pack_start(GTK_BOX(content_area), label_color, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content_area), combo_color, FALSE, FALSE, 5);

    // Configuración de turno inicial
    GtkWidget *label_turno = gtk_label_new("Turno inicial:");
    GtkWidget *combo_turno = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_turno), NULL, "Jugador 1");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_turno), NULL, "Jugador 2");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_turno), NULL, "Aleatorio");
    gtk_box_pack_start(GTK_BOX(content_area), label_turno, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content_area), combo_turno, FALSE, FALSE, 5);

    gtk_widget_show_all(dialog);

    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        const char *nombre_j1 = gtk_entry_get_text(GTK_ENTRY(entry_j1));
        const char *nombre_j2 = gtk_entry_get_text(GTK_ENTRY(entry_j2));

        const char *color = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_color));
        if (g_strcmp0(color, "Aleatorio") == 0) {
            srand(time(NULL));
            color = (rand() % 2 == 0) ? "🔴 Rojo" : "🅾️ Blanco";
        }
        const char *color_j1 = color;
        const char *color_j2 = g_strcmp0(color, "🔴 Rojo") == 0 ? "🅾️ Blanco" : "🔴 Rojo";

        const char *turno = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_turno));
        if (g_strcmp0(turno, "Aleatorio") == 0) {
            turno = (rand() % 2 == 0) ? "Jugador 1" : "Jugador 2";
        }
        int turno_inicial = g_strcmp0(turno, "Jugador 1") == 0 ? 0 : 1;

        Juego *juego = malloc(sizeof(Juego));
        strncpy(juego->jugador1_nombre, nombre_j1, sizeof(juego->jugador1_nombre) - 1);
        strncpy(juego->jugador2_nombre, nombre_j2, sizeof(juego->jugador2_nombre) - 1);
        juego->color_jugador1 = color_j1;
        juego->color_jugador2 = color_j2;
        juego->turno = turno_inicial;

        inicializar_tablero(juego);
        mostrar_juego(juego);
    }

    gtk_widget_destroy(dialog);
}
void mostrar_autores(GtkWidget *widget, gpointer window) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Información de los Autores",
        GTK_WINDOW(window),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "Cerrar", GTK_RESPONSE_CLOSE,
        NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *label = gtk_label_new(
        "<b>Trabajo Final de Lenguaje de Programación 1</b>\n"
    	"\n\n⚽ MASTERGOAL ⚽\n\n"
        "\nAutores: Luis Morel y Guillermo Laguardia\n"
        "\nAño 2024\n");
    gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
    gtk_box_pack_start(GTK_BOX(content_area), label, TRUE, TRUE, 0);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Main
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "⚽ MASTERGOAL ⚽ by: Luis y Sebas");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *btn_iniciar = gtk_button_new_with_label("Iniciar Juego");
    GtkWidget *btn_estadisticas = gtk_button_new_with_label("Estadísticas");
    GtkWidget *btn_autores = gtk_button_new_with_label("Información sobre los Autores");
    GtkWidget *btn_salir = gtk_button_new_with_label("Salir");

    g_signal_connect(btn_iniciar, "clicked", G_CALLBACK(mostrar_configuracion), window);
    g_signal_connect(btn_autores, "clicked", G_CALLBACK(mostrar_autores), window);
    g_signal_connect(btn_salir, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    gtk_box_pack_start(GTK_BOX(box), btn_iniciar, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(box), btn_estadisticas, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(box), btn_autores, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(box), btn_salir, TRUE, TRUE, 5);

    gtk_container_add(GTK_CONTAINER(window), box);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
