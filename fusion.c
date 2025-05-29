#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

//********************************* infos sur la réservation *********************************

#define MAX_RESERVATIONS 100
#define MAX_CODE_LENGTH 10
#define NOMBRE_SALLES 100
#define NOMBRE_CLASSES 100

#define FICHIER_SAUVEGARDE "reservations_salles.txt"

typedef enum {
    RESERVEE,
    ANNULEE,
    VALIDEE,
    EN_COURS,
    TERMINEE
} EtatReservation;

typedef struct {
    int id;
    int salle;
    int classe;
    char jour[10];
    int heure_debut;
    int duree;
    char motif[100];
    EtatReservation etat;
} Reservation;

typedef struct {
    char nom[20];
    int salle_id[20];
    char position[50];
    char disponibilite_machine[30];
    int capacite;
} Salle;

typedef struct {
    char classe_id[20];
    char classe_nom[50];
    char niveau[20];
    int effectif;
} Classe;

Salle salles[NOMBRE_SALLES];
Classe classes[NOMBRE_CLASSES];
Reservation reservations[MAX_RESERVATIONS];
int nb_reservations = 0;
int nb_salles = 0;
int nb_classes = 0;



//***************************************** infos sur la salle *********************************************
void afficherSalles();
void ajouter_salles(struct salles tableau[], int *n);


//***************************************** infos sur la classe ********************************************
void menu_classes ();
void sauvegard (Classe *tab, int n);
int charger_tableau (Classe **tab, int *cap);
void afficher_classe (Classe *tab, int taille);
void ajouter_classe(Classe **tab, int *pointeur, int *cap);
void rechercher_classe (Classe *tab, int taille);
void modifier_classe (Classe *tab, int taille);
void delet_classe (Classe *tab, int *taille);

//***************************************************** Main  ****************************************

int main ()
{
    int choix;
    do 
    {
        SetConsoleOutputCP(CP_UTF8); //juste pour pouvoir permettre à mon compilateur de reconnaitre les caractères accentués

        printf("Bienvenue! Vous êtes dans le menu principal\n");
        printf("[1]Réservations\n[2]Salles\n[3]Classes\n[0]Quitter\n");
        printf("\n> ");
        scanf("%d", &choix);

        switch(choix) 
        {
            case 1:
                // menu_reservations();
                break;
            case 2:
                // menu_salles();
                break;
            case 3:
                menu_classes();
                break;
            case 0:
                printf("Au revoir!\n");
                break;
            default:
                printf("Choix invalide. Saisir de nouveau :\n");

        }
    } while (choix != 0);
}

//************************************************* definition des prototypes **************************************

//                  ************************************ about classe **********************************************    
void menu_classes ()
{
    //la variable taille permet de connaitre la taille actuelle de mon tableau. engros combien de cases sont éffectivements remplies dans le tableau
    //la variable nbre_de_classe permet de gerer de façons dynamique la taille du tableau notamment pour les opérations d'ajout, suppression, modification....Elle indique la taille réelle de mon tableau
    int choix, taille = 0, nbre_de_classe = NOMBRE_CLASSES;

    Classe *t = malloc (nbre_de_classe *sizeof(Classe)); //ceci est mon tableau dynamique qui s'adapte aux différents procédures
    if (!t) //toujours verifier que l'allocation s'est passée comme prévue 
    {
        printf ("\nErreur d'allocation memoire\n");
        return;
    }

    taille = charger_tableau(&t, &nbre_de_classe); //c'est pour charger les données précédentes lors du démarrage de l'apk 

    /*if (taille == 0) // verifier si mes éléments de base sont déja dans le fichier ou pas pour les mettres dedans
    {
        declaration_tableau_base (t);
        taille = nbre_classe_base;
        sauvegard (t, taille);
    }*/

    //menu principal
    do 
    {
        printf ("\n=========== Menu des classes ============\n");
        printf ("\n1 - Ajouter une nouvelle classe\n");
        printf ("2 - Afficher la liste des classes connues\n");
        printf ("3 - Rechercher une classe à partir de son code\n");
        printf ("4 - Modifier une classe connue\n");
        printf ("5 - Supprimer une classe connue\n");
        printf ("0 - Fin de traitement\n");

        printf ("\nFaite votre choix :\t");
        scanf ("%d", &choix);

        switch (choix)
        {
            case 1 :
            {
                ajouter_classe(&t, &taille, &nbre_de_classe);
                sauvegard (t, taille);
                break;
            }
            case 2 :
            {
                afficher_classe (t, taille);
                break;
            }
            case 3 :
            {
                rechercher_classe (t, taille);
                break;
            }
            case 4 :
            {
                modifier_classe (t, taille);
                sauvegard (t, taille);
                break;
            }
            case 5 :
            {
                delet_classe (t, &taille);
                sauvegard (t, taille);
                break;
            }
            case 0 :
            {
                printf ("\nMerci et à la prochaine !!!\n");
                break;
            }
            default :
            {
                printf ("Saisir une valeur correcte (0 - 5):\t");
            }
        }
        printf ("\n");
        // system ("cls");
    
    }while (choix != 0);
    free (t); //libérer la memoire
}

void sauvegard (Classe *tab, int n) //dans le but de sauvegarder des éléments dans un fichier binaire

{   
    FILE *fp;

     fp = fopen ("data_classe.txt", "wb"); //ici j'ai mis le mode "w" à l'ouverture pour pouvoir maitre à jour le fichier
        if (fp == NULL)
        {
            printf ("erreur d'ouverture du fichier !\n");
            return;
        }

        fwrite (&n, sizeof (int), 1, fp); // et bien sur je sauvergarde la taille de mon tableau
        fwrite (tab, sizeof (Classe), n, fp); // également le tableau lui-même
    fclose (fp);
}

int charger_tableau (Classe **tab, int *cap) 
{
    FILE *fp;
    int x = 0;

    fp = fopen ("data_classe.txt", "rb");
        if (fp == NULL)
        {
            printf ("\nAucune sauvegarde trouvée\n");
            return 0;
        }

        fread (&x, sizeof(int), 1, fp); 
        if (x > *cap) 
        {
            *cap = x;
            *tab = realloc (*tab, x * sizeof(Classe));
            if (!*tab) 
            {
                printf("Erreur de réallocation mémoire\n");
                fclose(fp);
                return 0;
            }
        }

        fread (*tab, sizeof(Classe), x, fp);
    fclose (fp); 

    return x;
}

void ajouter_classe(Classe **tab, int *taille, int *cap) //pusiqu'il s'agit d'un ajout, alors je dois également modifier la taille de mon tableau
{
    Classe nouveau;
    int i;

    getchar ();

    printf ("\n************** Ajout d'une nouvelle classe *************\n");
    printf ("saisir le code unique de la classe :\t");
    fgets (nouveau.classe_id, sizeof (nouveau.classe_id), stdin);
    nouveau.classe_id[strcspn (nouveau.classe_id, "\n")] = '\0';
    
    //je me suis rendu compte qu'à chaque fois que j'utilisais un scanf, un caractère \n restait dans le buffer. 
    //ce qui fait en sorte que la prochaine fonction de lecture d'un caractère ou de chaine le lis ! 
    //Cela constituais un réel probleme. j'ai essayer de vider le buffer avec fflush mais sans succès. 
    //Ensuite, un jours dans une correction proposer par le prof Diane, il a user d'un getchar (), c'était 
    //subtil mais bon ... ça fait le taff !!!


    for (i = 0; i <= (*taille - 1); i++)
    {
        if (strcmp((*tab)[i].classe_id, nouveau.classe_id) == 0)
        {
            printf ("\nLa classe exite déjà\n");
            return;
        }
    }

    printf ("Saisir le nom de la classe :\t");
    fgets (nouveau.classe_nom, sizeof (nouveau.classe_nom), stdin);
    nouveau.classe_nom[strcspn (nouveau.classe_nom, "\n")] = '\0';

    printf ("Saisir le niveau de la classe :\t");
    fgets (nouveau.niveau, sizeof (nouveau.niveau), stdin);
    nouveau.niveau[strcspn (nouveau.niveau, "\n")] = '\0';

    do 
    {
        printf ("Saisir l'effectif de la classe (entier positif):\t");
        scanf ("%d", &nouveau.effectif);

    }while (nouveau.effectif < 0);

    if (*taille >= *cap)// cette action je la fais pour m'assurer si je dois augmenter la taille de mon tableau en cas de bavurre avant l'ajout d'une nouvelle classe
    {
        *cap = *cap * 2;
        *tab = realloc (*tab, (*cap) * sizeof(Classe));
        if (!*tab) 
        {
            printf("\nErreur de réallocation memoire\n");
            return;
        }
    }

    (*tab)[*taille] = nouveau;
    (*taille)++;
    printf("\nClasse ajoutée avec succès.\n");
}

void afficher_classe (Classe *tab, int taille) //rien de plus simple 
{
    int i;

    if (taille == 0)
    {
        printf("\nAucune classe enregistrée.\n");
        return;
    }

    printf("\nListe des classes connues :\n");
    printf ("\n");
    
    for (i = 0; i <= (taille - 1); i++) 
    {
        printf("Code :\t%s\n", tab[i].classe_id);
        printf("Nom :\t%s\n", tab[i].classe_nom);
        printf("Niveau :\t%s\n", tab[i].niveau);
        printf("Effectif :\t%d\n", tab[i].effectif);
        printf ("\n");
    }
}

void rechercher_classe (Classe *tab, int taille)
{
    int i;
    char codeR[100];

    if (taille == 0)
    {
        printf("\nAucune classe enregistrée.\n");
        return;
    }

    getchar ();
    printf ("Saisir le code la classe recherchée :\t");
    fgets (codeR, sizeof (codeR), stdin);
    codeR[strcmp (codeR, "\n")] = '\0';

    for (i = 0; i <= (taille - 1); i++)
    {
        if (strcmp(tab[i].classe_id, codeR) == 0)
        {
            printf ("\nClasse trouvée !\n");
            printf ("\nCode : %s\n", tab[i].classe_id);
            printf ("Nom de la classe : %s\n", tab[i].classe_nom);
            printf ("Niveau de la classe : %s\n", tab[i].niveau);
            printf ("Effectif de la classe : %d\n", tab[i].effectif);
            return;
        }
    }

    printf ("Classe non trouvée !\n");
}

void modifier_classe (Classe *tab, int taille)
{
    int i;
    char codeR[100];

    if (taille == 0)
    {
        printf("\nAucune classe enregistrée.\n");
        return;
    }

    getchar (); // je ne vais plus commettre la même erreur !!!

    printf ("\nSaisir le code de la classe à modifier :\t");
    fgets (codeR, sizeof (codeR), stdin);
    codeR[strcspn (codeR, "\n")] = '\0';

    for (i = 0; i <= (taille - 1); i++)
    {
        if (strcmp (tab[i].classe_id, codeR) == 0)
        {
            printf ("Saisir le nouveau nom :\t");
            fgets (tab[i].classe_nom, sizeof(tab[i].classe_nom), stdin);
            tab[i].classe_nom[strcspn(tab[i].classe_nom, "\n")] = '\0'; // pour éviter d'avoir de erreur de comparaison de chaînes

            printf ("Saisir le nouveau niveau de la classe :\t");
            fgets (tab[i].niveau, sizeof(tab[i].niveau), stdin);
            tab[i].niveau[strcspn(tab[i].niveau, "\n")] = '\0';

            do 
            {
                printf ("Saisir le nouvel éffectif (entier positif) :\t");
                scanf ("%d", &tab[i].effectif);
                getchar ();

            }while (tab[i].effectif <= 0);

            printf ("\nModifications éffectuées avec succès !\n");

            return;
        }
    }

    printf ("Classe non trouvée !\n");
}

void delet_classe (Classe *tab, int *taille)//taille est passée en pointeur car je voudrais avoir à soustraire une classe, par conséquent, la taille aussi sera modifiée
{
    int i, enlever;
    char codeR[100];
    
    if (*taille == 0)
    {
        printf("\nAucune classe enregistrée.\n");
        return;
    }

    getchar ();
    printf ("\nSaisir le code de la classe à supprimer :\t");
    fgets (codeR, sizeof (codeR), stdin);
    codeR[strcspn (codeR, "\n")] = '\0';

    //je vais commencer par chercher l'indice de la classe à supprimer par le biais du code qu'aura saisi l'utilisateur
    enlever = -1; // -1 parce que dans un tableau, les indices sont positifs

    for (i = 0; i <= (*taille - 1); i++)
    {
        if (strcmp (tab[i].classe_id, codeR) == 0)
        {
            enlever = i;
            break;
        }
    }

    //si le code n'est pas trouvé, alors je m'arrête ici
    if (enlever == -1)
    {
        printf ("\nClasse non trouvée !\n");
        return;
    }

    //sinon, je vais commencer par décaler toutes les cases qui suivent cette case vers la gauche 
    for (i = enlever; i <= (*taille - 2); i++)
    {
        tab[i] = tab[i + 1];
    }

    //ensuite je mets à jours la taille de mon tableau 
    *taille = *taille - 1;

    printf ("\nClasse supprimée avec succès !\n");
}

//                  ************************************ about salle **********************************************  

void afficherSalles() {
    FILE *fp = fopen("data_salles.txt", "r");
    char contenu[200];
    int vide=1;
    if (fp == NULL) {
        printf("Erreur de lecture du fichier.\nVous n'avez sans doute pas encore ajoutee une salle\n");
        //j'ai rajoute ce truc car si l'utilisateur n'a pas encore ajoute une salle
        //si il choisit un des le debut ca vas lui dire erreur de lecture du fichier
        //commme ca ca sera plus clair pour lui qu'il devra d'abord ajouter une salle avant de
        //vouloir les afficher
        return;
    }
    while (fgets(contenu, sizeof(contenu), fp) != NULL) {
        fputs(contenu, stdout);
        vide=2;
    }
    if(vide==1){
        printf("Vous n'avez pas encore ajoutee de salle\n");
    }
    fclose(fp);
}

/*
void ajouter_salles(Salle tableau[], int *n) {
    int i, h, reponse = 1;
    do {
        getchar();  // Consommer le '\n'
        printf("Saisir le nom de la salle:\n");
        scanf("%99[^\n]", tableau[*n].nom);  //  ajout à l'indice *n
        getchar();

        printf("Saisir la position de la salle:\n");
        scanf("%99[^\n]", tableau[*n].position);
        getchar();

        printf("Saisir la disponibilite des machines:\n");
        scanf("%99[^\n]", tableau[*n].disponibilite_machine);
        getchar();

        printf("Saisir la capacite de la salle: \n");
        scanf("%d", &tableau[*n].capacite);
        getchar();

        do {
            h = 0;
            printf("Saisir le code unique: \n");
            scanf("%d", &tableau[*n].code);
            for (i = 0; i < *n; i++) {
                if (tableau[*n].code == tableau[i].code) {
                    printf("Ce code existe deja. Veuillez en saisir un autre.\n");
                    h = 1;
                    break;
                }
            }
        } while (h == 1);

        (*n)++;  // on augmente la taille apres verification

        FILE *fichier = fopen("salles.txt", "w");
        if (fichier == NULL) {
            printf("Impossible d'ouvrir le fichier\n");
            return;
        }

        for (i = 0; i < *n; i++) {
            fprintf(fichier, "Nom: %s\nCode unique: %d\nPosition: %s\nDisponibilite des machines: %s\nCapacite: %d\n\n",
                    tableau[i].nom, tableau[i].code, tableau[i].position,
                    tableau[i].disponibilite_machine, tableau[i].capacite);
        }

        fclose(fichier);
        printf("Salle ajoutee avec succes !\n");
        printf("Voulez-vous ajouter une nouvelle salle ? Tapez 1 pour oui : ");
        scanf("%d", &reponse);
    } while (reponse == 1);
}
    */