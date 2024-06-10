#include <allegro.h>
#include "game.h"
#include "menu.h"
#include "audio.h"

void menu() {
    int flash = 0;
    int startClicado = false; // Vari�vel para controlar se o bot�o Start j� foi clicado

    // Tocar m�sica de in�cio em loop
    tocarMusicaInicio(somInicio, true); // Corrigido: Passando somInicio como argumento

    while (!(sai || key[KEY_ESC])) {
        draw_sprite(buffer, logo, 0, 0);
        
        // Anima��o do texto "Start"
        if (flash % 60 < 40) { // Faz o texto piscar a cada 40 frames
            textout_centre_ex(buffer, font, "Start", width / 2, height / 2, makecol(255, 255, 255), -1);
        }
        
        draw_sprite(screen, buffer, 0, 0);
        rest(10);
        clear_bitmap(buffer);
        
        flash++;
        
        // Verifica se o bot�o "Start" foi clicado (tecla Enter)
        if (key[KEY_ENTER] && !startClicado) {
            escolha = 0; // Define a escolha como "Start"
            startClicado = true; // Indica que o bot�o Start foi clicado
            clear_keybuf(); // Limpa o buffer de teclas
            rest(200); // Aguarda 200 milissegundos para evitar entrada r�pida
            tocarSom("start.wav"); // Corrigido: Passando o nome do arquivo de �udio
        }
        
        // Se o bot�o Start foi clicado, fa�a a transi��o de fade out e saia do loop
        if (startClicado) {
            fadeOut(buffer); // Chama a fun��o de fade out antes de sair do loop
            break;
        }
    }
}

void menu_opcoes() {
    clear_bitmap(buffer);
    int opcao = 0;
    int opcaoAnterior = 0; // Vari�vel para armazenar a op��o anterior
    int i; // Movendo a declara��o da vari�vel i para antes do loop

    while (!(sai || key[KEY_ESC])) {
        textout_centre_ex(buffer, font, "Escolha uma Dificuldade:", width / 2, 100, makecol(255, 255, 255), -1);
        
        // Desenha as op��es do menu
        textout_centre_ex(buffer, font, "1 - Iniciante", width / 2, 200, makecol(255, 255, 255), -1);
        textout_centre_ex(buffer, font, "2 - Cavaleiro", width / 2, 250, makecol(255, 255, 255), -1);
        textout_centre_ex(buffer, font, "3 - Veterano", width / 2, 300, makecol(255, 255, 255), -1);
        
        // Destaca a op��o selecionada
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
        
        // Verifica as teclas pressionadas para selecionar a op��o
        if (key[KEY_UP] && opcao > 0) {
            opcao--;
            // Tocar som de op��o
            tocarSom("swapMenu.wav"); // Corrigido: Passando o nome do arquivo de �udio
        } else if (key[KEY_DOWN] && opcao < 2) {
            opcao++;
            // Tocar som de op��o
            tocarSom("swapMenu.wav"); // Corrigido: Passando o nome do arquivo de �udio
        } else if (opcao != opcaoAnterior) { // Se a op��o mudou
            // Tocar som de op��o
            tocarSom("swapMenu.wav"); // Corrigido: Passando o nome do arquivo de �udio
            opcaoAnterior = opcao; // Atualizar a op��o anterior
        } else if (key[KEY_ENTER]) {
            // Define a escolha com base na op��o selecionada
            escolha = opcao + 1;
            clear_keybuf(); // Limpa o buffer de teclas
            break; // Sai do loop
        }
    }
    
    
}

void mostrar_dificuldade(int dificuldade) {
    // L�gica para mostrar a dificuldade escolhida
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
