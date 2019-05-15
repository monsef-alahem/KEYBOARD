/*************************************
 *  KEYBOAR RACING
 *  AUTHOR  :   alahem monsef
 *  EMAIL   :   m.alahem09@gmail.com
 *  VERSION :   __
 *  DATE    :   24-avril-2019
 *
 *************************************/


//utiliser sdl2
#include <SDL2/SDL.h>

//ouvrir des images a part bmp avec sdl2
#include <SDL2/SDL_image.h>

//lire sons et musiques avec sdl2
#include <SDL2/SDL_mixer.h>

//afficher du text avec sdl2
#include <SDL2/SDL_ttf.h>

#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <cmath>

#include "baht_tools.h"

///fin des headers


//variables globales

int i = 0;
int ret = 0;
int coins_left = 7;
int idx = 0;
int level_id = 0;


//variables globales SDL2
SDL_Window* window;
SDL_Renderer* renderer;
Mix_Chunk *wave;
Mix_Music *music;
SDL_Event event;
#define KEYS 38
typedef struct key_t{
    char letter;
    unsigned char hexcode[2];
    SDL_Keycode code; 
}key_t;


//char level_text[1000][50];
wchar_t level_text[1000][50];

void load_data()
{
    FILE *data_file = fopen("data.txt", "r");

    char prev_chr;
    int line_idx,i,j;
    int chr_idx = 0;
    i = j = line_idx = 0;
    while(1)
    {
        level_text[j][i] = getc(data_file);
        if(level_text[j][i] == '\n' && prev_chr == '\n') {
            level_text[j][i] = getc(data_file);
        }
        if (i > 49) {
            printf("error line %d is too long\n", line_idx);
            goto error;
        }
        if (level_text[j][i] == EOF) {
            level_text[j][i] = 0;
            break;
        }

        prev_chr = level_text[j][i];

        if (level_text[j][i] == '\n') {
            level_text[j][i] = 0;
            line_idx += 1;
            j++;
            i = -1;
        }
        i++;
    }

    //rewind (data_file);
    //fseek (data_file , 0 , SEEK_SET);
    error:
    fclose(data_file);
}

void wstrcpy(uint32_t *src, wchar_t *dest, int size)
{
    int i;
    for (i = 0 ; i < size ; i++) {
        dest[i] = src[i];
    }
    dest[size] = 0;
}
int mywcscmp(uint32_t *t1, wchar_t *t2)
{
    int i;
    for (i = 0 ;  ; i++) {
        if(t1[i] != t2[i])
            return 1;
        else if (t1[i] == t2[i] && t1[i] == 0)
            break;
    }
    return 0;
}
int red_file_fill_level_text()
{
    std::ifstream ifs("data.txt");

    unsigned char ch;
    while (ifs.good()) {
        ch = ifs.get();
        printf("%x\n", ch);
    }
    ifs.close();
    wcscpy(level_text[0], L"\ufeeb\ufead");
}
int read_file_fill_level_text()
{
    std::ifstream ifs("data.txt");
    arab_line_t line;
    int i = 0;
    int j = 0;
    unsigned char ch;
    while (ch != 0xff) {
        memset(line.in_byte,0,50);
        wmemset((wchar_t*)line.in_utf,0,50);
        //lecture ligne
        while (ifs.good()) {
            ch = ifs.get();
            if (ifs.good()){
                line.in_byte[i] = ch;
                i++;
            }
            if(ch == '\n')
                break;
        }
        if(i < 2) {
            i = 0;
            continue;
        }
        if (i > 49) {
            printf("error line %d is too long\n", j);
            goto error;
        }
        line.bytes = i-1;
        line.in_utf[0] = 0;
        line.utfs = line.bytes*4;
        get_presentation_form_b(&line);
        reverse_line(&line);
        correct(&line);

/*        printf("%x\n", line.in_utf[0]);
        printf("%x\n", line.in_utf[1]);
        printf("%x\n", line.in_utf[2]);
        printf("%x\n", line.in_utf[3]);
        printf("%x\n", line.in_utf[4]);
        printf("%x\n", line.in_utf[5]);
        printf("%x\n", line.in_utf[6]);
        printf("%x\n", line.in_utf[7]);
        printf("%x\n", line.in_utf[8]);
        printf("%x\n", line.in_utf[9]);
        printf("%x\n", line.in_utf[10]);
        printf("%x\n", line.in_utf[11]);
        printf("%x\n", line.in_utf[12]);
        printf("%x\n", line.in_utf[13]);
        printf("%x\n\n", line.in_utf[14]);

        printf("%x\n", line.in_byte[0]);
        printf("%x\n", line.in_byte[1]);
        printf("%x\n", line.in_byte[2]);
        printf("%x\n", line.in_byte[3]);
        printf("%x\n", line.in_byte[4]);
        printf("%x\n", line.in_byte[5]);
        printf("%x\n", line.in_byte[6]);
        printf("%x\n", line.in_byte[7]);
        printf("%x\n", line.in_byte[8]);
        printf("%x\n", line.in_byte[9]);
        printf("%x\n", line.in_byte[10]);
        printf("%x\n", line.in_byte[11]);
        printf("%x\n\n", line.in_byte[12]);*/
        //level_text[0][0] = line.in_utf[0];

        wstrcpy(line.in_utf, level_text[j], line.utfs);
        //wstrcpy(L"\ufeeb\ufead", level_text[j], 2);
        //wcscpy(level_text[j], L"\ufeeb\ufeeb");
        j++;
        i=0;
    }
    error:
    ifs.close();
}


int answer_check();

#include "ui.h"

/*
d8a7 d884 alif lam
*/

key_t key_table[KEYS] = {
    {'a', {0xd8,0xb6}, SDLK_a},
    {'b', {0xd8,0xa7}, SDLK_b},
    {'c', {0xd8,0xa4}, SDLK_c},
    {'d', {0xd9,0x8a}, SDLK_d},
    {'e', {0xd8,0xab}, SDLK_e},
    {'f', {0xd8,0xa8}, SDLK_f},
    {'g', {0xd9,0x84}, SDLK_g},
    {'h', {0xd8,0xa7}, SDLK_h},
    {'i', {0xd9,0x87}, SDLK_i},
    {'j', {0xd8,0xaa}, SDLK_j},
    {'k', {0xd9,0x86}, SDLK_k},
    {'l', {0xd9,0x85}, SDLK_l},
    {'m', {0xd9,0x83}, SDLK_m},
    {'n', {0xd9,0x89}, SDLK_n},
    {'o', {0xd8,0xae}, SDLK_o},
    {'p', {0xd8,0xad}, SDLK_p},
    {'q', {0xd8,0xb4}, SDLK_q},
    {'r', {0xd9,0x82}, SDLK_r},
    {'s', {0xd8,0xb3}, SDLK_s},
    {'t', {0xd9,0x81}, SDLK_t},
    {'u', {0xd8,0xb9}, SDLK_u},
    {'v', {0xd8,0xb1}, SDLK_v},
    {'w', {0xd8,0xa6}, SDLK_w},
    {'x', {0xd8,0xa1}, SDLK_x},
    {'y', {0xd8,0xba}, SDLK_y},
    {'z', {0xd8,0xb5}, SDLK_z},
    {'_', {0xd8,0xb0}, SDLK_8},
    {'-', {0xd8,0x00}, SDLK_6},
    {'\'', {0xd8,0x00}, SDLK_4},
    {' ', {0x20}, SDLK_SPACE},
    {' ', {0xd8,0xac}, SDLK_CARET},//^
    {' ', {0xd8,0xaf}, SDLK_DOLLAR},
    {' ', {0xd8,0xb7}, 249},//%
    {' ', {0xd8,0xb0}, SDLK_ASTERISK},
    {' ', {0xd8,0xa9}, SDLK_COMMA},
    {' ', {0xd9,0x88}, SDLK_SEMICOLON},
    {' ', {0xd8,0xb8}, SDLK_COLON},
    {' ', {0xd8,0xb2}, SDLK_EXCLAIM}

};

int answer_check() {
    ret = mywcscmp(input_box.text[0].in_utf, quiz_box.text[0]);
        printf("ret = %d\n", ret);
/*        printf("%x\n", input_box.text[0].in_utf[0]);
        printf("%x\n", input_box.text[0].in_utf[1]);
        printf("%x\n", input_box.text[0].in_utf[2]);
        printf("%x\n", input_box.text[0].in_utf[3]);
        printf("%x\n", input_box.text[0].in_utf[4]);
        printf("%x\n", input_box.text[0].in_utf[5]);
        printf("%x\n", input_box.text[0].in_utf[6]);
        printf("%x\n", input_box.text[0].in_utf[7]);
        printf("%x\n", input_box.text[0].in_utf[8]);
        printf("%x\n", input_box.text[0].in_utf[9]);
        printf("%x\n", input_box.text[0].in_utf[10]);
        printf("%x\n", input_box.text[0].in_utf[11]);
        printf("%x\n", input_box.text[0].in_utf[12]);
        printf("%x\n", input_box.text[0].in_utf[13]);
        printf("%x\n\n", input_box.text[0].in_utf[14]);

        printf("%x\n", quiz_box.text[0][0]);
        printf("%x\n", quiz_box.text[0][1]);
        printf("%x\n", quiz_box.text[0][2]);
        printf("%x\n", quiz_box.text[0][3]);
        printf("%x\n", quiz_box.text[0][4]);
        printf("%x\n", quiz_box.text[0][5]);
        printf("%x\n", quiz_box.text[0][6]);
        printf("%x\n", quiz_box.text[0][7]);
        printf("%x\n", quiz_box.text[0][8]);
        printf("%x\n", quiz_box.text[0][9]);
        printf("%x\n", quiz_box.text[0][10]);
        printf("%x\n", quiz_box.text[0][11]);
        printf("%x\n", quiz_box.text[0][12]);
        printf("%x\n", quiz_box.text[0][13]);
        printf("%x\n\n", quiz_box.text[0][14]);*/
    if (ret == 0) {
        idx++;
        wave = Mix_LoadWAV("good.mp3");
        Mix_PlayChannel(-1, wave, 0);
        wcscpy(quiz_box.text[0], level_text[idx]);
        if (time_bar.w > 400){
            level_id += 5;
            wave = Mix_LoadWAV("level_up.mp3");
            Mix_PlayChannel(-1, wave, 0);
        }
        if (time_bar.w > 300){
            level_id += 1;
            wave = Mix_LoadWAV("level_up.mp3");
            Mix_PlayChannel(-1, wave, 0);
        }
        time_bar.w = 610;
        time_checkpoint = SDL_GetTicks();
    } else { 
        wave = Mix_LoadWAV("bad.mp3");
        Mix_PlayChannel(-1, wave, 0);
        coins_left--;
        time_bar.w = 610;
        time_checkpoint = SDL_GetTicks();
    }
    //text is nihayat al moustawa
    if (wcscmp(level_text[idx], L"\ufeef\ufeed\ufe98\ufeb4\ufee4\ufedf\ufe8d \ufe94\ufef3\ufe8e\ufeec\ufee7") == 0) {
        wave = Mix_LoadWAV("level_up.mp3");
        Mix_PlayChannel(-1, wave, 0);
        idx++;
        level_id++;
        wcscpy(quiz_box.text[0], level_text[idx]);
        SDL_Delay(300);
        time_bar.w = 610;
        time_checkpoint = SDL_GetTicks();
    }
    //if text is nihaya
    if (wcscmp(level_text[idx], L"\ufe94\ufef3\ufe8e\ufeec\ufee7") == 0) {
        return 0;
    }
    return 1;
}


int main(int argc, char**args)
{
    //load_data();
    read_file_fill_level_text();

    //demmarer sdl2
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);

    //initialiser l'affichage du texte et choix police
    TTF_Init();

    //initialiser audio
    Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 2048 );

    //contruire une fenêtre
    window = SDL_CreateWindow( "KEYBOARD RACING", 500, 300, 640, 480, 0 );
    Uint32 flag = 0;
    SDL_SetWindowFullscreen(window, flag);

    //création de l'ecran de dessin dans la fenêtre
    renderer = SDL_CreateRenderer( window, -1, 0 );
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );

    //ouvrir un fichier son ou musique
    //music = Mix_LoadMUS("under_moon.ogg");

    //jouer son et musique
    //Mix_PlayMusic( music, -1);
    //Mix_PlayChannel(-1, wave, 0);

    init_ui();
    

    int loop = 1;
    int i = 0;
    int ret = 0;

    while ( loop )
    {
        //efface l'ecran de dessin avec le noir (r,v,b,a) a = alpha la transparence
        SDL_SetRenderDrawColor( renderer,0,0,0,255);
        SDL_RenderClear( renderer );

        //entrée de clavier
        while ( SDL_PollEvent( &event ) ) {
            if ( event.type == SDL_QUIT) {                
                    loop = 0;
            } else if ( event.type == SDL_KEYDOWN ) {
                cout << event.key.keysym.sym << endl;
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    loop = 0;
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
/*                  static int once = 1;
                    if (!once)
                    {
                        input_box_clean_lines();
                        once = 1;
                    } else { */
                    loop = answer_check();
                    input_box_clean_lines();
                    //}
                }
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    step_back();
                    //input_box.text[input_box.line_pos][input_box.cursor_pos] = 0;
                }

                if (event.key.keysym.sym == SDLK_DELETE) {
                    input_box_clean_lines();
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
/*                    if (input_box.line_pos < 2) {        
                        input_box.cursor_pos = input_box.line_pos*50;
                        input_box.line_pos++;
                    }*/
                }

                for (i = 0 ; i < KEYS ; i++) {
                    if (event.key.keysym.sym == key_table[i].code) {
    /*                  static int once = 1;
                        if (!once) {
                            input_box_clean_lines();
                            once = 1;
                        }*/
                        input_box_append_letter(key_table[i].hexcode);
                        wave = Mix_LoadWAV("key.mp3");
                        Mix_PlayChannel(-1, wave, 0);

                    }
                }


            }
        }
        update_ui();
        if (coins_left == 0)
            loop = 0;

        draw_ui();

        //affiche l'ecran de dessin sur la fenetre
        SDL_RenderPresent(renderer);

        //pause x millisecondes avant de recommencer la boucle "while"
        SDL_Delay(32);
    }

    loop = 1;
    int w,h;
    char text[20];
    
    //update le level indicator
    SDL_Color text_color = {25,0,255};
    if (level_id < 10)
        sprintf(text, "ton level est %d, vise mieux", level_id);
    else if (level_id < 20)
        sprintf(text, "ton level est %d, tu peux faire mieux", level_id);
    else if (level_id < 30)
        sprintf(text, "ton level est %d, bien", level_id);
    else if (level_id < 40)
        sprintf(text, "ton level est %d, tu es rapide !", level_id);
    else if (level_id > 50)
        sprintf(text, "ton level est %d, tu es tres rapide !!!", level_id);

    lvl_ind_sur = TTF_RenderText_Solid(lvl_ind_font, text, text_color);
    lvl_ind_tex = SDL_CreateTextureFromSurface(renderer, lvl_ind_sur);
    SDL_QueryTexture(lvl_ind_tex, NULL, NULL, &w, &h);
    lvl_ind_rec.w = w;
    lvl_ind_rec.h = h;
    lvl_ind_rec = (SDL_Rect){(640/2)-(w/2),(480/2)-(h/2),w,h};


    while(loop) {
             //entrée de clavier
        while ( SDL_PollEvent( &event ) ) {
            if ( event.type == SDL_QUIT) {                
                    loop = 0;
            } else if ( event.type == SDL_KEYDOWN ) {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        loop = 0;
            }
        }
        SDL_SetRenderDrawColor( renderer,0,0,0,255);
        SDL_RenderClear( renderer );


        //dessiner le level indicator
        SDL_RenderCopy(renderer, lvl_ind_tex, NULL, &lvl_ind_rec);

        SDL_RenderPresent(renderer);
        SDL_Delay(32);
    }

    //quand on quitte le programme (la boucle while) on detruit ce qu'on a crée
    //pour éviter les fuites de mémoire sur l'ordinateur
    Mix_FreeChunk(wave);
    Mix_FreeMusic(music);
    free_ui();
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();

}