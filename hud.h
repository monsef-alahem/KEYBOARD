/*************************************
 *  KEYBOAR RACING
 *  AUTHOR  :   alahem monsef
 *  EMAIL   :   m.alahem09@gmail.com
 *  VERSION :   __
 *  DATE    :   24-avril-2019
 *
 *************************************/

//time
SDL_Rect time_bar = {15,320,610,10};
double time_checkpoint = 0;

//level indicator
TTF_Font* lvl_ind_font;
SDL_Surface* lvl_ind_sur;
SDL_Texture* lvl_ind_tex;
SDL_Rect lvl_ind_rec = {530,10,0,0};


//coins
SDL_Texture* coin_tex;
SDL_Surface* coin_sur;

//title
SDL_Texture* title_tex;
SDL_Surface* title_sur;

//message label
TTF_Font* msg_lbl_font;
SDL_Surface* msg_lbl_sur;
SDL_Texture* msg_lbl_tex;
SDL_Rect msg_lbl_rec = {13,270,0,0};


int init_hud()
{
    lvl_ind_font = TTF_OpenFont("arial.ttf", 25);
    msg_lbl_font = TTF_OpenFont("arial.ttf", 25);

    //coins
    coin_sur = IMG_Load("coin.png");
    Uint32 colorkey = SDL_MapRGB(coin_sur->format, 0, 255, 0);
    SDL_SetColorKey(coin_sur, SDL_TRUE, colorkey);
    coin_tex = SDL_CreateTextureFromSurface(renderer, coin_sur);

    //title
    title_sur = IMG_Load("title.png");
    title_tex = SDL_CreateTextureFromSurface(renderer, title_sur);

}

int update_hud()
{
    //level indicator
    int w,h;
    static char text[20]; 
    SDL_Color text_color = {25,0,255};
    sprintf(text, "level : %d", level_id);
    SDL_FreeSurface(lvl_ind_sur);
    lvl_ind_sur = TTF_RenderText_Solid(lvl_ind_font, text, text_color);
    SDL_DestroyTexture(lvl_ind_tex);
    lvl_ind_tex = SDL_CreateTextureFromSurface(renderer, lvl_ind_sur);
    SDL_QueryTexture(lvl_ind_tex, NULL, NULL, &w, &h);
    lvl_ind_rec.w = w;
    lvl_ind_rec.h = h;

    // message notification
    int i = wcslen(level_text[idx])-1;
    static wchar_t wtext[30];
    wchar_t chr = get_key_from_utf(level_text[idx][i-input_box.cursor_pos]);
    //std::cout << "char : " << std::hex << level_text[idx][i-input_box.cursor_pos] << std::endl;
    text_color = {255,0,0};

    swprintf(wtext, 30, L"%c : \ufed1\ufead\ufea4\ufedf\ufe8d \ufe90\ufe98\ufedb\ufe8d", chr);
    SDL_FreeSurface(msg_lbl_sur);
    msg_lbl_sur = TTF_RenderUNICODE_Solid(msg_lbl_font, (Uint16*)wtext, text_color);
    SDL_DestroyTexture(msg_lbl_tex);
    msg_lbl_tex = SDL_CreateTextureFromSurface(renderer, msg_lbl_sur);
    SDL_QueryTexture(msg_lbl_tex, NULL, NULL, &w, &h);
    msg_lbl_rec.w = w;
    msg_lbl_rec.h = h;

    //time
    if (SDL_GetTicks() - time_checkpoint > 100) {
        int x = (int)((level_id+1)*3/wcslen(level_text[idx]));
        if(x == 0)
            x = 1;
        time_bar.w-=x;
        time_checkpoint = SDL_GetTicks();
    }

    if (time_bar.w < 0) {
        answer_check();
        input_box_clean_lines();
/*        wave = Mix_LoadWAV("bad.mp3");
        Mix_PlayChannel(-1, wave, 0);
        coins_left--;
        time_bar.w = 610;
        time_checkpoint = SDL_GetTicks();*/
    }
}

int draw_hud()
{
    int w,h,i;
    SDL_Rect rect1, rect2;
    //time bar
    SDL_Color bar_col = {255,255,0,255};
    SDL_SetRenderDrawColor( renderer,bar_col.r,bar_col.g,bar_col.b,bar_col.a); 
    SDL_RenderFillRect( renderer, &time_bar);

    //level indicator text
    SDL_RenderCopy(renderer, lvl_ind_tex, NULL, &lvl_ind_rec);
    //message label text
    SDL_RenderCopy(renderer, msg_lbl_tex, NULL, &msg_lbl_rec);

    //coin
    SDL_QueryTexture(coin_tex, NULL, NULL, &w, &h);
    for (i=0 ; i<coins_left ; i++) {
        rect1 = (SDL_Rect){0,0,w,h};
        rect2 = (SDL_Rect){20+(i*25),13,w,h};
        SDL_RenderCopy(renderer, coin_tex, &rect1, &rect2);
    }

    //title
    static int x = 110;
    /*static char rev = 1;
    if (x == 255)
        rev = 0;
    if (x == 110)
        rev = 1;
    if (rev) {
        x++;
    } else {
        x--;
    }*/

    static double  phi = 0;
    phi += 0.6;
    x = (100 * sin((2 * 3.14 * 600)+ phi/5)) + 150;
    //printf("x = %d\n", x);
    change_pixel(title_sur,0,x,0,255);
    SDL_DestroyTexture(title_tex);
    title_tex = SDL_CreateTextureFromSurface(renderer, title_sur);
    SDL_QueryTexture(title_tex, NULL, NULL, &w, &h);
    rect1 = (SDL_Rect){0,0,w,h};
    rect2 = (SDL_Rect){250,0,w*3,h*3};
    SDL_RenderCopy(renderer, title_tex, &rect1, &rect2);

}
int free_hud()
{
    //level indicator
    SDL_DestroyTexture(lvl_ind_tex);
    SDL_FreeSurface(lvl_ind_sur);

    //message label 
    SDL_DestroyTexture(msg_lbl_tex);
    SDL_FreeSurface(msg_lbl_sur);
    
    //title
    SDL_DestroyTexture(title_tex);
    SDL_FreeSurface(title_sur);

    //coin
    SDL_DestroyTexture(coin_tex);
    SDL_FreeSurface(coin_sur);

    TTF_CloseFont(lvl_ind_font);
}