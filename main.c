#include <allegro.h>
#include <allegro/font.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

// Definição de true e false caso não estejam disponíveis
#ifndef true
    #define true 1
#endif

#ifndef false
    #define false 0
#endif

// Funções
void sair();
void menu();
void menu_opcoes();
void mostrar_dificuldade(int dificuldade);
void fadeOut(BITMAP *buffer);
void mostrar_cenario_jogo();
void logica_jogo();
void desenhar_personagem(int x, int y, int current_frame_y, int frame);
void iniciar_timer(int dificuldade);
void mostrar_tempo(int tempo_restante);
void criar_bolinhas();
void desenhar_bolinhas();
void checar_colisao(int x, int y, int *score);
void adicionar_bolinha();
void desenhar_personagem_duplo(int x1, int y1, int current_frame_y1, int frame1, int x2, int y2, int current_frame_y2, int frame2);

// Estruturas
typedef struct {
    int x, y;
    int ativo;
} Bolinha;

// Variáveis Globais
int sai = 0;
int width = 800;
int height = 600;
int escolha = -1;
int dificuldade = -1;
int score1 = 0, score2 = 0;

//ponteiros
BITMAP *buffer, *logo, *charDragon;
SAMPLE *somOpcao, *somEnter, *somInicio, *somColisao;
BITMAP *cenarioJogo, *sprite, *sprite2;
time_t tempo_inicial;

//variaveis globais para tempo e bolinhas
int tempo_total;
Bolinha bolinhas[10];
int intervalo_criacao_bolinha = 2000; // Intervalo de tempo em milissegundos para criar novas bolinhas
time_t tempo_ultima_criacao;

// Função principal
int main() {
    // Iniciação
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_window_title("Dragon Defense");
    set_close_button_callback(sair);
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);  
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);
    
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Variáveis Locais
    buffer = create_bitmap(width, height);
    sprite = load_bitmap("img/dragon.bmp", NULL);
    sprite2 = load_bitmap("img/dragon2.bmp",NULL);
    logo = load_bitmap("img/DragonDefense.bmp", NULL);
    somOpcao = load_sample("swapMenu.wav"); 
    somEnter = load_sample("start.wav");
    somInicio = load_sample("intro.wav");
    somColisao = load_sample("somColisao.wav");
    

    
    if(!sprite2){
    	allegro_message("Não foi possivel carregar a imagem dragon2.bmp");
    	return 1;
	}
    
    if (!sprite) {
        allegro_message("Não foi possível carregar a imagem dragon.bmp!");
        return 1;
    } 
    
    if (!logo) {
        allegro_message("Não foi possível carregar a imagem DragonDefense.bmp!");
        return 1;
    }

    while (!(sai || key[KEY_ESC])) {        
        menu();
        
        // Após selecionar "Start", exibe o menu de opções
        if (escolha == 0) {
            menu_opcoes();
            
            // Se uma dificuldade foi escolhida, mostrar o cenário do jogo
            if (dificuldade != -1) {
                mostrar_cenario_jogo();
                iniciar_timer(dificuldade);
                criar_bolinhas();
                tempo_ultima_criacao = time(NULL);
                logica_jogo();
            }
        }
    }
    
    // Finalização
    destroy_bitmap(buffer);
    destroy_bitmap(logo);
    destroy_bitmap(sprite);
    destroy_sample(somOpcao);
    destroy_sample(somEnter);
    destroy_sample(somInicio);
   
    return 0;
}
END_OF_MAIN();

void menu() {
    int flash = 0;
    int startClicado = false;

    // Tocar música de início em loop
    play_sample(somInicio, 255, 128, 1000, true);

    while (!(sai || key[KEY_ESC])) {
        draw_sprite(buffer, logo, 0, 0);
        
        // Animação do texto "Start"
        if (flash % 60 < 40) {
            textout_centre_ex(buffer, font, "Start", width / 2, height / 2, makecol(255, 255, 255), -1);
        }
        
        draw_sprite(screen, buffer, 0, 0);
        rest(10);
        clear_bitmap(buffer);
        
        flash++;
        
        // Verifica se o botão "Start" foi clicado (tecla Enter)
        if (key[KEY_ENTER] && !startClicado) {
            escolha = 0;
            startClicado = true;
            clear_keybuf();
            rest(200);
            play_sample(somEnter, 255, 128, 1000, 0);

            // Faz a transição de fade out
            fadeOut(buffer);
            break;
        }
    }
}

void menu_opcoes() {
    clear_bitmap(buffer);
    int opcao = 0;
    int opcaoAnterior = 0;

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
            rest(100);
        } else if (key[KEY_DOWN] && opcao < 2) {
            opcao++;
            rest(100);
        } else if (opcao != opcaoAnterior) {
            // Tocar som de opção
            play_sample(somOpcao, 255, 128, 1000, 0);
            opcaoAnterior = opcao;
            rest(100);
        } else if (key[KEY_ENTER]) {
            dificuldade = opcao + 1;
            clear_keybuf();
            play_sample(somEnter, 255, 128, 1000, 0);
            fadeOut(buffer);
            mostrar_cenario_jogo();
            logica_jogo();
            break;
        }
    }
}

void mostrar_dificuldade(int dificuldade) {
    clear_bitmap(buffer);
    
    switch (dificuldade) {
        case 1:
            textout_centre_ex(buffer, font, "Dificuldade: Iniciante", width / 2, height / 2, makecol(255, 255, 255), -1);
            break;
        case 2:
            textout_centre_ex(buffer, font, "Dificuldade: Cavaleiro", width / 2, height / 2, makecol(255, 255, 255), -1);
            break;
        case 3:
            textout_centre_ex(buffer, font, "Dificuldade: Veterano", width / 2, height / 2, makecol(255, 255, 255), -1);
            break;
    }
}

void fadeOut(BITMAP *buffer) {
    int alpha;
    BITMAP *tempBuffer = create_bitmap(width, height);
    
    for (alpha = 255; alpha >= 0; alpha -= 5) {
        clear_bitmap(tempBuffer);
        draw_sprite(tempBuffer, buffer, 0, 0);
        set_trans_blender(0, 0, 0, alpha);
        clear_bitmap(buffer);
        draw_trans_sprite(buffer, tempBuffer, 0, 0);
        draw_sprite(screen, buffer, 0, 0);
        rest(20);
    }
    
    destroy_bitmap(tempBuffer);
}

void mostrar_cenario_jogo() {
    clear_bitmap(buffer);

    if (cenarioJogo) {
        destroy_bitmap(cenarioJogo);
        cenarioJogo = NULL;
    }

    cenarioJogo = load_bitmap("img/cenarioJogo.bmp", NULL);
    if (!cenarioJogo) {
        allegro_message("Não foi possível carregar a imagem cenarioJogo.bmp!");
        exit(1);
    }

    draw_sprite(buffer, cenarioJogo, 0, 0);
    draw_sprite(screen, buffer, 0, 0);
}

void iniciar_timer(int dificuldade) {
    switch (dificuldade) {
        case 1:
            tempo_total = 3 * 60; // 3:00 minutos para Iniciante
            break;
        case 2:
            tempo_total = 2 * 60 + 30; // 2:30 minutos para Cavaleiro
            break;
        case 3:
            tempo_total = 2 * 60; // 2:00 minutos para Veterano
            break;
    }
    tempo_inicial = time(NULL);
}

void mostrar_tempo(int tempo_restante) {
    int minutos = tempo_restante / 60;
    int segundos = tempo_restante % 60;
    char buffer_tempo[10];
    sprintf(buffer_tempo, "%02d:%02d", minutos, segundos);
    textout_centre_ex(buffer, font, buffer_tempo, width - 60, 30, makecol(255, 255, 255), -1);
}

void criar_bolinhas() {
    int i;
    for (i = 0; i < 10; i++) {
        bolinhas[i].x = rand() % (width - 20) + 10;
        bolinhas[i].y = rand() % (height - 20) + 10;
        bolinhas[i].ativo = true;
    }
}

void adicionar_bolinha() {
    int i;
    for (i = 0; i < 10; i++) {
        if (!bolinhas[i].ativo) {
            bolinhas[i].x = rand() % (width - 20) + 10;
            bolinhas[i].y = rand() % (height - 20) + 10;
            bolinhas[i].ativo = true;
            break;
        }
    }
}

void desenhar_bolinhas() {
    int i;
    for (i = 0; i < 10; i++) {
        if (bolinhas[i].ativo) {
            circlefill(buffer, bolinhas[i].x, bolinhas[i].y, 10, makecol(255, 0, 0));
        }
    }
}

void checar_colisao(int x, int y, int *score) {
    int i;
    for (i = 0; i < 10; i++) {
        if (bolinhas[i].ativo) {
            int dist = sqrt((x - bolinhas[i].x) * (x - bolinhas[i].x) + (y - bolinhas[i].y) * (y - bolinhas[i].y));
            if (dist < 20) {
                bolinhas[i].ativo = false;
                (*score) += 2;
                // Reproduz o som de colisão
                play_sample(somColisao, 255, 128, 1000, 0);
            }
        }
    }
}



void logica_jogo() {
    float frame1 = 0.f, frame2 = 0.f;
    int pos_x1 = width / 2, pos_y1 = height / 2;
    int pos_x2 = width / 2 + 100, pos_y2 = height / 2 + 100;
    int current_frame_y1 = 161, current_frame_y2 = 161;
    int tempo_restante;

    while (!(sai || key[KEY_ESC])) {
        // Controle do dragão 1 (teclas de seta)
        if (key[KEY_RIGHT]) {
            current_frame_y1 = 161;
            pos_x1 += 2;
        } else if (key[KEY_LEFT]) {
            current_frame_y1 = 161 * 3;
            pos_x1 -= 2;
        } else if (key[KEY_DOWN]) {
            current_frame_y1 = 161 * 2;
            pos_y1 += 2;
        } else if (key[KEY_UP]) {
            current_frame_y1 = 0;
            pos_y1 -= 2;
        }

        // Controle do dragão 2 (teclas WASD)
        if (key[KEY_D]) {
            current_frame_y2 = 161;
            pos_x2 += 2;
        } else if (key[KEY_A]) {
            current_frame_y2 = 161 * 3;
            pos_x2 -= 2;
        } else if (key[KEY_S]) {
            current_frame_y2 = 161 * 2;
            pos_y2 += 2;
        } else if (key[KEY_W]) {
            current_frame_y2 = 0;
            pos_y2 -= 2;
        }

        frame1 += 0.3f;
        if (frame1 > 3) {
            frame1 -= 3;
        }

        frame2 += 0.3f;
        if (frame2 > 3) {
            frame2 -= 3;
        }

        // Calcula o tempo restante
        tempo_restante = tempo_total - (int)(time(NULL) - tempo_inicial);

       if (tempo_restante <= 0) {
            textout_centre_ex(buffer, font, "Tempo Esgotado!", width / 2, height / 2, makecol(255, 0, 0), -1);
            draw_sprite(screen, buffer, 0, 0);
            rest(2000); // Pausa por 2 segundos antes de sair
            
            // Determina o jogador vencedor
            if (score1 > score2) {
                textout_centre_ex(buffer, font, "Jogador 1 Venceu!", width / 2, height / 2 + 20, makecol(255, 0, 0), -1);
            } else if (score2 > score1) {
                textout_centre_ex(buffer, font, "Jogador 2 Venceu!", width / 2, height / 2 + 20, makecol(255, 255 , 0), -1);
            } else {
                textout_centre_ex(buffer, font, "Empate!", width / 2, height / 2 + 20, makecol(255, 255, 255), -1);
            }
            draw_sprite(screen, buffer, 0, 0);
            rest(2000); // Pausa por 2 segundos antes de sair
            break;
        }

        // Cria uma nova bolinha a cada intervalo de tempo
        if (time(NULL) - tempo_ultima_criacao >= intervalo_criacao_bolinha / 1000) {
            adicionar_bolinha();
            tempo_ultima_criacao = time(NULL);
        }

        draw_sprite(buffer, cenarioJogo, 0, 0);
        desenhar_personagem_duplo(pos_x1, pos_y1, current_frame_y1, (int)frame1, pos_x2, pos_y2, current_frame_y2, (int)frame2);
        desenhar_bolinhas();
        checar_colisao(pos_x1 + 95, pos_y1 + 80, &score1); // Verifica colisão com o centro do dragão 1
        checar_colisao(pos_x2 + 95, pos_y2 + 80, &score2); // Verifica colisão com o centro do dragão 2

        // Mostra o tempo restante na tela
        mostrar_tempo(tempo_restante);

        // Mostra a pontuação na tela
        char buffer_pontos[40];
        sprintf(buffer_pontos, "Score1: %d | Score2: %d", score1, score2);
        textout_centre_ex(buffer, font, buffer_pontos, width - 100, 60, makecol(255, 255, 255), -1);

        draw_sprite(screen, buffer, 0, 0);
        rest(30);
    }
}

void desenhar_personagem_duplo(int x1, int y1, int current_frame_y1, int frame1, int x2, int y2, int current_frame_y2, int frame2) {
    draw_sprite(buffer, cenarioJogo, 0, 0);
    masked_blit(sprite, buffer, 191 * frame1, current_frame_y1, x1, y1, 191, 161);
    masked_blit(sprite2, buffer, 191 * frame2, current_frame_y2, x2, y2, 191, 161);
    draw_sprite(screen, buffer, 0, 0);
}

void sair() {
    sai = 1;
}
END_OF_FUNCTION(sair);

