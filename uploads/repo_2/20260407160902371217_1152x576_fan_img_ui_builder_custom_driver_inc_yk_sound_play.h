#include "../apps.h"
#ifdef YK_USE_UI_BUILDER


#if yk_sound_play_h


#ifndef __YK_SOUND_PLAY__
#define __YK_SOUND_PLAY__

void yk_f_sound_play_init(void);
void yk_f_sound_play(uint8_t *file_name);
void yk_f_sound_set_vol(uint8_t vol);

#endif


#endif


#endif  //YK_USE_UI_BUILDER