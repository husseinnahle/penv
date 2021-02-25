/* ********************* *
 * TP0 INF3173 H2021
 * Nom: Hussein
 * Prénom: Nahle
 * ********************* *

 .---------------------------------.
 |              Code               |
 |---------------------------------|
 | VIDE --> env vide               |
 | BSN  --> env contient \n        |
 | MAX  --> valeur max d'une ligne |
 | ERR  --> erreur de fichier/path |
 '---------------------------------'
                                    */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 5000
#define ERR 5001
#define VIDE 5002
#define BSN 5003

/**
 *  Afficher la valeur de la variable d'environnement (print i á '\0')
 *
 *  Parametres:
 *  char *ligne --> chaine de caractere dans /environ/PID/proc
 *  int i       --> indice du premier caractere apres '='
 *
 *  Codes de retours:
 *  0           --> succes
 *  VIDE        --> valeur de la chaine est null
 *  BSN		--> valeur contenant un '\n'
 *		    (chaine qui continue sur une nouvelle ligne)
 */

int printValue(char *ligne, int i){
    int j = 0;
    for(i = i; ligne[i] != '\0'; i++){
        printf("%c", ligne[i]);
        j++;
    }
    if (ligne[i-1] == '\n' && i > 1) return BSN;
    if(j == 0) return VIDE;
    return 0;
}

/**
 *  Comparer le Nom entrer en parametre et le nom dans ligne (compare i á '=')
 *
 *  Parametres:
 *  char *ligne --> chaine de caractere dans /environ/PID/proc
 *  char *argv  --> nom de la variable
 *  int i       --> indice du premier caractere de argv
 *
 *  Codes de retours:
 *  1		--> nom incorrect
 *  printValue(ligne, i+1)
 */

int compare(char *ligne, char *argv, int i){
    int y = 0;
    while (ligne[i] != '=') {
        if ((ligne[i] != argv[y]) || y > sizeof(argv)) return 1;
        i++;
        y++;
    }
    return printValue(ligne, i + 1);
}

/**
 * Méthode qui recherche le nom entrer en parametre. Et retourne un code
 * en rapport avec le resultat (si le nom est trouver ou non/erreur)
 *
 *  Parametres:
 *  char *path  --> chaine "/proc/PID/environ"
 *  char *nom   --> nom de la variable d'environnement
 *
 *  Codes de retours:
 *  0 		--> succes
 *  2		--> erreur de fichier
 */

int search(char *path, char *nom){
    FILE *environ;
    environ = fopen(path, "r");
    if(!environ){
        printf("File error: %s\n", path);
        return 2;
    }
    char ligne[MAX];
    int resultat = 1;
    while (fgets(ligne, MAX, environ)){
        for(int i=0; i<MAX; i++){
            if(ligne[i] == '\0'){
                resultat = compare(ligne, nom, i + 1);
                if(resultat == 0 || resultat == VIDE) return 0;
                else if (resultat == BSN) break;
            }else if (resultat == BSN){
                resultat = printValue(ligne, i);
                break;
            }
        }
    }
    fclose(environ);
    return resultat;
}

char *getPath(char *pid, char *fichier){
    char *path;
    path = malloc(sizeof(pid) + sizeof(fichier) + sizeof("/proc/"));
    strcat(path, "/proc/");
    strcat(path, pid);
    strcat(path, fichier);
    return path;
}

int getPPID(char *argv){
    char* path;
    path = getPath(argv, "/stat");
    FILE *stat = fopen(path, "r");
    if(!stat){
        printf("File error: %s\n", path);
        return ERR;
    }
    int ppid = 0;
    fscanf(stat, "%*d %*s %*c %d", &ppid);
    fclose(stat);
    free(path);
    return ppid;
}

/**
 * Verifier si les arguments sont valides
 *
 * NB: On verifie pas si le pid/ppid est valide car s'il est invalide
 * une erreur va etre declancher lors de l'ouverture du fichier (fopen)
 */

int verifyArgs(int argc, char *argv[]){
    if(argc == 4 && strcmp(argv[1], "-p") == 0){
        return getPPID(argv[2]);
    }else if(argc != 3){
        printf("usage: penv [-p] PID NOM\n");
        return ERR;
    }
    return 0;
}

int main(int argc, char *argv[]){
    int resultat = 2;
    int i = 0;
    int tmp = verifyArgs(argc, argv);
    if(tmp == ERR){
        return 2;
    }else if(tmp != 0){
        sprintf(argv[2], "%d", tmp);
        i++;
    }
    char *path;
    path = getPath(argv[1+i], "/environ");
    resultat = search(path, argv[2+i]);
    free(path);

    return resultat;
}
