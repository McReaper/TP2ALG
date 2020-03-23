#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a234.h"

#define max(a,b) ((a)>(b)?(a):(b))


int hauteur (Arbre234 a)
{
  int h0, h1, h2, h3 ;

  if (a == NULL)
    return 0 ;

  if (a->t == 0)
    return 0 ;

  h0 = hauteur (a->fils [0]) ;
  h1 = hauteur (a->fils [1]) ;
  h2 = hauteur (a->fils [2]) ;
  h3 = hauteur (a->fils [3]) ;

  return 1 + max (max (h0,h1),max (h2,h3)) ;
}

int NombreCles (Arbre234 a)
{
  /*
     Retourne le nombre de cles presentes dans l'arbre a
  */

  //noeud vide
  if (a->t ==0){
    return 0;;
  }

  int nb_cles = 0;
  //cas de récurence : appel récursif sur chacun des fils du noeud courant.
  for (size_t i = 0; i < a->t; i++) {
    nb_cles = NombreCles(a->fils[i]);
  }
  //cas de base : ajout des cles du noeud courant.
  return nb_cles + a->t -1; //si il y a 4 noeuds il y a 3 clefs.
}

int GetIndexMax(Arbre234 a){
  //Renvoie l'index du plus grand fils dans un noeud => si 0 alors type noeud = 0
  return (a->t > 0? a->t-1 , a->t);
}

int CleMax (Arbre234 a)
{
  Arbre234 current = a;
  Arbre234 prec = current;
  while(GetIndexMax(current) != 0){
    prec = current;
    current = current->fils[GetIndexMax(current)];
  }
  return prec->cle[GetCleMax(prec)];
}

int CleMin (Arbre234 a)
{
  Arbre234 current = a;
  Arbre234 prec = current;
  while(current->t != 0){
    prec = current;
    current = current->fils[0];
  }
  return prec->cle[0];

}

Arbre234 RechercherCle (Arbre234 a, int cle)
{
  /*
     rechercher si la cle a est presente dans
     l'arbre a. Si oui, retourne le noeud ou se trouve la cle.
  */

  return NULL ;
}

void AnalyseStructureArbre (Arbre234 a, int *feuilles, int *noeud2, int *noeud3, int *noeud4)
{
  /*
     calculer le nombre de feuilles, de 2-noeuds, 3-noeuds,et 4-noeuds
  */
  if(a->t != 0){
    int i = 0;
    while(i != GetIndexMax(a)){
      AnalyseStructureArbre(a->fils[i], feuilles, noeud2, noeud3, noeud4);
      i++;
    }
  }
}

Arbre234 noeud_max (Arbre234 a)
{
  /*
    Retourne le noeud avec la somme maximale des cles internes
  */

  return NULL ;
}


void Afficher_Cles_Largeur (Arbre234 a)
{
  /*
    Afficher le cles de l'arbre a avec
    un parcours en largeur
  */

  return ;
}

void Affichage_Cles_Triees_Recursive (Arbre234 a)
{
  /*
     Afficher les cles en ordre croissant
     Cette fonction sera recursive
  */

}

void Affichage_Cles_Triees_NonRecursive (Arbre234 a)
{
    /*
     Afficher les cles en ordre croissant
     Cette fonction ne sera pas recursive
     Utiliser une pile
  */

}


void Detruire_Cle (Arbre234 *a, int cle)
{
  /*
    retirer la cle de l'arbre a
  */

  return ;
}




int main (int argc, char **argv)
{
  Arbre234 a ;

  if (argc != 2)
    {
      fprintf (stderr, "il manque le parametre nom de fichier\n") ;
      exit (-1) ;
    }

  a = lire_arbre (argv [1]) ;

  printf ("==== Afficher arbre ====\n") ;
  afficher_arbre (a, 0) ;

  printf ("\n==== Infos Arbres ====\n") ;

  // printf("Nombre de clés : %d\n",NombreCles (a));
  printf("Clé maximale : %d\n",CleMax (a));
  printf("Clé minimale : %d\n",CleMin (a));

  printf ("\n==== Rechercher clé ====\n") ;

  Arbre234 RechercherCle (Arbre234 a, int cle);

  printf ("\n==== Analyser structure arbre ====\n") ;

  int feuilles, noeud2, noeud3, noeud4;
  AnalyseStructureArbre (a, &feuilles, &noeud2, &noeud3, &noeud4);
  printf("Nb feuilles : %d\n", feuilles);
  printf("Nb 2-noeuds : %d\n", noeud2);
  printf("Nb 3-noeuds : %d\n", noeud3);
  printf("Nb 4-noeuds : %d\n", noeud4);

  printf ("\n==== Noeud max ====\n");

  afficher_arbre(noeud_max (a),0);

  printf ("\n==== Afficher clés largeur ====\n");

  Afficher_Cles_Largeur (a);

  printf ("\n==== Afficher clés triées récursivement ====\n");

  Affichage_Cles_Triees_Recursive (a);

  printf ("\n==== Afficher clés triées non-récursivement ====\n") ;

  Affichage_Cles_Triees_NonRecursive (a);

  printf ("\n==== Destruction clé arbre ====\n") ;

  for (int i = 3; i < 10; i+=2) {
    printf("Destruction clé n°%d", i);
    Detruire_Cle (&a, i);
    afficher_arbre(a,0);
  }

  afficher_arbre (a, 0) ;
  // int nbclef = NombreCles(a);
  // printf("nombres de clefs dans l'arbre a : %d\n",nbclef);

}
