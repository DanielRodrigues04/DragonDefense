#ifndef GAME_H
#define GAME_H

#include <allegro.h>

extern int sai;
extern int width;
extern int height;
extern int escolha;

extern BITMAP *buffer;
extern BITMAP *logo;
extern SAMPLE *somOpcao;
extern SAMPLE *somEnter;
extern SAMPLE *somInicio;

void inicializarAllegro();
void finalizarAllegro();
void inicializarRecursos();
void finalizarRecursos();
void sair();

#endif /* GAME_H */

