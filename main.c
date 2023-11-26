#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define TAILLEX 9
#define TAILLEY 8
/*
#define OISEAU              0X0E // note
#define BLOC_INVINCIBLE     0x0F // virus
#define BLOC_DEPLACABLE     0xC4 // trait horizontal
#define SNOOPY              0x01
#define BALLE               0x0B
// */
///*
#define OISEAU           'A' // note
#define BLOC_INVINCIBLE     'B' // virus
#define BLOC_DEPLACABLE    'C' // trait horizontal
#define SNOOPY           'D'
#define BALLE            'E'
//*/
char plateau[TAILLEX][TAILLEY];
int balleX=-1, balleY=-1, sensBalle;// sensBalle 0=haut-gauche, 1=haut-droite, 2=bas-gauche, 3=bas-droite
char objetSousBalle;
int snoopyX, snoopyY;
int nbOiseau;

///////////////////////////////////////
// Nom du sous-programme : COLOR
// Rôle : change la couleur du texte dans la console, ainsi que la couleur du fond pour la ligne
// Compatibilité : windows XP
/*0 : Noir
1 : Bleu foncé
2 : Vert foncé
3 : Bleu clair
4 : Rouge foncé
5 : Violet
6 : Vert caca d'oie
7 : Gris clair
8 : Gris foncé
9 : Bleu fluo
10 : Vert fluo
11 : Turquoise
12 : Rouge fluo
13 : Violet 2
14 : Jaune
15 : Blanc
*/

///////////////////////////////////////
void Color(int couleurDuTexte,int couleurDeFond) // fonction d'affichage de couleurs
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}

void gotoXY(int x, int y) {
    COORD mycoord;

    mycoord.X=x;
    mycoord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), mycoord);

}

void afficheSnoopyWins()
{
    Color(14,10);
    gotoXY(40,15);
    printf("Snoopy WINS!!! Loading Next Level!");
}

void afficheSnoopyLoose()
{
    Color(14,12);
    gotoXY(40,15);
    printf("Snoopy Loose!!! End of game!");
    gotoXY(1, TAILLEY*4+2);
}


void constructionPlateau()
{

    int i, j, nbCol, nbLig, Col=TAILLEX, Lig=TAILLEY, ti, tj, ligneCourante;
    char objet;

    //char plateau[33][37];

    Color(0,15);
    nbCol = Col * 4 + 1;
    nbLig = Lig * 4 + 1;

    /*for(int i=0; i<nbLig; i++)
    {
        gotoligcol(i+1,3);
        //gotoligcol(0,0);
        for(j=0; j<nbCol; j++)
        {
            //plateau[i][j]=0x03;
            //printf("  %c  ", plateau[i][j]);
            //printf("   ");
            printf("   ");
        }
    }*/

    // gris
    Color(15,8);
    ligneCourante=1;
    gotoXY(3,ligneCourante); ligneCourante++;
    for(j=0; j<TAILLEX; j++)
    {
        printf("            ");
    }
    printf("   ");
    for(j=0; j<TAILLEY; j++)
    {
        //while(_kbhit()==0); _getch();
        // 1ere ligne grise et blanche
        gotoXY(3,ligneCourante); ligneCourante++;
        for(i=0; i<TAILLEX; i++)
        {
            // colonne grise
            Color(15,8);
            printf("   ");
            // case blanche
            Color(0,15);
            printf("         ");
        }
        Color(15,8);
        printf("   ");

        // 2ème ligne grise et blanche avec les objets
        gotoXY(3,ligneCourante); ligneCourante++;
        for(i=0; i<TAILLEX; i++)
        {
            Color(15,8);
            printf("   ");
            objet=plateau[i][j];
            //blanc
            Color(0,15);
            printf("    %c    ",objet);
        }
        Color(15,8);
        printf("   ");

        // 3eme ligne grise et blanche
        gotoXY(3,ligneCourante); ligneCourante++;
        for(i=0; i<TAILLEX; i++)
        {
            Color(15,8);
            printf("   ");
            //blanc
            Color(0,15);
            printf("         ");
        }
        Color(15,8);
        printf("   ");

        // 4eme ligne totalement grise
        Color(15,8);
        gotoXY(3,ligneCourante); ligneCourante++;
        for(i=0; i<TAILLEX; i++)
        {
            printf("            ");
        }
        Color(15,8);
        printf("   ");
    }

    gotoXY(3,ligneCourante); ligneCourante++;
    printf("ballX=%d ballY=%d sensball=%d\n",balleX, balleY, sensBalle);
    Color(15,0);

    printf("\n");

}

int lectureFichier(char *nomFichier) {
    FILE *desc;
    char ligneFichier[64];
    int i, j, val;
    char c;
    desc=fopen(nomFichier,"r");
    if(desc==NULL) {
        printf("Impossible lirefichier %s\n",nomFichier);
        return -1;
    }

    j=0;
    while(fgets(ligneFichier, 64, desc)!=NULL) {
        for(i=0; i<9; i++) {
            c=ligneFichier[i];
            if(c=='0') c=' ';   // case vide
            if(c=='9') c=OISEAU;    // oiseaux
            if(c=='4') c=BLOC_INVINCIBLE;   // bloc invincible
            if(c=='6') c=BLOC_DEPLACABLE;   // bloc deplacable
            if(c=='7') {
                c=SNOOPY;  // snoopy
                snoopyX=i;
                snoopyY=j;
            }
            if(c=='8') {
                c=BALLE;    // balle
                balleX=i; balleY=j;
                objetSousBalle=' ';
            }
            plateau[i][j]=c;
        }
        j++;
        if(j>=20) break;
    }
    fclose(desc);
}

void affichePlateau(int posX, int posY) {
    int i,j;
    //printf("balleX=%d balleY=%d sens=%d\n",balleX, balleY, sensBalle);
    for(j=0;j<10; j++) {
        gotoXY(posX,posY+j);
        for(i=0; i<20; i++) printf("%c",plateau[i][j]);
    }
}

int snoopyAction(char action) {
    char tcase;
    plateau[snoopyX][snoopyY]=' ';
    // haut
    if(action=='z') {
        tcase=plateau[snoopyX][snoopyY-1];
        if(tcase==BALLE) return -1;
        // case vide : snoopy se déplace (s'il n'est pas sur la ligne du haut)
        if(snoopyY>0 && (tcase==' ' || tcase==OISEAU)) snoopyY--;
        else if(((int)tcase & 0xff)==BLOC_DEPLACABLE) {
            // gestion du bloc deplacable
            if(snoopyY>1 && plateau[snoopyX][snoopyY-2]==' ') {
                // si la case haut dessus du bloc est vide alors on deplace le bloc d'un case vers le haut
                plateau[snoopyX][snoopyY-2]=BLOC_DEPLACABLE;
                snoopyY--;
            }
        }
    }
    // bas
    if(action=='s') {
        tcase=plateau[snoopyX][snoopyY+1];
        if(tcase==BALLE) return -1;
        if(snoopyY<TAILLEY-1 && (tcase==' ' || tcase==OISEAU)) snoopyY++;
        else if(((int)tcase & 0xff)==BLOC_DEPLACABLE) {
            // gestion du bloc deplacable
            if(snoopyY<TAILLEY-2 && plateau[snoopyX][snoopyY+2]==' ') {
                // si la case en dessous du bloc est vide alors on deplace le bloc d'une case vers le bas
                plateau[snoopyX][snoopyY+2]=BLOC_DEPLACABLE;
                snoopyY++;
            }
        }
    }
    // gauche
    if(action=='q'  ) {
        tcase=plateau[snoopyX-1][snoopyY];
        if(tcase==BALLE) return -1;
        if(snoopyX>0 && (tcase==' ' || tcase==OISEAU)) snoopyX--;
        else if(((int)tcase & 0xff)==BLOC_DEPLACABLE) {
            // gestion du bloc deplacable
            if(snoopyX>1 && plateau[snoopyX-2][snoopyY]==' ') {
                // si la case à gauche du bloc est vide alors on deplace le bloc d'une case vers la gauche
                plateau[snoopyX-2][snoopyY]=BLOC_DEPLACABLE;
                snoopyX--;
            }
        }
    }
    // droite
    if(action=='d') {
        tcase=plateau[snoopyX+1][snoopyY];
        if(tcase==BALLE) return -1;
        if(snoopyX<TAILLEX-1  && (tcase==' ' || tcase==OISEAU)) snoopyX++;
        else if(((int)tcase & 0xff)==BLOC_DEPLACABLE) {
            // gestion du bloc deplacable
            if(snoopyX<TAILLEX-2 && plateau[snoopyX+2][snoopyY]==' ') {
                // si la case a droite du bloc est vide alors on deplace le bloc d'une case vers la droite
                plateau[snoopyX+2][snoopyY]=BLOC_DEPLACABLE;
                snoopyX++;
            }
        }
    }
    if(tcase==OISEAU) nbOiseau--;

    plateau[snoopyX][snoopyY]=SNOOPY;
    return 1;

}

int ballAction() {
    plateau[balleX][balleY]=objetSousBalle;
    // gestion des rebonds
    if(sensBalle==0) {
        // haut gauche
        balleX--; balleY--;
        if(balleX==0) sensBalle=1;
        if(balleY==0) sensBalle=2;
        if(balleX==0 && balleY==0) {
            sensBalle=3;
            balleX=0; balleY=0;
        }

    } else if (sensBalle==1) {   // 1 haut droite
        // haut droite
        balleX++; balleY--;
        if(balleX==TAILLEX-1) sensBalle=0;
        if(balleY==0) sensBalle=3;
        if(balleX==TAILLEX-1 && balleY==0) {
            sensBalle=2;
            balleX=TAILLEX-1; balleY=0;
        }

    } else if (sensBalle==2) {
        // bas gauche
        balleX--; balleY++;
        if(balleX==0) sensBalle=3;
        if(balleY==TAILLEY-1) sensBalle=0;

        if(balleX==0 && balleY==(TAILLEY-1)) {
            sensBalle=1;
            balleX=0; balleY=TAILLEY-1;
        }


    } else if (sensBalle==3) {
        // bas droite
        balleX++; balleY++;
        if(balleX==TAILLEX-1) sensBalle=2;
        if(balleY==TAILLEY-1) sensBalle=1;

        if(balleX==TAILLEX-1 && balleY==TAILLEY-1) {
            sensBalle=0;
            balleX=TAILLEX-1; balleY=TAILLEY-1;
        }
    }
    if(balleX<0) balleX==0;
    if(balleX>=TAILLEX) balleX=TAILLEX-1;
    if(balleY<0) balleY==0;
    if(balleY>=TAILLEY) balleY=TAILLEY-1;
    objetSousBalle=plateau[balleX][balleY];
    // snoopy touche par balle
    if(objetSousBalle==SNOOPY) return 1;
    plateau[balleX][balleY]=BALLE;
    return 0;

}


int main() {

    balleX=-1; balleY=-1; sensBalle=0;
    char c;
    int fin, i, v;


    SetConsoleCP(850);
    SetConsoleOutputCP(850);

    if(lectureFichier("C:\\Users\\flori\\CLionProjects\\702\\plateau.txt")<0) return 0;
//    if(lectureFichier("plateau.txt")<0) return 0;
    constructionPlateau();
    fin=0; nbOiseau=4;
    while(fin!=1) {
        constructionPlateau();
        //affichePlateau(0,0);
        for(i=0; i<6000; i++) {
            if(_kbhit()!=0) {
                c=_getch();
                if(c=='p') exit(0);
                v=snoopyAction(c);
                if(v==-1) {
                    // snoopy touche par balle
                    fin=1;
                    afficheSnoopyLoose();
                }
                if(nbOiseau==0) {
                    afficheSnoopyWins();
                    Sleep(5000);
                    if(lectureFichier("C:\\Users\\flori\\CLionProjects\\702\\plateau2.txt")<0) return 0;
                    constructionPlateau();
                    nbOiseau=4;
                }
                else constructionPlateau();
//           else affichePlateau(0,0);
            }
        }
        v=ballAction();
        if(v==1) {
            // snoopy touche par balle
            fin=1;
            afficheSnoopyLoose();
            Sleep(5000);
        }
    }
    return 0;
}