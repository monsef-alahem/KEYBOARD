/*************************************
 *  ARAB_TEXT_TO_UNICODE_CONVERTER
 *  AUTHOR  :   alahem monsef
 *  EMAIL   :   m.alahem09@gmail.com
 *  VERSION :   0.1
 *  DATE    :   10-05-2019
 *
 *************************************/

#include <iostream>
//#include <cstdlib>
#include <string>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

wchar_t keytoutf[121][2] =
{
    //dad
    {'a', 0xfebd},
    {'a', 0xfebe},
    {'a', 0xfebf},
    {'a', 0xfec0},
    //waw hamza
    {'c', 0xfe85},
    {'c', 0xfe86},
    //yaa
    {'d', 0xfef1},
    {'d', 0xfef2},
    {'d', 0xfef3},
    {'d', 0xfef4},
    //tha
    {'e', 0xfe99},
    {'e', 0xfe9a},
    {'e', 0xfe9b},
    {'e', 0xfe9c},
    //baa
    {'f', 0xfe8f},
    {'f', 0xfe90},
    {'f', 0xfe91},
    {'f', 0xfe92},
    //lam
    {'g', 0xfedd},
    {'g', 0xfede},
    {'g', 0xfedf},
    {'g', 0xfee0},
    //alif
    {'h', 0xfefb},
    {'h', 0xfefc},
    {'h', 0xfe8d},
    {'h', 0xfe8e},
    //haa
    {'i', 0xfee9},
    {'i', 0xfeea},
    {'i', 0xfeeb},
    {'i', 0xfeec},
    //taa
    {'j', 0xfe95},
    {'j', 0xfe96},
    {'j', 0xfe97},
    {'j', 0xfe98},
    //noon
    {'k', 0xfee5},
    {'k', 0xfee6},
    {'k', 0xfee7},
    {'k', 0xfee8},
    //mim
    {'l', 0xfee1},
    {'l', 0xfee2},
    {'l', 0xfee3},
    {'l', 0xfee4},
    //kaf
    {'m', 0xfed9},
    {'m', 0xfeda},
    {'m', 0xfedb},
    {'m', 0xfedc},
    //alif maqsoura
    {'n', 0xfeef},
    {'n', 0xfef0},
    //khaa
    {'o', 0xfea5},
    {'o', 0xfea6},
    {'o', 0xfea7},
    {'o', 0xfea8},
    //hhaa
    {'p', 0xfea1},
    {'p', 0xfea2},
    {'p', 0xfea3},
    {'p', 0xfea4},
    //shin
    {'q', 0xfeb5},
    {'q', 0xfeb6},
    {'q', 0xfeb7},
    {'q', 0xfeb8},
    //qaf
    {'r', 0xfed5},
    {'r', 0xfed6},
    {'r', 0xfed7},
    {'r', 0xfed8},
    //sin
    {'s', 0xfeb1},
    {'s', 0xfeb2},
    {'s', 0xfeb3},
    {'s', 0xfeb4},
    //faa
    {'t', 0xfed1},
    {'t', 0xfed2},
    {'t', 0xfed3},
    {'t', 0xfed4},
    //ain
    {'u', 0xfec9},
    {'u', 0xfeca},
    {'u', 0xfecb},
    {'u', 0xfecc},
    //raa
    {'v', 0xfead},
    {'v', 0xfeae},
    //alif maqsoura hamza
    {'w', 0xfe89},
    {'w', 0xfe8a},
    {'w', 0xfe8b},
    {'w', 0xfe8c},
    //hamza
    {'x', 0xfe80},
    //ghain
    {'y', 0xfecd},
    {'y', 0xfece},
    {'y', 0xfecf},
    {'y', 0xfed0},
    //sad
    {'z', 0xfeb9},
    {'z', 0xfeba},
    {'z', 0xfebb},
    {'z', 0xfebc},
    //space
    {' ', 0x20},
    //jim
    {'^', 0xfe9d},
    {'^', 0xfe9e},
    {'^', 0xfe9f},
    {'^', 0xfea0},
    //dal
    {'$', 0xfea9},
    {'$', 0xfeaa},
    //tah
    {'%', 0xfec1},
    {'%', 0xfec2},
    {'%', 0xfec3},
    {'%', 0xfec4},
    //dhal
    {'*', 0xfeab},
    {'*', 0xfeac},
    //taa marbouta
    {',', 0xfe93},
    {',', 0xfe94},
    //waw
    {';', 0xfeed},
    {';', 0xfeee},
    //thah
    {':', 0xfeb5},
    {':', 0xfeb6},
    {':', 0xfeb7},
    {':', 0xfeb8},
    //zayn
    {'!', 0xfeaf},
    {'!', 0xfeb0},
    //empty
    {' ', 0},
    {' ', 0x10},
    {' ', 0x13}
    //121

};

//global variables
typedef struct arab_line_t {
    unsigned char in_byte[10000];
    size_t bytes;
    uint32_t in_utf[10000];
    size_t utfs;
} arab_line_t;

typedef struct arab_line_light_t {
    unsigned char in_byte[100];
    size_t bytes;
    uint32_t in_utf[50];
    size_t utfs;
} arab_line_light_t;

//arab_line_t quran_data[114][286];

int aya = 0;
int soura = 0;


#include "utf8.h"
#include "arabtype.h"

using namespace std;

wchar_t get_key_from_utf(wchar_t wch)
{
    for (int i = 0 ; i < 121 ; i++) {
        if (keytoutf[i][1] == wch)
            return keytoutf[i][0];

    }
    return '?';
}


int reverse_line(arab_line_t *line)
{
    uint32_t revsed_line[sizeof(line->in_utf)];
    int i;
    int j = line->utfs -1;
    //copier la ligne a inverser
    for (i = 0 ; i < line->utfs ; i++) {
        revsed_line[j] = line->in_utf[i];
        j--;
    }
    for (i = 0 ; i < line->utfs ; i++) {
        //printf("rev : %x\n", revsed_line[i]);
        line->in_utf[i] = revsed_line[i]; 
    }
}

int correct(arab_line_t *line)
{
    for( int i = 0; i < line->utfs; i++ )
    {
        ///raaa
        if (line->in_utf[i] == 0xfeae)
            line->in_utf[i] = 0xfead;
        //dhaa
        if (line->in_utf[i] == 0xfec6)
            line->in_utf[i] = 0xfec5;
        ///baaa
        if (line->in_utf[i] == 0xfe90)
            line->in_utf[i] = 0xfe8f;
        ///zaaa
        if (line->in_utf[i] == 0xfeb0)
            line->in_utf[i] = 0xfeaf;
        ///fAA
        if (line->in_utf[i] == 0xfed2)
            line->in_utf[i] = 0xfed1;
        ///qaf
        if (line->in_utf[i] == 0xfed6)
            line->in_utf[i] = 0xfed5;
        ///lam
        if (line->in_utf[i] == 0xfeee)
            line->in_utf[i] = 0xfeed;
        ///lam AKHIR
        if (line->in_utf[i] == 0xfede)
            line->in_utf[i] = 0xfedd;
        ///noun
        if (line->in_utf[i] == 0xfee6)
            line->in_utf[i] = 0xfee5;
        ///waw
        if (line->in_utf[i] == 0xfeee)
            line->in_utf[i] = 0xfeed;
        ///dal
        if (line->in_utf[i] == 0xfeaa)
            line->in_utf[i] = 0xfea9;
        ///dhal
        if (line->in_utf[i] == 0xfeac)
            line->in_utf[i] = 0xfeab;
        ///sin
        if (line->in_utf[i] == 0xfeb2)
            line->in_utf[i] = 0xfeb1;
        ///shin
        if (line->in_utf[i] == 0xfeb6)
            line->in_utf[i] = 0xfeb5;
        ///kaf
        if (line->in_utf[i] == 0xfeda)
            line->in_utf[i] = 0xfed9;
        ///mim
        if (line->in_utf[i] == 0xfee2)
            line->in_utf[i] = 0xfee1;
        ///TA FLAWL
        if (line->in_utf[i] == 0xfec3)
            line->in_utf[i] = 0xfec1;
        ///TA
        if (line->in_utf[i] == 0xfec4)
            line->in_utf[i] = 0xfec1;
        ///SAD
        if (line->in_utf[i] == 0xfeba)
            line->in_utf[i] = 0xfeb9;
        ///TA2
        if (line->in_utf[i] == 0xfe96)
            line->in_utf[i] = 0xfe95;
        ///HAMZA WAW
        if (line->in_utf[i] == 0xfe86)
            line->in_utf[i] = 0xfe85;
        ///HAMZA dadh
        if (line->in_utf[i] == 0xfec7)
            line->in_utf[i] = 0xfec5;

    }
}



int is_text_match(uint32_t *text, uint32_t *text2, int size)
{
    for (int i = 0 ; i < size ; i++) {
        //printf("1 : %x\n", text[i]);
        //printf("2 : %x\n\n", text2[i]);
        if (text[i] != text2[i])
            return 0;
    }
    printf("found in soura %d aya %d\n", soura, aya);
    return 1;
}

int search_arab(arab_line_t *line, uint32_t *text, int txtsize)
{
    int j = txtsize;
    int k = line->utfs-1;
    for (int i = k-txtsize ; i > -1 ; i--) { 
        is_text_match(&line->in_utf[i], text, j);
    }
}