#include <allegro.h>
#include "game.h"
#include "menu.h"
#include "audio.h"

void menu() {
    int flash = 0;
    int startClicado = false; // Variável para controlar se o botão Start já foi clicado

    // Tocar música de início em loop
    tocarMusicaInicio(somInicio, true); // Corrigido: Passando somInicio como argumento

    while (!(sai || key[KEY_ESC])) {
        draw_sprite(buffer, logo, 0, 0);
        
        // Animação do texto "Start"
        if (flash % 60 < 40) { // Faz o texto piscar a cada 40 frames
            textout_centre_ex(buffer, font, "Start", width / 2, height / 2, makecol(255, 255, 255), -1);
        }
        
        draw_sprite(screen, buffer, 0, 0);
        rest(10);
        clear_bitmap(buffer);
        
        flash++;
        
        // Verifica se o botão "Start" foi clicado (tecla Enter)
        if (key[KEY_ENTER] && !startClicado) {
            escolha = 0; // Define a escolha como "Start"
            startClicado = true; // Indica que o botão Start foi clicado
            clear_keybuf(); // Limpa o buffer de teclas
            rest(200); // Aguarda 200 milissegundos para evitar entrada rápida
            tocarSom("start.wav"); // Corrigido: Passando o nome do arquivo de áudio
        }
        
        // Se o botão Start foi clicado, faça a transição de fade out e saia do loop
        if (startClicado) {
            fadeOut(buffer); // Chama a função de fade out antes de sair do loop
            break;
        }
    }
}

void menu_opcoes() {
    clear_bitmap(buffer);
    int opcao = 0;
    int opcaoAnterior = 0; // Variável para armazenar a opção anterior
    int i; // Movendo a declaração da variável i para antes do loop

    while (!(sai || key[KEY_ESC])) {
        textout_centre_ex(buffer, font, "Escolha uma Dificuldade:", width / 2, 100, makecol(255, 255, 255), -1);
        
        // Desenha as opções do menu
        textout_centre_ex(buffer, font, "1 - Iniciante", width / 2, 200, makecol(255, 255, 255), -1);
        textout_centre_ex(buffer, font, "2 - Cavaleiro", width / 2, 250, makecol(255, 255, 255), -1);
        textout_centre_ex(buffer, font, "3 - Veterano", width / 2, 300, makecol(255, 255, 255), -1);
        
        // Destaca a opção selecionada
        switch(opcao) {
            case 0:
                textout_ex(buffer, font, "->", width / 2 - 100, 200, makecol(255, 255, 255), -1);
                break;
            case 1:
                textout_ex(buffer, font, "->", width / 2 - 100, 250, makecol(255, 255, 255), -1);
                break;
            case 2:
                textout_ex(buffer, font, "->", width / 2 - 100, 300, makecol(255, 255, 255), -1);
                break;
        }
        
        draw_sprite(screen, buffer, 0, 0);
        rest(10);
        clear_bitmap(buffer);
        
        // Verifica as teclas pressionadas para selecionar a opção
        if (key[KEY_UP] && opcao > 0) {
            opcao--;
            // Tocar som de opção
            tocarSom("swapMenu.wav"); // Corrigido: Passando o nome do arquivo de áudio
        } else if (key[KEY_DOWN] && opcao < 2) {
            opcao++;
            // Tocar som de opção
            tocarSom("swapMenu.wav"); // Corrigido: Passando o nome do arquivo de áudio
        } else if (opcao != opcaoAnterior) { // Se a opção mudou
            // Tocar som de opção
            tocarSom("swapMenu.wav"); // Corrigido: Passando o nome do arquivo de áudio
            opcaoAnterior = opcao; // Atualizar a opção anterior
        } else if (key[KEY_ENTER]) {
            // Define a escolha com base na opção selecionada
            escolha = opcao + 1;
            clear_keybuf(); // Limpa o buffer de teclas
            break; // Sai do loop
        }
    }
    
    
}

void mostrar_dificuldade(int dificuldade) {
    // Lógica para mostrar a dificuldade escolhida
    switch (dificuldade) {
        case 1:
            textout_centre_ex(buffer, font, "Dificuldade: Iniciante", width / 2, 350, makecol(255, 255, 255), -1);
            break;
        case 2:
            textout_centre_ex(buffer, font, "Dificuldade: Cavaleiro", width / 2, 350, makecol(255, 255, 255), -1);
            break;
        case 3:
            textout_centre_ex(buffer, font, "Dificuldade: Veterano", width / 2, 350, makecol(255, 255, 255), -1);
            break;
        default:
            textout_centre_ex(buffer, font, "Dificuldade: Desconhecida", width / 2, 350, makecol(255, 255, 255), -1);
            break;
    }
}
