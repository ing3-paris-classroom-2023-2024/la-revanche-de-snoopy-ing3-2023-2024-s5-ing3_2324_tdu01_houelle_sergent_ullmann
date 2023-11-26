#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>



#define TAILLEX 9
#define TAILLEY 8
/*
#define OISEAU           0X0E // note
#define BLOC_INVINCIBLE     0x0F // virus
#define BLOC_DEPLACABLE    0xC4 // trait horizontal
#define SNOOPY           0x01
#define BALLE            0x0B
*/
#define OISEAU           'A' // note
#define BLOC_INVINCIBLE     'B' // virus
#define BLOC_DEPLACABLE    'C' // trait horizontal
#define SNOOPY           'D'
#define BALLE            'E'

char plateau[TAILLEX][TAILLEY];
int balleX=-1, balleY=-1, sensBalle;// sensBalle 0=haut-gauche, 1=haut-droite, 2=bas-gauche, 3=bas-droite
char objetSousBalle;
int snoopyX, snoopyY;
int nbOiseau;


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
        if(snoopyY<TAILLEY-1 && (tcase==' ' || tcase==OISEAU)) snoopyY++;
    }
    // gauche
    if(action=='q'  ) {
        tcase=plateau[snoopyX-1][snoopyY];
        if(snoopyX>0 && (tcase==' ' || tcase==OISEAU)) snoopyX--;
    }
    // droite
    if(action=='d') {
        tcase=plateau[snoopyX+1][snoopyY];
        if(snoopyX<TAILLEX-1  && (tcase==' ' || tcase==OISEAU)) snoopyX++;
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
    if(objetSousBalle==1) return 1;
    plateau[balleX][balleY]=BALLE;
    return 0;

}








// Sous programme pour calculer le score final
void calculerScoreFinal(int tempsNiveau1, int tempsNiveau2, int tempsNiveau3) {

    // Calcul du score de chaque niveau
    int scoreNiveau1 = tempsNiveau1 * 100;
    int scoreNiveau2 = tempsNiveau2 * 100;
    int scoreNiveau3 = tempsNiveau3 * 100;

    // Calcul du score final
    int scoreFinal = scoreNiveau1 + scoreNiveau2 + scoreNiveau3;
    printf("Score Final:%d\n",scoreFinal);
}



//Sous programme pour afficher les règles du jeu à partir d'un fichier texte
void afficherRegles() {
    // Ouverture du fichier
    FILE *fichierRegles = fopen("C:\\Users\\Nicolas\\Desktop\\Regles.txt", "r");

    // Vérifier si le fichier a bien été ouvert
    if (fichierRegles == NULL) {
        printf("Erreur lors de l'ouverture du fichier des regles.\n");
        return;
    }

    // Lire et afficher le contenu du fichier
    char ligne[1000];
    while (fgets(ligne, sizeof(ligne), fichierRegles) != NULL) {
        printf("%s", ligne);
    }

    // Fermer le fichier après lecture
    fclose(fichierRegles);
}




// Sous programme affichage Timer et Score du niveau en cours
void TimerScore(int* score) {
    time_t startTime;
    time(&startTime);

    int durée = 120;  // Initialisation de la durée du timer à 120 secondes

    while (difftime(time(NULL), startTime) < durée) {
        int remainingTime = durée - difftime(time(NULL), startTime);

        // Calcul du score en fonction du temps restant
        *score = remainingTime * 100;

        // Affichage du score et du temps restant
        printf("\rScore : %d - Temps restant: %d secondes", *score, remainingTime);
        fflush(stdout);

    }

    printf("\nTemps ecoule : Vous avez perdu une vie\n");
}




// Sous programme pour les mots de passe liés à chaque niveau
int verifmdp(const char *bonmdp) {
    char mdpentre[20];
    scanf("%s", mdpentre);

    //Comparer le mot de passe entré avec le bon
    int i = 0;
    while (mdpentre[i] == bonmdp[i] && mdpentre[i] != '\0') {
        i++;
    }

    if (mdpentre[i] == '\0' && bonmdp[i] == '\0') {
        printf("Mot de passe correct.\n");
        return 1;
    } else {
        printf("Mot de passe incorrect.\nRetour au menu principal.\n");
        return 0;
    }
}




//Fonction Principale
int main() {
    int tempsNiveau1 = 3;
    int tempsNiveau2 = 12;
    int tempsNiveau3 = 24;
    int score = 0;
    //TimerScore(&score);
    char motdepasse[][20] = {"Snoopy1", "Snoopy2", "Snoopy3"};

    // Menu du jeu
    int a;
    balleX=-1; balleY=-1; sensBalle=0;
    char c;
    int fin, i, v;
    printf("1) Regles du jeu\n2) Lancer un nouveau jeu a partir du niveau 1\n3) Charger une partie\n4) Lancer directement un niveau via son Mot de passe\n5) Scores\n6) Quitter\n");
    scanf("%d",&a);

    switch (a) {
        case 1:
            printf("Voici les regles du jeu:\n");
            afficherRegles();
            break;

        case 2:
            printf("Lancement du jeu au niveau 1");
            break;

        case 3:
            printf("Chargement de la partie sauvgardee");
            break;

        case 4:
            printf("Saisir le mot de passe pour lancer un niveau\n");

            //Menu pour lancer un niveau parmi les 3 à l'aide des mots de passe
            int b;
            printf("1) Saisie du mot de passe pour le lancement du niveau 1\n2) Saisie du mot de passe pour le lancement du niveau 2\n3) Saisie du mot de passe pour le lancement du niveau 3\n");
            scanf("%d",&b);
            switch (b) {

                case 1:
                    printf("Saisie du mot de passe pour le lancement du niveau 1\n");
                if(verifmdp(motdepasse[0])){
                    printf("Lancement du Niveau 1\n");
                }
                    break;

                case 2:
                    printf("Saisie du mot de passe pour le lancement du niveau 2\n");
                if(verifmdp(motdepasse[1])){
                    printf("Lancement du Niveau 2\n");
                    if(lectureFichier("C:\\Users\\Nicolas\\Desktop\\PlateauSnoopy.txt")<0) return 0;
                    constructionPlateau();
                    fin=0; nbOiseau=4;
                    while(fin!=1) {
                        constructionPlateau();
                        //affichePlateau(0,0);
                        for(i=0; i<6000; i++) {
                            if(_kbhit()!=0) {
                                c=getch();
                                if(c=='p') exit(0);
                                snoopyAction(c);
                                if(nbOiseau==0) {
                                    fin=1;
                                    printf("\n Yoouppiii Snoopy !!!!!\n");
                                }
                                else constructionPlateau();
                            }
                        }
                        v=ballAction();
                        if(v==1) {
                            // snoopy touche par balle
                            fin=1;
                            printf("\n Snoopy tue par balle!!!!\n");
                        }
                    }
                    return 0;
                }
                    break;

                case 3:
                    printf("Saisie du mot de passe pour le lancement du niveau 3\n");
                if(verifmdp(motdepasse[2])){
                    printf("Lancement du Niveau 3\n");
                }
                break;
            }
            break;

        case 5:
            printf("Affichage des scores\n");
            calculerScoreFinal(tempsNiveau1, tempsNiveau2, tempsNiveau3);
            break;

        case 6:
            break;


    }


}



