#include <stdio.h>
#include <time.h>


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



