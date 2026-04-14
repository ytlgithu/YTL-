#include "../apps.h"
#ifdef YK_USE_UI_BUILDER


#if yk_sound_play_h


//播放 MP3 文件测试
struct mini_audio_player *player = NULL;
static uint8_t vol = 100;

// 自定义MP3声音播放初始化
void yk_f_sound_play_init(void)
{
    printf("yk_f_sound_play_init\n");
    player = mini_audio_player_create();
    if (player == NULL) {
        printf("mini_audio_player_create fail!!!\n");
    }
    mini_audio_player_set_volume(player,vol);
}

//自定义播放MP3文件
void yk_f_sound_play(uint8_t *file_name)
{
    printf("yk_f_sound_play\n");
    uint8_t file_path[100];
    sprintf(file_path,"/rodata/sound/%s.mp3",file_name);
    printf("file_path:%s\n",file_path);
    mini_audio_player_play(player,file_path);
}

//自定义设置声音大小
void yk_f_sound_set_vol(uint8_t vol)
{
    mini_audio_player_set_volume(player,vol);
}


#endif


#endif  //YK_USE_UI_BUILDER