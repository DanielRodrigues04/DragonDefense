#include "game.h"

// Definição das variáveis globais
int sai = 0;
int width = 640; // Defina a largura desejada
int height = 480; // Defina a altura desejada
int escolha = -1; // Defina o valor inicial de escolha conforme necessário

BITMAP *buffer = NULL;
BITMAP *logo = NULL;
SAMPLE *somOpcao = NULL;
SAMPLE *somEnter = NULL;
SAMPLE *somInicio = NULL;

// Funções de inicialização e finalização do Allegro e recursos do jogo
void inicializarAllegro() {
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_window_title("Meu Programa");
    set_close_button_callback(sair);
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);
}

void finalizarAllegro() {
    allegro_exit();
}

void inicializarRecursos() {
    buffer = create_bitmap(width, height);
    logo = load_bitmap("DragonDefense.bmp", NULL);
    somOpcao = load_sample("swapMenu.wav");
    somEnter = load_sample("start.wav");
    somInicio = load_sample("intro.wav");

    if (!logo) {
        allegro_message("Não foi possível carregar a imagem DragonDefense.bmp!");
        exit(EXIT_FAILURE);
    }
}

void finalizarRecursos() {
    destroy_bitmap(buffer);
    destroy_bitmap(logo);
    destroy_sample(somOpcao);
    destroy_sample(somEnter);
    destroy_sample(somInicio);
}

void limparTela() {
    clear_bitmap(buffer);
}

void sair() {
    sai = 1;
}

