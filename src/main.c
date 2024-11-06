/*
Compilation et lancement du programme

gcc src/main.c -o bin/prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
bin/prog

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 750


            //Création d'une fenêtre window et d'un rendu renderer
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;



void init_tab(int tab[]){
    // args : tab(int *, 64) : tableau qu'il faut initialiser
    // 
    //  Initialise le tableau comme au début d'une partie d'echec
    //
    // return : void

    for (int i = 0; i < 8; i++)
        tab[i+8] = 1;

    tab[0] = 2;
    tab[1] = 3;
    tab[2] = 4;
    tab[3] = 6;
    tab[4] = 5;
    tab[5] = 4;
    tab[6] = 3;
    tab[7] = 2;

    for (int i = 0; i < 8; i++)
        tab[i+48] = -1;
        
    tab[56] = -2;
    tab[57] = -3;
    tab[58] = -4;
    tab[59] = -6;
    tab[60] = -5;
    tab[61] = -4;
    tab[62] = -3;
    tab[63] = -2;
}

void reset_tab(int tab[]){
    // args : tab(int *, 64) : tableau qu'il faut reset
    // 
    //  Reset tout le tableau à 0
    //
    // return : void

    for (int i = 0; i < 64; i++)
        tab[i] = 0;
}

void verif_tab(int possibility_tab[], int tab[], int click_mouse, int rocque, int pion_pass_take){
    // args :   possibility_tab(int *, 64) : tableau des possibilité de la piece sélectionné
    //          tab(int *, 64) : tableau du jeu d'echec actuel
    //          click_mouse(int) : case de la pièce selectionné
    //          rocque (int) : possibilités du rocque pour le tour actuel
    //          pion_pass_take (int) : case du dernier pion qui a avancé de 2 cases
    // 
    //  Détermine les possibilités du pion de la case "click_mouse", à partir du jeu sauvegardé dans "tab" et des valeurs de "rocque" et "pion_pass_take"
    //  Sauvegarde ces possibilités dans "possibility_tab"
    //
    // return : void

    int temp;
    reset_tab(possibility_tab);

    possibility_tab[click_mouse] = 3;
    switch (tab[click_mouse]){
        case 1: //Pion blanc        
            if (tab[click_mouse + 8] == 0 && click_mouse + 8 > 0)
                possibility_tab[click_mouse + 8] = 1;
            if (tab[click_mouse + 16] == 0 && click_mouse < 16 && click_mouse > 7 && tab[click_mouse+8] == 0)
                possibility_tab[click_mouse + 16] = 1;
            if (tab[click_mouse + 9] < 0 && click_mouse + 9 > 0 && (click_mouse+9)%8 != 0)
                possibility_tab[click_mouse + 9] = 2;
            if (tab[click_mouse + 7] < 0 && click_mouse + 7 > 0 && (click_mouse+7)%8 != 7)
                possibility_tab[click_mouse + 7] = 2;

            //  Prise en passant
            if (click_mouse - 1 == pion_pass_take){
                possibility_tab[click_mouse + 7] = 6;
                possibility_tab[click_mouse - 1] = 7;
            }
            if (click_mouse + 1 == pion_pass_take){
                possibility_tab[click_mouse + 9] = 6;
                possibility_tab[click_mouse + 1] = 7;
            }
            
            break;

        case 2: //Tour blanche 
            if (click_mouse % 8 > 0)    //Gauche
            {
                temp = click_mouse - 1;
                while (temp % 8 != 7 && temp >= 0)
                {
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] < 0 ){
                        possibility_tab[temp] = 2;
                        temp = 0;
                    }
                    if (tab[temp] > 0) {
                        possibility_tab[temp] = 0;
                        temp = 0;
                    }
                    
                    temp--;
                }
            }
            if (click_mouse % 8 < 7)    //Droite
            {    
                temp = click_mouse + 1;
                while (temp % 8 != 0)
                {
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 2;
                        temp = 7;
                    }
                    if (tab[temp] > 0) {
                        possibility_tab[temp] = 0;
                        temp = 7;
                    }
                    temp++;
                }
            }      
            if (click_mouse < 55)       //Bas
            {    
                temp = click_mouse + 8;
                while (temp < 64)
                {
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 2;
                        temp = 64;
                    }
                    if (tab[temp] > 0) {
                        possibility_tab[temp] = 0;
                        temp = 64;
                    }
                    temp += 8;
                }
            }  
            if (click_mouse > 7)        //Haut
            {    
                temp = click_mouse - 8;
                while (temp >= 0)
                {
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 2;
                        temp = 0;
                    }
                    if (tab[temp] > 0) {
                        possibility_tab[temp] = 0;
                        temp = 0;
                    }
                    temp -= 8;
                }
            }
            break;

        case 3: //Cavalier blanc
            if (click_mouse%8 > 1 && click_mouse > 7){
                if (tab[click_mouse - 10] == 0)
                    possibility_tab[click_mouse - 10] = 1;
                if (tab[click_mouse - 10] < 0)
                    possibility_tab[click_mouse - 10] = 2;
            }
            if (click_mouse%8 > 1 && click_mouse < 56){
                if (tab[click_mouse + 6] == 0)
                    possibility_tab[click_mouse + 6] = 1;
                if (tab[click_mouse +6] < 0)
                    possibility_tab[click_mouse + 6] = 2;
            }
            if (click_mouse%8 > 0 && click_mouse < 48){
                if (tab[click_mouse + 15] == 0)
                    possibility_tab[click_mouse + 15] = 1;
                if (tab[click_mouse + 15] < 0)
                    possibility_tab[click_mouse + 15] = 2;
            }
            if (click_mouse%8 < 7 && click_mouse < 48){
                if (tab[click_mouse + 17] == 0)
                    possibility_tab[click_mouse + 17] = 1;
                if (tab[click_mouse + 17] < 0)
                    possibility_tab[click_mouse + 17] = 2;
            }
            if (click_mouse%8 < 6 && click_mouse > 7){
                if (tab[click_mouse - 6] == 0)
                    possibility_tab[click_mouse - 6] = 1;
                if (tab[click_mouse - 6] < 0)
                    possibility_tab[click_mouse - 6] = 2;
            }
            if (click_mouse%8 < 6 && click_mouse < 56){
                if (tab[click_mouse + 10] == 0)
                    possibility_tab[click_mouse + 10] = 1;
                if (tab[click_mouse + 10] < 0)
                    possibility_tab[click_mouse + 10] = 2;
            }
            if (click_mouse%8 < 7 && click_mouse > 15){
                if (tab[click_mouse - 15] == 0)
                    possibility_tab[click_mouse - 15] = 1;
                if (tab[click_mouse - 15] < 0)
                    possibility_tab[click_mouse - 15] = 2;
            }
            if (click_mouse%8 > 0 && click_mouse > 15){
                if (tab[click_mouse - 17] == 0)
                    possibility_tab[click_mouse - 17] = 1;
                if (tab[click_mouse - 17] < 0)
                    possibility_tab[click_mouse - 17] = 2;
            }
            break;

        case 4: //Fou blanc         
            if (click_mouse%8 != 7 && click_mouse < 55){
                temp = click_mouse + 9;     //Diagonale bas droite
                while (temp < 64){
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 2;
                        temp = 65;
                    }
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 0;
                        temp = 65;
                    }
                    if (temp%8 == 7)
                        break;
                    temp += 9;
                }
            }
            if(click_mouse%8 != 0 && click_mouse < 55){
                temp = click_mouse + 7;     //Diagonale bas gauche
                while (temp < 64){
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 2;
                        temp = 65;
                    }
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 0;
                        temp = 65;
                    }
                    if (temp%8 == 0)
                        break;
                    temp += 7;
                }
            }
            if (click_mouse%8 != 0 && click_mouse > 7){
                temp = click_mouse - 9;     //Diagonale haut gauche
                while (temp >= 0){
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 2;
                        temp = 0;
                    }
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 0;
                        temp = 0;
                    }
                    if (temp%8 == 0)
                        break;
                    temp -= 9;
                }
            }
            if (click_mouse%8 != 7 && click_mouse > 7){
                temp = click_mouse - 7;     //Diagonale haut droit
                while (temp >= 0){
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 2;
                        temp = 0;
                    }
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 0;
                        temp = 0;
                    }
                    if (temp%8 == 7)
                        break;
                    temp -= 7;
                }
            }
            break;

        case 5: //Reine blanche:    
            if (click_mouse % 8 != 7 && click_mouse < 56){     //Diagonale bas droite
                temp = click_mouse + 9;
                while (temp < 64){
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 2;
                        temp = 65;
                    }
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 0;
                        temp = 65;
                    }
                    if (temp%8 == 7)
                        break;
                    temp += 9;
                }
            }
            if (click_mouse % 8 != 0 && click_mouse < 56){     //Diagonale bas gauche
                temp = click_mouse + 7;
                while (temp < 64){
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 2;
                        temp = 65;
                    }
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 0;
                        temp = 65;
                    }
                    if (temp%8 == 0)
                        break;
                    temp += 7;
                }
            }
            if (click_mouse % 8 != 0 && click_mouse > 7){     //Diagonale haut gauche
                temp = click_mouse - 9;
                while (temp % 8 != 7){
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 2;
                        temp = 16;
                    }
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 0;
                        temp = 16;
                    }
                    temp -= 9;
                }
            }
            if (click_mouse % 8 != 7 && click_mouse > 7){     //Diagonale haut droit
                temp = click_mouse - 7;
                while (temp % 8 != 0 && temp > 0){
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 2;
                        temp = 7;
                    }
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 0;
                        temp = 7;
                    }
                    temp -= 7;
                }
            }
            if (click_mouse % 8 != 0)    //Gauche
            {
                temp = click_mouse - 1;
                while (temp % 8 != 7 && temp >= 0)
                {
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] < 0 ){
                        possibility_tab[temp] = 2;
                        temp = 0;
                    }
                    if (tab[temp] > 0) {
                        possibility_tab[temp] = 0;
                        temp = 0;
                    }
                    
                    temp--;
                }
            }
            if (click_mouse % 8 != 7)    //Droite
            {    
                temp = click_mouse + 1;
                while (temp % 8 != 0)
                {
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 2;
                        temp = 7;
                    }
                    if (tab[temp] > 0) {
                        possibility_tab[temp] = 0;
                        temp = 7;
                    }
                    temp++;
                }
            }      
            if (click_mouse < 56)       //Bas
            {    
                temp = click_mouse + 8;
                while (temp < 64)
                {
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 2;
                        temp = 64;
                    }
                    if (tab[temp] > 0) {
                        possibility_tab[temp] = 0;
                        temp = 64;
                    }
                    temp += 8;
                }
            }  
            if (click_mouse > 7)        //Haut
            {    
                temp = click_mouse - 8;
                while (temp >= 0)
                {
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 2;
                        temp = 0;
                    }
                    if (tab[temp] > 0) {
                        possibility_tab[temp] = 0;
                        temp = 0;
                    }
                    temp -= 8;
                }
            }
            break;

        case 6: //Roi blanc
            if (click_mouse > 7){
                if (tab[click_mouse - 8] == 0)
                    possibility_tab[click_mouse - 8] = 1;
                if (tab[click_mouse - 8] < 0 && tab[click_mouse - 8] != -6)
                    possibility_tab[click_mouse - 8] = 2;
            }
            if (click_mouse < 55){
                if (tab[click_mouse + 8] == 0)
                    possibility_tab[click_mouse + 8] = 1;
                if (tab[click_mouse + 8] < 0)
                    possibility_tab[click_mouse + 8] = 2;
            }
            if (click_mouse % 8 > 0){
                if (tab[click_mouse - 1] == 0)
                    possibility_tab[click_mouse - 1] = 1;
                if (tab[click_mouse - 1] < 0 && tab[click_mouse - 1] != -6)
                    possibility_tab[click_mouse - 1] = 2;
            }
            if (click_mouse % 8 < 7){
                if (tab[click_mouse + 1] == 0)
                    possibility_tab[click_mouse + 1] = 1;
                if (tab[click_mouse + 1] < 0 && tab[click_mouse + 1] != -6)
                    possibility_tab[click_mouse + 1] = 2;
            }
            if (click_mouse % 8 != 0 && click_mouse < 56){
                if (tab[click_mouse + 9] == 0)
                    possibility_tab[click_mouse + 9] = 1;
                if (tab[click_mouse + 9] < 0 && tab[click_mouse + 9] != -6)
                    possibility_tab[click_mouse + 9] = 2;
            }
            if (click_mouse % 8 != 7 && click_mouse < 56){
                if (tab[click_mouse + 7] == 0)
                    possibility_tab[click_mouse + 7] = 1;
                if (tab[click_mouse + 7] < 0 && tab[click_mouse + 7] != -6)
                    possibility_tab[click_mouse + 7] = 2;
            }
            if (click_mouse % 8 != 0 && click_mouse > 7){
                if (tab[click_mouse - 9] == 0)
                    possibility_tab[click_mouse - 9] = 1;
                if (tab[click_mouse - 9] < 0 && tab[click_mouse - 9] != -6)
                    possibility_tab[click_mouse - 9] = 2;
            }
            if (click_mouse % 8 != 7 && click_mouse > 7){
                if (tab[click_mouse - 7] == 0)
                    possibility_tab[click_mouse - 7] = 1;
                if (tab[click_mouse - 7] < 0 && tab[click_mouse - 7] != -6)
                    possibility_tab[click_mouse - 7] = 2;
            }
            
            if ((rocque == 1 || rocque == 3) && tab[click_mouse-1] == 0 && tab[click_mouse-2] == 0 && tab[click_mouse-3] == 2)    //Gauche
                possibility_tab[click_mouse-2] = 4;
            if ((rocque == 2 || rocque == 3) && tab[click_mouse+1] == 0 && tab[click_mouse+2] == 0 && tab[click_mouse+3] == 0 && tab[click_mouse+4] == 2)    //Gauche
                possibility_tab[click_mouse+2] = 4;

            break;


        case -1:    //Pion noir     
            if (tab[click_mouse - 8] == 0 && click_mouse - 8 > 0)
                possibility_tab[click_mouse - 8] = 1;
            if (tab[click_mouse - 16] == 0 && click_mouse < 56 && click_mouse > 47 && tab[click_mouse - 8] == 0)
                possibility_tab[click_mouse - 16] = 1;
            if (tab[click_mouse - 9] > 0 && click_mouse - 9 > 0 && (click_mouse-9)%8 != 7)
                possibility_tab[click_mouse - 9] = 2;
            if (tab[click_mouse - 7] > 0 && click_mouse - 7 > 0 && (click_mouse-7)%8 != 0)
                possibility_tab[click_mouse - 7] = 2;
            if (click_mouse - 1 == pion_pass_take){
                possibility_tab[click_mouse - 9] = 6;
                possibility_tab[click_mouse - 1] = 7;
            }
            if (click_mouse + 1 == pion_pass_take){
                possibility_tab[click_mouse - 7] = 6;
                possibility_tab[click_mouse + 1] = 7;
            }
            break;
            
        case -2: //Tour noire       
            if (click_mouse % 8 > 0)
            {
                temp = click_mouse - 1;
                while (temp % 8 != 7 && temp >= 0)
                {
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] > 0 ){
                        possibility_tab[temp] = 2;
                        temp = 0;
                    }
                    if (tab[temp] < 0) {
                        possibility_tab[temp] = 0;
                        temp = 0;
                    }
                    temp--;
                }
            }       
            if (click_mouse % 8 < 7)
            {    
                temp = click_mouse + 1;
                while (temp % 8 != 0)
                {
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 2;
                        temp = 7;
                    }
                    if (tab[temp] < 0) {
                        possibility_tab[temp] = 0;
                        temp = 7;
                    }
                    temp++;
                }
            }       
            if (click_mouse < 55)
            {    
                temp = click_mouse + 8;
                while (temp < 64)
                {
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 2;
                        temp = 64;
                    }
                    if (tab[temp] < 0) {
                        possibility_tab[temp] = 0;
                        temp = 64;
                    }
                    temp += 8;
                }
            }
            if (click_mouse > 7)
            {    
                temp = click_mouse - 8;
                while (temp >= 0)
                {
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 2;
                        temp = 0;
                    }
                    if (tab[temp] < 0) {
                        possibility_tab[temp] = 0;
                        temp = 0;
                    }
                    temp -= 8;
                }
            }
            break;

        case -3: //Cavalier noir    
            if (click_mouse%8 > 1 && click_mouse > 7){  //Gauche-haut
                if (tab[click_mouse - 10] == 0)
                    possibility_tab[click_mouse - 10] = 1;
                if (tab[click_mouse - 10] > 0)
                    possibility_tab[click_mouse - 10] = 2;
            }
            if (click_mouse%8 > 1 && click_mouse < 56){ //Gauche-bas
                if (tab[click_mouse + 6] == 0)
                    possibility_tab[click_mouse + 6] = 1;
                if (tab[click_mouse +6] > 0)
                    possibility_tab[click_mouse + 6] = 2;
            }
            if (click_mouse%8 > 0 && click_mouse < 48){ //Bas-gauche
                if (tab[click_mouse + 15] == 0)
                    possibility_tab[click_mouse + 15] = 1;
                if (tab[click_mouse + 15] > 0)
                    possibility_tab[click_mouse + 15] = 2;
            }
            if (click_mouse%8 < 7 && click_mouse < 48){ //Bas-droite
                if (tab[click_mouse + 17] == 0)
                    possibility_tab[click_mouse + 17] = 1;
                if (tab[click_mouse + 17] > 0)
                    possibility_tab[click_mouse + 17] = 2;
            }
            if (click_mouse%8 < 6 && click_mouse > 7){  //Droite-haut
                if (tab[click_mouse - 6] == 0)
                    possibility_tab[click_mouse - 6] = 1;
                if (tab[click_mouse - 6] > 0)
                    possibility_tab[click_mouse - 6] = 2;
            }
            if (click_mouse%8 < 6 && click_mouse < 56){ //Droite-bas
                if (tab[click_mouse + 10] == 0)
                    possibility_tab[click_mouse + 10] = 1;
                if (tab[click_mouse + 10] > 0)
                    possibility_tab[click_mouse + 10] = 2;
            }
            if (click_mouse%8 < 7 && click_mouse > 15){ //Haut-droite
                if (tab[click_mouse - 15] == 0)
                    possibility_tab[click_mouse - 15] = 1;
                if (tab[click_mouse - 15] > 0)
                    possibility_tab[click_mouse - 15] = 2;
            }
            if (click_mouse%8 > 0 && click_mouse > 15){ //Haut-gauche
                if (tab[click_mouse - 17] == 0)
                    possibility_tab[click_mouse - 17] = 1;
                if (tab[click_mouse - 17] > 0)
                    possibility_tab[click_mouse - 17] = 2;
            }
            break;

        case -4: //Fou noir     
            if (click_mouse%8 != 7 && click_mouse < 55){
                temp = click_mouse + 9;     //Diagonale bas droite
                while (temp < 64){
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 2;
                        temp = 65;
                    }
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 0;
                        temp = 65;
                    }
                    if (temp%8 == 7)
                        break;
                    temp += 9;
                }
            }
            if(click_mouse%8 != 0 && click_mouse < 55){
                temp = click_mouse + 7;     //Diagonale bas gauche
                while (temp < 64){
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 2;
                        temp = 65;
                    }
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 0;
                        temp = 65;
                    }
                    if (temp%8 == 0)
                        break;
                    temp += 7;
                }
            }
            if (click_mouse%8 != 0 && click_mouse > 7){
                temp = click_mouse - 9;     //Diagonale haut gauche
                while (temp >= 0){
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 2;
                        temp = 0;
                    }
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 0;
                        temp = 0;
                    }
                    if (temp%8 == 0)
                        break;
                    temp -= 9;
                }
            }
            if (click_mouse%8 != 7 && click_mouse > 7){
                temp = click_mouse - 7;     //Diagonale haut droit
                while (temp >= 0){
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 2;
                        temp = 0;
                    }
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 0;
                        temp = 0;
                    }
                    if (temp%8 == 7)
                        break;
                    temp -= 7;
                }
            }
            break;
            
        case -5: //Reine noire:    
            if (click_mouse % 8 != 7 && click_mouse < 56){     //Diagonale bas droite
                temp = click_mouse + 9;
                while (temp < 64){
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 2;
                        temp = 65;
                    }
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 0;
                        temp = 65;
                    }
                    if (temp%8 == 7)
                        break;
                    temp += 9;
                }
            }
            if (click_mouse % 8 != 0 && click_mouse < 56){     //Diagonale bas gauche
                temp = click_mouse + 7;
                while (temp < 64){
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 2;
                        temp = 65;
                    }
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 0;
                        temp = 65;
                    }
                    if (temp%8 == 0)
                        break;
                    temp += 7;
                }
            }
            if (click_mouse % 8 != 0 && click_mouse > 7){     //Diagonale haut gauche
                temp = click_mouse - 9;
                while (temp % 8 != 7 && click_mouse > 7){
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 2;
                        temp = 16;
                    }
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 0;
                        temp = 16;
                    }
                    temp -= 9;
                }
            }
            if (click_mouse % 8 != 7 && click_mouse > 7){     //Diagonale haut droit
                temp = click_mouse - 7;
                while (temp % 8 != 0 && temp > 0){
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 2;
                        temp = 7;
                    }
                    if (tab[temp] < 0){
                        possibility_tab[temp] = 0;
                        temp = 7;
                    }
                    temp -= 7;
                }
            }
            if (click_mouse % 8 != 0)    //Gauche
            {
                temp = click_mouse - 1;
                while (temp % 8 != 7 && temp >= 0)
                {
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] > 0 ){
                        possibility_tab[temp] = 2;
                        temp = 0;
                    }
                    if (tab[temp] < 0) {
                        possibility_tab[temp] = 0;
                        temp = 0;
                    }
                    
                    temp--;
                }
            }
            if (click_mouse % 8 != 7)    //Droite
            {    
                temp = click_mouse + 1;
                while (temp % 8 != 0)
                {
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 2;
                        temp = 7;
                    }
                    if (tab[temp] < 0) {
                        possibility_tab[temp] = 0;
                        temp = 7;
                    }
                    temp++;
                }
            }      
            if (click_mouse < 56)       //Bas
            {    
                temp = click_mouse + 8;
                while (temp < 64)
                {
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 2;
                        temp = 64;
                    }
                    if (tab[temp] < 0) {
                        possibility_tab[temp] = 0;
                        temp = 64;
                    }
                    temp += 8;
                }
            }  
            if (click_mouse > 7)        //Haut
            {    
                temp = click_mouse - 8;
                while (temp >= 0)
                {
                    if (tab[temp] == 0)
                        possibility_tab[temp] = 1;
                    if (tab[temp] > 0){
                        possibility_tab[temp] = 2;
                        temp = 0;
                    }
                    if (tab[temp] < 0) {
                        possibility_tab[temp] = 0;
                        temp = 0;
                    }
                    temp -= 8;
                }
            }
            break;

        case -6: //Roi noir
            if (click_mouse > 7){
                if (tab[click_mouse - 8] == 0)
                    possibility_tab[click_mouse - 8] = 1;
                if (tab[click_mouse - 8] > 0 && tab[click_mouse - 8] != 6)
                    possibility_tab[click_mouse - 8] = 2;
            }
            if (click_mouse < 55){
                if (tab[click_mouse + 8] == 0)
                    possibility_tab[click_mouse + 8] = 1;
                if (tab[click_mouse + 8] > 0 && tab[click_mouse + 8] != 6)
                    possibility_tab[click_mouse + 8] = 2;
            }
            if (click_mouse % 8 > 0){
                if (tab[click_mouse - 1] == 0)
                    possibility_tab[click_mouse - 1] = 1;
                if (tab[click_mouse - 1] > 0 && tab[click_mouse - 1] != 6)
                    possibility_tab[click_mouse - 1] = 2;
            }
            if (click_mouse % 8 < 7){
                if (tab[click_mouse + 1] == 0)
                    possibility_tab[click_mouse + 1] = 1;
                if (tab[click_mouse + 1] > 0 && tab[click_mouse + 1] != 6)
                    possibility_tab[click_mouse + 1] = 2;
            }
            if (click_mouse % 8 != 0 && click_mouse < 56){
                if (tab[click_mouse + 9] == 0)
                    possibility_tab[click_mouse + 9] = 1;
                if (tab[click_mouse + 9] > 0 && tab[click_mouse + 9] != 6)
                    possibility_tab[click_mouse + 9] = 2;
            }
            if (click_mouse % 8 != 7 && click_mouse < 56){
                if (tab[click_mouse + 7] == 0)
                    possibility_tab[click_mouse + 7] = 1;
                if (tab[click_mouse + 7] > 0 && tab[click_mouse + 7] != 6)
                    possibility_tab[click_mouse + 7] = 2;
            }
            if (click_mouse % 8 != 0 && click_mouse > 7){
                if (tab[click_mouse - 9] == 0)
                    possibility_tab[click_mouse - 9] = 1;
                if (tab[click_mouse - 9] > 0 && tab[click_mouse - 9] != 6)
                    possibility_tab[click_mouse - 9] = 2;
            }
            if (click_mouse % 8 != 7 && click_mouse > 7){
                if (tab[click_mouse - 7] == 0)
                    possibility_tab[click_mouse - 7] = 1;
                if (tab[click_mouse - 7] > 0 && tab[click_mouse - 7] != 6)
                    possibility_tab[click_mouse - 7] = 2;
            }
            if ((rocque == 1 || rocque == 3) && tab[click_mouse-1] == 0 && tab[click_mouse-2] == 0 && tab[click_mouse-3] == -2)    //Gauche
            {
                possibility_tab[click_mouse-2] = 4;
                possibility_tab[click_mouse-3] = 5;
            }
            if ((rocque == 2 || rocque == 3) && tab[click_mouse+1] == 0 && tab[click_mouse+2] == 0 && tab[click_mouse+3] == 0 && tab[click_mouse+4] == -2)    //Gauche
            {
                possibility_tab[click_mouse+2] = 4;
                possibility_tab[click_mouse+3] = 5;
            }
            break;
    }
}

int countLetters(char word[])
{
    // args : word(char *) : mot dont il faut compter les lettres
    // 
    //  Compte le nombre de lettres du mot
    //
    // return : i (int) : nombre de lettres du mot

    int i = 0;                //On met i à 0
    while (word[i] != '\0')   //Tant que le mot/phrase n'est pas fini
        i++;                  //on incrémente i de 1
    return i;                 //On retourne i
}

void save(int tab[], int turn, int rocque_white, int rocque_black){
    // args :   tab(int *, 64) : tableau du jeu actuel
    //          turn (int) : tour actuel
    // 
    //  Sauvegarde le tableau et le tour dans un fichier .txt
    //
    // return : void
    FILE* save = NULL;

    save = fopen("save.txt", "w+");
    for (int i = 0; i < 64; i++)
    {
        switch (tab[i])
        {
            case 0:
                fprintf(save, "00");
                break;

            case 1:
                fprintf(save, "01");
                break;
            case 2:
                fprintf(save, "02");
                break;
            case 3:
                fprintf(save, "03");
                break;
            case 4:
                fprintf(save, "04");
                break;
            case 5:
                fprintf(save, "05");
                break;
            case 6:
                fprintf(save, "06");
                break;


            case -1:
                fprintf(save, "11");
                break;
            case -2:
                fprintf(save, "12");
                break;
            case -3:
                fprintf(save, "13");
                break;
            case -4:
                fprintf(save, "14");
                break;
            case -5:
                fprintf(save, "15");
                break;
            case -6:
                fprintf(save, "16");
                break;
            
            default:
                break;
        }
    }
    if (turn == 1)
        fprintf(save, "00");
    if (turn == -1)
        fprintf(save, "01");
    switch (rocque_white)
    {
    case 0:
        fprintf(save, "0");
        break;
    case 1:
        fprintf(save, "1");
        break;
    case 2:
        fprintf(save, "2");
        break;
    case 3:
        fprintf(save, "3");
        break;
    
    default:
        break;
    }
    switch (rocque_black)
    {
    case 0:
        fprintf(save, "0");
        break;
    case 1:
        fprintf(save, "1");
        break;
    case 2:
        fprintf(save, "2");
        break;
    case 3:
        fprintf(save, "3");
        break;
    
    default:
        break;
    }
    
    printf("Save done.\n");
    fclose(save);
}

int loading(int tab[], int *rocque_white, int *rocque_black){
    // args :   tab(int *, 64) : tableau du jeu sauvegardé
    //          turn (int) : tour sauvegardé
    // 
    //  Charge le tableau et le tour du fichier .txt dans les variables du jeu
    //
    // return : turn (int) : tour sauvegardé 
    FILE* load = NULL;
    int temp, turn;
    char temp_c[10];

    load = fopen("save.txt", "r");

    for (int i = 0; i < 64; i++)
    {
        fscanf(load, "%2i", &temp);
        switch (temp)
        {
            case 0:
                tab[i] = 0;
                break;

            case 1:
                tab[i] = 1;
                break;
            case 2:
                tab[i] = 2;
                break;
            case 3:
                tab[i] = 3;
                break;
            case 4:
                tab[i] = 4;
                break;
            case 5:
                tab[i] = 5;
                break;
            case 6:
                tab[i] = 6;
                break;
            case 11:
                tab[i] = -1;
                break;
            case 12:
                tab[i] = -2;
                break;
            case 13:
                tab[i] = -3;
                break;
            case 14:
                tab[i] = -4;
                break;
            case 15:
                tab[i] = -5;
                break;
            case 16:
                tab[i] = -6;
                break;
            
            default:
                break;
        }
    }
    fgets(temp_c, 1, load);
    printf("%1s", temp_c);
    switch (temp_c[0])
    {
        case 0:
            turn = 1;
            break;
        case 1:
            turn = -1;
            break;
        
        default:
            break;
    }

    printf("Load done.\n");
    fclose(load);

    return turn;
}

int is_check(int tab[], int king_place){
    int check = 0;
    int possibility_tab[64];
    int i = 0;
    
    while(i < 64)
    {
        verif_tab(possibility_tab, tab, i, 3, -2);
        if(possibility_tab[king_place] == 2)
            check++;
        i++;
    }

    return check;
}

int is_checkmate(int tab[], int king_place, int turn, int rocque){
    // args : tab(int *, 64) : tableau du jeu actuel
    // 
    //  Cette fonction initialise le tableau comme au début d'une partie d'echec
    //
    // return : tab(int *) : tableau initialisé

    int check = is_check(tab, king_place), temp_check = 0;
    int possibility_tab[64];
    int checkmate = 0, temp;
    int i = 0, j = 0;

    if (check > 0){
        // Parcours du tableau pour trouver une pièce du bon tour
        while(i < 64)
        {
            j = 0;
            
            // Est-ce le bon tour ?
            if (tab[i]/abs(tab[i]) == turn)
            {
                // Si oui on regarde les coups possible de la pièce
                verif_tab(possibility_tab, tab, i, rocque, -2);

                while (j < 64)
                {
                    if(possibility_tab[j] != 0 && possibility_tab[j] != 3 && abs(tab[i]) != 6){
                        temp = tab[j];
                        tab[j] = tab[i];
                        tab[i] = 0;

                        temp_check = is_check(tab, king_place);
                        
                        tab[i] = tab[j];
                        tab[j] = temp;

                        if (temp_check == 0)
                            checkmate ++;
                    }
                    if(possibility_tab[j] != 0 && possibility_tab[j] != 3 && abs(tab[i]) == 6){
                        temp = tab[j];
                        tab[j] = tab[i];
                        tab[i] = 0;


                        temp_check = is_check(tab, j);
                        
                        tab[i] = tab[j];
                        tab[j] = temp;

                        if (temp_check == 0)
                            checkmate ++;
                    }
                    j++;
                }
            }
            i++;
        }
    }

    if (checkmate == 0 && check > 0)
        return 2;
    if (check > 0)
        return 1;
    if (check == 0)
        return 0;
}

int calcul_value(int tab[], int place){
    switch (tab[place])
    {
    case 0:
        return 0;
        break;

    case 1:
        return 10;
        break;
    case 2:
        return 50;
        break;
    case 3:
        return 30;
        break;
    case 4:
        return 30;
        break;
    case 5:
        return 90;
        break;
    case 6:
        return 900;
        break;

    case -1:
        return -10;
        break;
    case -2:
        return -50;
        break;
    case -3:
        return -30;
        break;
    case -4:
        return -30;
        break;
    case -5:
        return -90;
        break;
    case -6:
        return -900;
        break;
    
    default:
        return 0;
        break;
    }
}

int calcul_move(int tab[], int place, int rocque, int move[2], int depth, int value, int king_place, int pion_pass_take){
    int possibility_tab[64];
    int temp_tab[64];
    int temp_value = 0, temp_value_responce = 0, nbr_move = 0;
    int temp_move[2];
    int depth_value;
    memcpy(temp_tab, tab, sizeof(int*) * 64);

    //Partie coup :
    verif_tab(possibility_tab, tab, place, rocque, pion_pass_take);
    for (int i = 0; i < 64; i++)
    {
        temp_value_responce = 0;
        if(possibility_tab[i] != 0 && possibility_tab[i] != 3 && i != place){
            temp_value = calcul_value(tab, i);
            
            temp_tab[i] = temp_tab[place];
            temp_tab[place] = 0;

            if (is_check(temp_tab, king_place) > 0)
                temp_value = -9000;
            
            //Partie réponse :      
            for (int j = 0; j < 64; j++)
            {
                verif_tab(possibility_tab, temp_tab, j, rocque, pion_pass_take);
                if (temp_tab[j] >= 0)
                {       
                    for (int h = 0; h < 64; h++)
                    {                            
                        if(possibility_tab[h] != 0 && possibility_tab[h] != 3){
                            temp_value_responce += calcul_value(temp_tab, h);

                            if (depth > 0)
                            {
                                temp_move[0] = place;
                                temp_move[1] = i;
                                depth_value = 0;
                                for (int k = 0; k < 64; k++)
                                {
                                    if (tab[k] < 0)
                                        depth_value += calcul_move(tab, i, rocque, temp_move, depth-1, value, king_place, pion_pass_take);
                                }
                            }
                        }
                    }
                }
            }
            //printf("Value responce : %i, pour aller de %i (%i) a %i (%i)\n", temp_value_responce + temp_value, place, tab[place], i, tab[i]);
            if ((temp_value + temp_value_responce) > value)
            {
                value = temp_value + temp_value_responce;
                move[0] = place;
                move[1] = i;
            }
        }
    }
    return value;
}

int IA(int tab[], int rocque, int *turn, int *win, int king_place, int pion_pass_take){
    int best_move[2];
    int value = -999999;

    for (int i = 0; i < 64; i++)
    {
        if (tab[i] < 0)
            value = calcul_move(tab, i, rocque, best_move, 0, value, king_place, pion_pass_take);
    }
    printf("  Tour %i, parti de %i (piece %i) pour aller sur %i (piece %i), Valeur : %i\n", *turn, best_move[0], tab[best_move[0]], best_move[1], tab[best_move[1]], value);

    if (tab[best_move[1]] == 6)
        *win = -1;
    if (tab[best_move[1]] == -6)
        *win = 1;
    
    tab[best_move[1]] = tab[best_move[0]];
    tab[best_move[0]] = 0;

    *turn *= -1;
}

int apply_moves(int tab[], int click_mouse, int last_click_mouse, int *win, SDL_bool *game, int *turn, int *rocque_white, int *rocque_black, int *is_promote, int *case_white_king, int *case_black_king, int *pion_pass_take){
    if (tab[click_mouse] == 6){
        *win = -1;
        *game = SDL_FALSE;
    }
    if (tab[click_mouse] == -6){
        *win = 1;
        *game = SDL_FALSE;
    }
    if (tab[last_click_mouse] == 2 && click_mouse%8 == 0){
        if (*turn == 1)
            *rocque_white = 2;
        if (*turn == -1)
            *rocque_black = 2;
    }
    if (tab[last_click_mouse] == 2 && click_mouse%8 == 7){
        if (*turn == 1)
            *rocque_white = 1;
        if (*turn == -1)
            *rocque_black = 1;
    }
    if (tab[last_click_mouse] == 6){
        *rocque_white = 0;
        *case_white_king = last_click_mouse;
    }
    if (tab[last_click_mouse] == -6){
        *rocque_black = 0;
        *case_black_king = last_click_mouse;
    }
    if (tab[last_click_mouse] == 1 && click_mouse > 54)
        *is_promote = 1;
    if (tab[last_click_mouse] == -1 && click_mouse < 8)
        *is_promote = 1;

    if (abs(click_mouse - last_click_mouse) >= 14 && abs(tab[last_click_mouse]) == 1)
        *pion_pass_take = click_mouse;
    else 
        *pion_pass_take = -2;

    tab[click_mouse] = tab[last_click_mouse];
    tab[last_click_mouse] = 0;
    if (*is_promote == 0)
        *turn *= -1;
}

int main(int argc, char *argv[])
{
    int *tab, *possibility_tab;
    tab = calloc(64, sizeof(int));           //On alloue la mémoire du tableau de jeu selon la dimension choisie
    init_tab(tab);

    SDL_Rect rectangle;                     //Variable rectangle qu'on utilise dans tout le programme pour afficher les textures à des points précis

    int rocque_white = 3, rocque_black = 3;

    possibility_tab = calloc(64, sizeof(int));           //On alloue la mémoire du tableau de jeu selon la dimension choisie

    int turn = 1, click_mouse = -1, last_click_mouse = -1;
    int case_white_king = 3, case_black_king = 59;
    int check_white = 0, check_black = 0;
    int pion_pass_take = -2;

    SDL_Surface *texte = NULL;              //Variable texte qu'on utilise dans tout le programme 
    char texte_c[50];

    int win = 0;
    int game_mode = 1;
    int can_move = 1;
    int is_promote = 0;
    int load = -1;
    int rules = 0;
    
    SDL_Event event;
    SDL_Texture *text_texte = NULL;

    SDL_bool welcome_launch = SDL_TRUE;
    SDL_bool game = SDL_TRUE;
    SDL_bool game_over = SDL_TRUE;
/*------------------------------------------------------------*/

            //Lancement SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
        goto endProg;

            //Création fenêtre + rendu 
    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) != 0 )
        goto endProg;

            //Lancement SDl_TTF :
    if(TTF_Init() == -1)
        goto endProg;

    TTF_Font *police = NULL;                    //Variable police du jeu
    police = TTF_OpenFont("src/police.ttf", 40);//On sauvegarde l'emplacement de la police dans la variable
    SDL_Color noir = {0,0,0};                   //Définition de la couleur du texte, ici noir

    SDL_SetWindowTitle(window, "Jeu d'Echecs");                //On détermine le nom de la fenêtre

/*------------------------------------------------------------------------------------------------------------------------*/ 

/*-------------------------------*/             //Chargement de la texture "welcomeText"
    SDL_Surface *welcome = NULL;               
    welcome = IMG_Load("src/chess_welcome.png");
    if(welcome == NULL)
        goto endProg;
    SDL_Texture *welcomeText = NULL;
    welcomeText = SDL_CreateTextureFromSurface(renderer, welcome);
    if(welcomeText == NULL)
        goto endProg;

    SDL_FreeSurface(welcome);
/*-------------------------------*/             //Chargement de la texture "puceText"
    SDL_Surface *puce = NULL;               
    puce = IMG_Load("src/puce.png");
    if(puce == NULL)
        goto endProg;
    SDL_Texture *puceText = NULL;
    puceText = SDL_CreateTextureFromSurface(renderer, puce);
    if(puceText == NULL)
        goto endProg;

    SDL_FreeSurface(puce);

    while(welcome_launch)
    {
        SDL_Delay(100);
                            //On défini un rectangle où l'on placera la texture "welcomeText"
        rectangle.w = WINDOW_WIDTH;                              
        rectangle.h = WINDOW_HEIGHT;
        rectangle.x = 0;
        rectangle.y = 0;

                            //On affiche la texture "welcomeText"
        if(SDL_RenderCopy(renderer, welcomeText, NULL, &rectangle))
            goto endProg; 

        switch (game_mode)
        {
            case 1:
                                            //On défini un rectangle où l'on placera la texture
                rectangle.w = 30;                      
                rectangle.h = 30;
                rectangle.x = 105;
                rectangle.y = 155;

                if(SDL_RenderCopy(renderer, puceText, NULL, &rectangle))         //On place la texture
                    goto endProg; 
                break;

            case 2:
                                            //On défini un rectangle où l'on placera la texture
                rectangle.w = 30;                      
                rectangle.h = 30;
                rectangle.x = 105;
                rectangle.y = 255;

                if(SDL_RenderCopy(renderer, puceText, NULL, &rectangle))         //On place la texture
                    goto endProg; 
                break;
            
            case 3:
                                            //On défini un rectangle où l'on placera la texture
                rectangle.w = 30;                      
                rectangle.h = 30;
                rectangle.x = 105;
                rectangle.y = 355;

                if(SDL_RenderCopy(renderer, puceText, NULL, &rectangle))         //On place la texture
                    goto endProg; 
                break;
            
            default:
                break;
        }

        if (load == 1)
        {
                //On défini un rectangle où l'on placera la texture
            rectangle.w = 30;                      
            rectangle.h = 30;
            rectangle.x = 545;
            rectangle.y = 155;

            if(SDL_RenderCopy(renderer, puceText, NULL, &rectangle))         //On place la texture
                goto endProg;
        }
        

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT :                     //Dans le cas d'une fermeture de la fenêtre
                    welcome_launch = SDL_FALSE;
                    game = SDL_FALSE;
                    game_over = SDL_FALSE;
                    break;
                
                case SDL_MOUSEBUTTONDOWN :
                    if (event.motion.x >= 100 && event.motion.x <= 140 && event.motion.y >= 150 && event.motion.y <= 190 && rules != 1)
                        game_mode = 1;
                    if (event.motion.x >= 100 && event.motion.x <= 140 && event.motion.y >= 250 && event.motion.y <= 290 && rules != 1)
                        printf("WIP... (network)\n");
                    if (event.motion.x >= 100 && event.motion.x <= 140 && event.motion.y >= 350 && event.motion.y <= 390 && rules != 1)
                        game_mode = 3;
                    if (event.motion.x >= 100 && event.motion.x <= 700 && event.motion.y >= 625 && event.motion.y <= 725 && rules != 1)
                        welcome_launch = SDL_FALSE;
                    if (event.motion.x >= 335 && event.motion.x <= 465 && event.motion.y >= 500 && event.motion.y <= 545 && rules != 1)
                        printf("WIP... (rules)\n");
                    if (event.motion.x >= 540 && event.motion.x <= 580 && event.motion.y >= 150 && event.motion.y <= 190 && rules != 1)
                        load *= -1;
                    break;

                default :
                    break;
            }
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(welcomeText);
    SDL_DestroyTexture(puceText);



/*------------------------------------------------------------------------------------------------------------------------*/ 
/*-------------------------------*/             //Chargement de la texture "board"
    SDL_Surface *board = NULL;                                                      //On initialise une nouvelle surface appelé "board"
    board = IMG_Load("src/chess.jpg");                                             //On charge l'image "Regles" dans la nouvelle surface
    if(board == NULL)                                                              //On vérifie si la texture a chargée
        goto endProg;   //sinon on quitte le programme
        
    SDL_Texture *boardText = NULL;                                                  //On initialise une nouvelle texture appelé "boardText"
    boardText = SDL_CreateTextureFromSurface(renderer, board);                      //On transforme la surface "board" en une texture sauvegardée dans "boardText"
    if(boardText == NULL)                                                           //On vérifie si la texture a chargée
        goto endProg;   //sinon on quitte le programme
    SDL_FreeSurface(board);

/*-------------------------------*/             //Chargement de la texture "white_pin"
    SDL_Surface *white_pin = NULL;
    white_pin = IMG_Load("src/pion_blanc.png");
    if(white_pin == NULL)
        goto endProg;
    SDL_Texture *white_pinText = NULL;
    white_pinText = SDL_CreateTextureFromSurface(renderer, white_pin);
    if(white_pinText == NULL)
        goto endProg;
    SDL_FreeSurface(white_pin);

/*-------------------------------*/             //Chargement de la texture "black_pin"
    SDL_Surface *black_pin = NULL;
    black_pin = IMG_Load("src/pion_noir.png");
    if(black_pin == NULL)
        goto endProg;
    SDL_Texture *black_pinText = NULL;
    black_pinText = SDL_CreateTextureFromSurface(renderer, black_pin);
    if(black_pinText == NULL)
        goto endProg;
    SDL_FreeSurface(black_pin);

/*-------------------------------*/             //Chargement de la texture "white_horse"
    SDL_Surface *white_horse = NULL;
    white_horse = IMG_Load("src/cavalier_blanc.png");
    if(white_horse == NULL)
        goto endProg;
    SDL_Texture *white_horseText = NULL;
    white_horseText = SDL_CreateTextureFromSurface(renderer, white_horse);
    if(white_horseText == NULL)
        goto endProg;
    SDL_FreeSurface(white_horse);


/*-------------------------------*/             //Chargement de la texture "black_horse"
    SDL_Surface *black_horse = NULL;
    black_horse = IMG_Load("src/cavalier_noir.png");
    if(black_horse == NULL)
        goto endProg;
    SDL_Texture *black_horseText = NULL;
    black_horseText = SDL_CreateTextureFromSurface(renderer, black_horse);
    if(black_horseText == NULL)
        goto endProg;
    SDL_FreeSurface(black_horse);

/*-------------------------------*/             //Chargement de la texture "white_mad"
    SDL_Surface *white_mad = NULL;
    white_mad = IMG_Load("src/fou_blanc.png");
    if(white_mad == NULL)
        goto endProg;
    SDL_Texture *white_madText = NULL;
    white_madText = SDL_CreateTextureFromSurface(renderer, white_mad);
    if(white_madText == NULL)
        goto endProg;
    SDL_FreeSurface(white_mad);


/*-------------------------------*/             //Chargement de la texture "black_mad"
    SDL_Surface *black_mad = NULL;
    black_mad = IMG_Load("src/fou_noir.png");
    if(black_mad == NULL)
        goto endProg;
    SDL_Texture *black_madText = NULL;
    black_madText = SDL_CreateTextureFromSurface(renderer, black_mad);
    if(black_madText == NULL)
        goto endProg;
    SDL_FreeSurface(black_mad);

/*-------------------------------*/             //Chargement de la texture "white_tower"
    SDL_Surface *white_tower = NULL;
    white_tower = IMG_Load("src/tour_blanc.png");
    if(white_tower == NULL)
        goto endProg;
    SDL_Texture *white_towerText = NULL;
    white_towerText = SDL_CreateTextureFromSurface(renderer, white_tower);
    if(white_towerText == NULL)
        goto endProg;
    SDL_FreeSurface(white_tower);


/*-------------------------------*/             //Chargement de la texture "black_tower"
    SDL_Surface *black_tower = NULL;
    black_tower = IMG_Load("src/tour_noir.png");
    if(black_tower == NULL)
        goto endProg;
    SDL_Texture *black_towerText = NULL;
    black_towerText = SDL_CreateTextureFromSurface(renderer, black_tower);
    if(black_towerText == NULL)
        goto endProg;
    SDL_FreeSurface(black_tower);

/*-------------------------------*/             //Chargement de la texture "white_queen"
    SDL_Surface *white_queen = NULL;
    white_queen = IMG_Load("src/dame_blanc.png");
    if(white_queen == NULL)
        goto endProg;
    SDL_Texture *white_queenText = NULL;
    white_queenText = SDL_CreateTextureFromSurface(renderer, white_queen);
    if(white_queenText == NULL)
        goto endProg;
    SDL_FreeSurface(white_queen);


/*-------------------------------*/             //Chargement de la texture "black_queen"
    SDL_Surface *black_queen = NULL;
    black_queen = IMG_Load("src/dame_noir.png");
    if(black_queen == NULL)
        goto endProg;
    SDL_Texture *black_queenText = NULL;
    black_queenText = SDL_CreateTextureFromSurface(renderer, black_queen);
    if(black_queenText == NULL)
        goto endProg;
    SDL_FreeSurface(black_queen);

/*-------------------------------*/             //Chargement de la texture "white_king"
    SDL_Surface *white_king = NULL;
    white_king = IMG_Load("src/roi_blanc.png");
    if(white_king == NULL)
        goto endProg;
    SDL_Texture *white_kingText = NULL;
    white_kingText = SDL_CreateTextureFromSurface(renderer, white_king);
    if(white_kingText == NULL)
        goto endProg;
    SDL_FreeSurface(white_king);


/*-------------------------------*/             //Chargement de la texture "black_king"
    SDL_Surface *black_king = NULL;
    black_king = IMG_Load("src/roi_noir.png");
    if(black_king == NULL)
        goto endProg;
    SDL_Texture *black_kingText = NULL;
    black_kingText = SDL_CreateTextureFromSurface(renderer, black_king);
    if(black_kingText == NULL)
        goto endProg;
    SDL_FreeSurface(black_king);

/*-------------------------------*/             //Chargement de la texture "Promotion"
    SDL_Surface *promote = NULL;
    promote = IMG_Load("src/Promotion.png");
    if(promote == NULL)
        goto endProg;
    SDL_Texture *promote_text = NULL;
    promote_text = SDL_CreateTextureFromSurface(renderer, promote);
    if(promote_text == NULL)
        goto endProg;
    SDL_FreeSurface(promote);

    if(load == 1)
        turn = loading(tab, &rocque_white, &rocque_black);


    while (game)
    {
        SDL_Delay(100);

        /*-------------------------------*/             //Affichage de la texture "board"
                                //On défini un rectangle où l'on placera la texture
        rectangle.w = WINDOW_WIDTH;                                               //On défini la largeur de ce que l'on affichera
        rectangle.h = WINDOW_HEIGHT;                                              //On défini la hauteur de ce que l'on affichera
        rectangle.x = 0;                                                                //On défini la coordonnée x du coin en haut à gauche de ce que l'on affichera
        rectangle.y = 0;                                                                //On défini la coordonnée y du coin en haut à gauche de ce que l'on affichera

        if(SDL_RenderCopy(renderer, boardText, NULL, &rectangle)){ 
            goto endProg;
            
        }
        
        rectangle.w = 81;                                               //On défini la largeur de ce que l'on affichera
        rectangle.h = 81;                                              //On défini la hauteur de ce que l'on affichera
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                switch(tab[i * 8 + j]){
                    case 0:
                        break;
                    case 1: 
                        /*-------------------------------*/             //Affichage de la texture "white_pin"
                                                //On défini un rectangle où l'on placera la texture
                        rectangle.x = 134 + 81*j;                                                                //On défini la coordonnée x du coin en haut à gauche de ce que l'on affichera
                        rectangle.y = 18 + 81*i;                                                                //On défini la coordonnée y du coin en haut à gauche de ce que l'on affichera

                        if(SDL_RenderCopy(renderer, white_pinText, NULL, &rectangle))
                            goto endProg;
                        break;

                    case -1: 
                        /*-------------------------------*/             //Affichage de la texture "black_pin"
                        rectangle.x = 134 + 81*j;
                        rectangle.y = 18 + 81*i;

                        if(SDL_RenderCopy(renderer, black_pinText, NULL, &rectangle))
                            goto endProg;
                        break;

                    case 2:    
                        /*-------------------------------*/             //Affichage de la texture "white_tower"
                        rectangle.x = 134 + 81*j;
                        rectangle.y = 18 + 81*i;


                        if(SDL_RenderCopy(renderer, white_towerText, NULL, &rectangle))
                            goto endProg;
                        break;

                    case -2:
                        /*-------------------------------*/             //Affichage de la texture "black_tower"
                        rectangle.x = 134 + 81*j;
                        rectangle.y = 18 + 81*i;

                        if(SDL_RenderCopy(renderer, black_towerText, NULL, &rectangle))
                            goto endProg;
                        break;

                    case 3:
                        /*-------------------------------*/             //Affichage de la texture "white_horse"
                        rectangle.x = 134 + 81*j;
                        rectangle.y = 18 + 81*i;

                        if(SDL_RenderCopy(renderer, white_horseText, NULL, &rectangle))
                            goto endProg;
                        break;
                    
                    case -3:
                        /*-------------------------------*/             //Affichage de la texture "black_horse"
                        rectangle.x = 134 + 81*j;
                        rectangle.y = 18 + 81*i;

                        if(SDL_RenderCopy(renderer, black_horseText, NULL, &rectangle))
                            goto endProg;
                        break;

                    case 4:
                        /*-------------------------------*/             //Affichage de la texture "white_mad"
                        rectangle.x = 134 + 81*j;
                        rectangle.y = 18 + 81*i;

                        if(SDL_RenderCopy(renderer, white_madText, NULL, &rectangle))
                            goto endProg;
                        break;

                    case -4:
                        /*-------------------------------*/             //Affichage de la texture "black_mad"
                        rectangle.x = 134 + 81*j;
                        rectangle.y = 18 + 81*i;

                        if(SDL_RenderCopy(renderer, black_madText, NULL, &rectangle))
                            goto endProg;
                        break;

                    case 5:
                        /*-------------------------------*/             //Affichage de la texture "white_queen"
                        rectangle.x = 134 + 81*j;
                        rectangle.y = 18 + 81*i;

                        if(SDL_RenderCopy(renderer, white_queenText, NULL, &rectangle))
                            goto endProg;
                        break;

                    case -5:
                        /*-------------------------------*/             //Affichage de la texture "black_queen"
                        rectangle.x = 134 + 81*j;
                        rectangle.y = 18 + 81*i;

                        if(SDL_RenderCopy(renderer, black_queenText, NULL, &rectangle))
                            goto endProg;
                        break;

                    case 6:
                        /*-------------------------------*/             //Affichage de la texture "white_king"
                        rectangle.x = 134 + 81*j;
                        rectangle.y = 18 + 81*i;

                        if(SDL_RenderCopy(renderer, white_kingText, NULL, &rectangle))
                            goto endProg;
                        break;

                    case -6:
                        /*-------------------------------*/             //Affichage de la texture "black_king"
                        rectangle.x = 134 + 81*j;
                        rectangle.y = 18 + 81*i;

                        if(SDL_RenderCopy(renderer, black_kingText, NULL, &rectangle))
                            goto endProg;
                        break;
                    
                    default:
                        printf("Display error : pieces\n");
                        break;
                }
            }
        }

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (possibility_tab[i + 8 * j] == 1 || possibility_tab[i + 8 * j] == 3 || possibility_tab[i + 8 * j] == 4){
                    rectangle.x = i * 81 + 134;
                    rectangle.y = j * 81 + 18;

                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    SDL_SetRenderDrawColor(renderer, 150, 175, 0, 70);
                    SDL_RenderFillRect(renderer, &rectangle);
                }
                else if (possibility_tab[i + 8 * j] == 2 || possibility_tab[i + 8 * j] == 6){
                    rectangle.x = i * 81 + 134;
                    rectangle.y = j * 81 + 18;

                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    SDL_SetRenderDrawColor(renderer, 150, 0, 0, 70);
                    SDL_RenderFillRect(renderer, &rectangle);
                }
            }
        }

/*-------------------------------*/             //Chargement de la texture "tour" et affichage
        switch (turn)
        {
        case -1:
            sprintf(texte_c, "Tour : Noir");
            break;
        case 1:
            sprintf(texte_c, "Tour : Blanc");
            break;
        
        default:
            break;
        }
        texte = TTF_RenderText_Blended(police, texte_c, noir);
        
        text_texte = SDL_CreateTextureFromSurface(renderer, texte);
        SDL_FreeSurface(texte);
        if(text_texte == NULL)
            goto endProg;
                            //On défini un rectangle où l'on placera la texture
        rectangle.w = 24*(countLetters(texte_c)-1);
        rectangle.h = 40;
        rectangle.x = 10;
        rectangle.y = 700;

        if(SDL_RenderCopy(renderer, text_texte, NULL, &rectangle))
            goto endProg; 
        SDL_DestroyTexture(text_texte);

        if (is_promote == 1){
            rectangle.w = 400;
            rectangle.h = 400;
            rectangle.x = 200;
            rectangle.y = 200;

            if(SDL_RenderCopy(renderer, promote_text, NULL, &rectangle))
                goto endProg; 
        }


        if (check_white == 1)
        {
            rectangle.w = 81;
            rectangle.h = 81;
            rectangle.x = case_white_king%8 * 81 + 134;
            rectangle.y = case_white_king/8 * 81 + 18;

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 150, 0, 0, 70);
            SDL_RenderFillRect(renderer, &rectangle);
        }
        
        if (check_black == 1)
        {
            rectangle.w = 81;
            rectangle.h = 81;
            rectangle.x = case_black_king%8 * 81 + 134;
            rectangle.y = case_black_king/8 * 81 + 18;

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 150, 0, 0, 70);
            SDL_RenderFillRect(renderer, &rectangle);
        }

        SDL_RenderPresent(renderer);                    //On actualise le rendu 

        

/*-------------------------------*/             //Partie IA

        if(turn == -1 && game_mode == 3)
            IA(tab, rocque_black, &turn, &win, case_black_king, pion_pass_take);
        if(win != 0)
            game = SDL_FALSE;


/*-------------------------------*/             //Sauvergarde des états d'échecs

        if (check_white == 2)
        {
            win = -1;
            game = SDL_FALSE;
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "ECHEC", "ECHEC ET MAT BLANC", NULL);
        }
        if (check_black == 2)
        {
            win = 1;
            game = SDL_FALSE;
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "ECHEC", "ECHEC ET MAT NOIR", NULL);
        }
        
        check_white = is_checkmate(tab, case_white_king, 1, rocque_white);
        check_black = is_checkmate(tab, case_black_king, -1, rocque_black);
        

/*-------------------------------*/             //Gestion des evenements
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT :
                    game = SDL_FALSE;
                    game_over = SDL_FALSE;
                    break;

                case SDL_KEYDOWN :
                    if(event.key.keysym.sym == SDLK_r){
                        reset_tab(tab);
                        init_tab(tab);
                        turn = 1;
                        rocque_black = 3;
                        rocque_white = 3;
                    }
                    if(event.key.keysym.sym == SDLK_1){
                        win = 1;
                        game = SDL_FALSE;
                    }
                    if(event.key.keysym.sym == SDLK_9){
                        win = -1;
                        game = SDL_FALSE;
                    }
                    break;
                
                case SDL_MOUSEBUTTONDOWN:
                    if (event.motion.x >= 134 && event.motion.x < 750 && event.motion.y > 18 && event.motion.y < 665 && is_promote == 0){

                        click_mouse = floor((event.motion.x-134)/81) + 8 * floor((event.motion.y-18)/81);

                        // Tour Blanc
                        if (turn == 1 && tab[click_mouse] > 0)
                                verif_tab(possibility_tab, tab, click_mouse, rocque_white, pion_pass_take);
                        // Tour Noir
                        if (turn == -1 && tab[click_mouse] < 0) 
                                verif_tab(possibility_tab, tab, click_mouse, rocque_black, pion_pass_take);
                        
                        if (turn == 1 && check_white == 1 && tab[last_click_mouse] > 0 && tab[click_mouse] <= 0 && last_click_mouse != click_mouse)
                        {
                            tab[click_mouse] = tab[last_click_mouse];
                            tab[last_click_mouse] = 0;

                            if(tab[click_mouse] == 6)
                                case_white_king = click_mouse;

                            if (is_checkmate(tab, case_white_king, turn, rocque_white) != 0)
                                can_move = 0;

                            if(tab[click_mouse] == 6)
                                case_white_king = last_click_mouse;

                            tab[last_click_mouse] = tab[click_mouse];
                            tab[click_mouse] = 0;
                        }
                        else if (turn == -1 && check_black == 1 && tab[last_click_mouse] < 0 && tab[click_mouse] >= 0 && last_click_mouse != click_mouse)
                        {
                            tab[click_mouse] = tab[last_click_mouse];
                            tab[last_click_mouse] = 0;

                            if(tab[click_mouse] == -6)
                                case_white_king = click_mouse;

                            if (is_checkmate(tab, case_black_king, turn, rocque_black) != 0)
                                can_move = 0;

                            if(tab[click_mouse] == -6)
                                case_white_king = last_click_mouse;

                            tab[last_click_mouse] = tab[click_mouse];
                            tab[click_mouse] = 0;
                        }
                        // //Verification de l'auto-mise en echec
                        // else if (turn == 1 && tab[last_click_mouse] > 0 && tab[click_mouse] <= 0 && last_click_mouse != click_mouse)
                        // {
                        //     tab[click_mouse] = tab[last_click_mouse];
                        //     tab[last_click_mouse] = 0;

                        //     if(tab[click_mouse] == 6)
                        //         case_white_king = click_mouse;

                        //     printf("enter");
                        //     if (is_checkmate(tab, case_black_king, turn, rocque_black) != 0)
                        //         can_move = 0;
                        //     printf("exit\n");

                        //     if(tab[click_mouse] == -6)
                        //         case_white_king = last_click_mouse;

                        //     tab[last_click_mouse] = tab[click_mouse];
                        //     tab[click_mouse] = 0;
                        // }
                        // else if (turn == -1 && tab[last_click_mouse] < 0 && tab[click_mouse] >= 0 && last_click_mouse != click_mouse)
                        // {
                        //     tab[click_mouse] = tab[last_click_mouse];
                        //     tab[last_click_mouse] = 0;

                        //     if(tab[click_mouse] == -6)
                        //         case_white_king = click_mouse;

                        //     if (is_checkmate(tab, case_black_king, turn, rocque_black) != 0)
                        //         can_move = 0;

                        //     if(tab[click_mouse] == -6)
                        //         case_white_king = last_click_mouse;

                        //     tab[last_click_mouse] = tab[click_mouse];
                        //     tab[click_mouse] = 0;
                        // }
                        else 
                            can_move = 1;

                        if (can_move == 0)
                            reset_tab(possibility_tab);
                        



                        if (can_move == 1)
                        {    
                            switch (possibility_tab[click_mouse])
                            {
                                case 0 :
                                    reset_tab(possibility_tab);
                                    break;

                                case 1:
                                    apply_moves(tab, click_mouse, last_click_mouse, &win, &game, &turn, &rocque_white, &rocque_black, &is_promote, &case_white_king, &case_black_king, &pion_pass_take);
                                    reset_tab(possibility_tab);
                                    break;

                                case 2 :
                                    apply_moves(tab, click_mouse, last_click_mouse, &win, &game, &turn, &rocque_white, &rocque_black, &is_promote, &case_white_king, &case_black_king, &pion_pass_take);
                                    reset_tab(possibility_tab);
                                    break;

                                case 4 : 
                                    if (click_mouse%8<4)
                                    {
                                        tab[click_mouse] = tab[last_click_mouse];
                                        tab[last_click_mouse] = 0;
                                        tab[click_mouse+1] = tab[last_click_mouse-3];
                                        tab[last_click_mouse-3] = 0;
                                        switch (turn)
                                        {
                                        case 1:
                                            rocque_white = 0;
                                            break;
                                        case -1:
                                            rocque_black = 0;
                                            break;
                                        }
                                        turn *= -1;
                                        reset_tab(possibility_tab);
                                    }
                                    if (click_mouse%8>4)
                                    {
                                        tab[click_mouse] = tab[last_click_mouse];
                                        tab[last_click_mouse] = 0;
                                        tab[click_mouse-1] = tab[last_click_mouse+4];
                                        tab[last_click_mouse+4] = 0;
                                        switch (turn)
                                        {
                                        case 1:
                                            rocque_white = 0;
                                            break;
                                        case -1:
                                            rocque_black = 0;
                                            break;
                                        }
                                        turn *= -1;
                                        reset_tab(possibility_tab);
                                    }
                                    break;

                                case 6:
                                    if (possibility_tab[last_click_mouse-1] == 7)
                                    {
                                        tab[click_mouse] = tab[last_click_mouse];
                                        tab[last_click_mouse] = 0;
                                        tab[last_click_mouse-1] = 0;
                                        turn*=-1;
                                        reset_tab(possibility_tab);
                                    }
                                    if (possibility_tab[last_click_mouse+1] == 7)
                                    {
                                        tab[click_mouse] = tab[last_click_mouse];
                                        tab[last_click_mouse] = 0;
                                        tab[last_click_mouse+1] = 0;
                                        turn*=-1;
                                        reset_tab(possibility_tab);
                                    }
                                    
                                    break;
                                
                                default:
                                    break;
                            }
                        }
                        
                        last_click_mouse = click_mouse;
                    }



                    //      COMMUN A TOUS LES MODES
                    if (event.motion.x >= 24 && event.motion.x <= 111 && event.motion.y >= 428 && event.motion.y <= 496 && is_promote == 0)
                    {
                        win = -1*turn;
                        game = SDL_FALSE;
                    }
                    if (event.motion.x >= 24 && event.motion.x <= 111 && event.motion.y >= 24 && event.motion.y <= 412 && is_promote == 0)
                        save(tab, turn, rocque_white, rocque_black);

                    // Partie promotion
                    if(is_promote == 1){
                        if (event.motion.x >= 275 && event.motion.x <= 515 && event.motion.y >= 300 && event.motion.y <= 340){
                            tab[click_mouse] = turn * 3;
                            is_promote = 0;
                            turn *= -1;
                        }
                        else if (event.motion.x >= 275 && event.motion.x <= 515 && event.motion.y >= 360 && event.motion.y <= 400){
                            tab[click_mouse] = turn * 2;
                            is_promote = 0;
                            turn *= -1;
                        }
                        if (event.motion.x >= 275 && event.motion.x <= 515 && event.motion.y >= 420 && event.motion.y <= 460){
                            tab[click_mouse] = turn * 4;
                            is_promote = 0;
                            turn *= -1;
                        }
                        else if (event.motion.x >= 275 && event.motion.x <= 515 && event.motion.y >= 480 && event.motion.y <= 520){
                            tab[click_mouse] = turn * 5;
                            is_promote = 0;
                            turn *= -1;
                        }
                    }
                    break;

                default :
                    break;
            }
        }
      
        SDL_RenderPresent(renderer);                    //On actualise le rendu   
    }

/*------------------------------------------------------------------------------------------------------------------------*/ 

    while(game_over)
    {
        
        switch (win){
            case -1 : 
                sprintf(texte_c, "Le joueur noir est vainqueur !");
                break;
            case 1 : 
                sprintf(texte_c, "Le joueur blanc est vainqueur !");
                break;
            case 0 : 
                sprintf(texte_c, "Match nul !");
                break;
            default:
                break;

        }
        rectangle.w = WINDOW_WIDTH;
        rectangle.h = WINDOW_HEIGHT;
        rectangle.x = 0;
        rectangle.y = 0;

        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_RenderFillRect(renderer, &rectangle);

        texte = TTF_RenderText_Blended(police, texte_c, noir);
        if(texte == NULL)
            goto endProg;
        text_texte = SDL_CreateTextureFromSurface(renderer, texte);
        SDL_FreeSurface(texte);
        if(text_texte == NULL)
            goto endProg;
                            //On défini un rectangle où l'on placera la texture
        rectangle.w = 24*(countLetters(texte_c)-1);
        rectangle.h = 40;
        rectangle.x = 50;
        rectangle.y = 375;
        if(SDL_RenderCopy(renderer, text_texte, NULL, &rectangle)!=0)
            goto endProg;
        SDL_DestroyTexture(text_texte);

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT :                     //Dans le cas d'une fermeture de la fenêtre
                    game_over = SDL_FALSE;    //on ferme l'annonce du gagnant
                    break;
                
                case SDL_MOUSEBUTTONDOWN :
                    game_over = SDL_FALSE;
                    break;

                default :
                    break;
            }
        }
        SDL_RenderPresent(renderer);
    }

/*------------------------------------------------------------*/
endProg:
	//Fermeture SDL + fenêtre + rendu
    SDL_DestroyTexture(boardText);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}