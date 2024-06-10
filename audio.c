#include <allegro.h>
#include "game.h"
#include "audio.h"

void tocarSom(const char *nomeArquivoSom) {
    SAMPLE *som = load_sample(nomeArquivoSom); // Carregar o arquivo de som
    if (som) {
        play_sample(som, 255, 128, 1000, 0); // Reproduzir o som
        destroy_sample(som); // Liberar a mem�ria do som ap�s a reprodu��o
    } else {
        allegro_message("N�o foi poss�vel carregar o arquivo de som: %s", nomeArquivoSom);
    }
}

void tocarMusicaInicio(SAMPLE *somInicio, bool loop) {
    if(loop)
        play_sample(somInicio, 255, 128, 1000, true); // Reproduz a m�sica de in�cio em loop
    else
        play_sample(somInicio, 255, 128, 1000, false); // Reproduz a m�sica de in�cio sem loop
}

void fadeOut(BITMAP *buffer) {
    int alpha; // Declarar a vari�vel alpha fora do loop
    BITMAP *tempBuffer = create_bitmap(width, height); // Criar um buffer tempor�rio para armazenar o conte�do
    
    for (alpha = 255; alpha >= 0; alpha -= 5) {
        clear_bitmap(tempBuffer); // Limpar o buffer tempor�rio
        draw_sprite(tempBuffer, buffer, 0, 0); // Copiar o conte�do do buffer para o buffer tempor�rio
        set_trans_blender(0, 0, 0, alpha);
        clear_bitmap(buffer); // Limpar o buffer original
        draw_trans_sprite(buffer, tempBuffer, 0, 0); // Desenhar o buffer tempor�rio com transpar�ncia no buffer original
        draw_sprite(screen, buffer, 0, 0); // Desenhar o buffer original na tela
        rest(20);
    }
    
    destroy_bitmap(tempBuffer); // Liberar a mem�ria do buffer tempor�rio
}

