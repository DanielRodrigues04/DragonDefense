#include <allegro.h>
#include "game.h"
#include "audio.h"

void tocarSom(const char *nomeArquivoSom) {
    SAMPLE *som = load_sample(nomeArquivoSom); // Carregar o arquivo de som
    if (som) {
        play_sample(som, 255, 128, 1000, 0); // Reproduzir o som
        destroy_sample(som); // Liberar a memória do som após a reprodução
    } else {
        allegro_message("Não foi possível carregar o arquivo de som: %s", nomeArquivoSom);
    }
}

void tocarMusicaInicio(SAMPLE *somInicio, bool loop) {
    if(loop)
        play_sample(somInicio, 255, 128, 1000, true); // Reproduz a música de início em loop
    else
        play_sample(somInicio, 255, 128, 1000, false); // Reproduz a música de início sem loop
}

void fadeOut(BITMAP *buffer) {
    int alpha; // Declarar a variável alpha fora do loop
    BITMAP *tempBuffer = create_bitmap(width, height); // Criar um buffer temporário para armazenar o conteúdo
    
    for (alpha = 255; alpha >= 0; alpha -= 5) {
        clear_bitmap(tempBuffer); // Limpar o buffer temporário
        draw_sprite(tempBuffer, buffer, 0, 0); // Copiar o conteúdo do buffer para o buffer temporário
        set_trans_blender(0, 0, 0, alpha);
        clear_bitmap(buffer); // Limpar o buffer original
        draw_trans_sprite(buffer, tempBuffer, 0, 0); // Desenhar o buffer temporário com transparência no buffer original
        draw_sprite(screen, buffer, 0, 0); // Desenhar o buffer original na tela
        rest(20);
    }
    
    destroy_bitmap(tempBuffer); // Liberar a memória do buffer temporário
}

