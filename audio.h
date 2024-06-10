#ifndef AUDIO_H
#define AUDIO_H

#include <allegro.h>
#include <stdbool.h> // Incluindo este cabeçalho para usar o tipo bool

void tocarSom(const char *nomeArquivoSom);
void tocarMusicaInicio(SAMPLE *somInicio, bool loop);
void fadeOut(BITMAP *buffer);

#endif

