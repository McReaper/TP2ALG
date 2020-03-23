#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a234.h"
#include "pile.h"

#define max(a,b) ((a)>(b)?(a):(b))

int GetIndexMax(Arbre234 a){
  //Renvoie l'index du plus grand fils dans un noeud => si 0 alors type noeud = 0
  return (a->t > 0 ? a->t-1 : a->t);
}

Arbre234 GetFils(Arbre234 a, int index){
  if(a->t == 2){
    return a->fils[index+1];
  } else {
    return a->fils[index];
  }
}

int GetCle(Arbre234 a, int index){
  if(a->t == 2){
    return a->cles[index+1];
  } else {
    return a->cles[index];
  }
}

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
    nb_cles += NombreCles(GetFils(a,i));
  }
  //cas de base : ajout des cles du noeud courant.
  return nb_cles + a->t -1; //si il y a 4 noeuds il y a 3 clefs.
}

int CleMax (Arbre234 a)
{
  Arbre234 current = a;
  Arbre234 prec = current;
  while(GetIndexMax(current) != 0){
    prec = current;
    current = GetFils(current,GetIndexMax(current));
  }
  return GetCle(prec, prec->t - 2);

}

int CleMin (Arbre234 a)
{
  Arbre234 current = a;
  Arbre234 prec = current;
  while (current->t != 0) {
    prec = current;
    current = GetFils(current,0);
  }
  return GetCle(prec, 0);

}

Arbre234 RechercherCle (Arbre234 a, int cle)
{
  /*
     rechercher si la cle "cle" est presente dans
     l'arbre a. Si oui, retourne le noeud ou se trouve la cle.
  */
  Arbre234 curr_node = a;
  while(curr_node != NULL && curr_node->t != 0) {
    int nb_elems = curr_node->t - 1;
    int pos = 0, trouve = 0;
    do {
      trouve = (cle <= GetCle(curr_node, pos));
      if (!trouve) pos++;
    } while (pos < nb_elems && !trouve);
    if (GetCle(curr_node, pos) == cle) return curr_node;
    else curr_node = GetFils(curr_node, pos);
  }
  return NULL ;
}

int EstFeuille(Arbre234 a){
  int res = 0;
  if(a->t != 0){
    int i = 0;
    while(i != GetIndexMax(a)){
      if(GetFils(a,i)->t != 0){
        return 0;
      }
      i++;
    }
    res = 1;
  }
  return res;
}

void AnalyseStructureArbreWorker (Arbre234 a, int *feuilles, int *noeud2, int *noeud3, int *noeud4){
  if(a->t != 0){
    //On ajoute 1 a sont type
    if(a->t == 2){
      *noeud2 = *noeud2 + 1;
    } else if(a->t == 3){
      *noeud3 = *noeud3 + 1;
    } else {
      *noeud4 = *noeud4 + 1;
    }
    if(EstFeuille(a) == 0){
      int i = 0;
      //On parcour tout ses enfants pour les analyser
      while(i <= GetIndexMax(a)){
        AnalyseStructureArbreWorker(GetFils(a,i), feuilles, noeud2, noeud3, noeud4);
        i++;
      }
    } else {
      //Cest une feuille on incremente feuilles
      *feuilles = *feuilles + 1;
    }
  }
}

void AnalyseStructureArbre (Arbre234 a, int *feuilles, int *noeud2, int *noeud3, int *noeud4)
{
  *feuilles = 0;
  *noeud2 = 0;
  *noeud3 = 0;
  *noeud4 = 0;
  AnalyseStructureArbreWorker(a, feuilles, noeud2, noeud3, noeud4);
}

int sumCle (Arbre234 a) {
  if (a == NULL) return -1;
  int nb_elems = a->t - 1;
  int tot = 0;
  for (int i = 0; i < nb_elems; i++)
    tot += GetCle(a, i);
  return tot;
}

Arbre234 noeud_max_worker(Arbre234 max, Arbre234 a);
Arbre234 nmw (Arbre234 max, Arbre234 a) {
  return noeud_max_worker(max, a);
}
Arbre234 noeud_max_worker(Arbre234 max, Arbre234 a) {
  if (a == NULL) {
    return max;
  } else if (EstFeuille(a)) {
    if (sumCle(a) > sumCle(max)) return a;
    else return max;
  } else {
    Arbre234 comp = max;
    if (sumCle(a) > sumCle(max)) comp = a;
    return (nmw(comp, nmw(nmw(a->fils[0], a->fils[1]), nmw(a->fils[2], a->fils[3]))));
  }
}

Arbre234 noeud_max (Arbre234 a)
{
  /*
    Retourne le noeud avec la somme maximale des cles internes
  */
  return noeud_max_worker(a, a);
}


void Afficher_Cles_Largeur (Arbre234 a)
{
  /*
    Afficher le cles de l'arbre a avec
    un parcours en largeur
  */

  return ;
}



void Affichage_Cles_Noeud (Arbre234 a)
{
  /*
  Afficher les cles d'un noeud en ordre croissant.
  */
  if (a != NULL){
    for (size_t i = 0; i < a->t -1; i++) {

      printf("%d | ", GetCle(a,i));
    }
  }
}

void Affichage_Cles_Triees_Recursive (Arbre234 a)
{
  /*
     Afficher les cles en ordre croissant
     Cette fonction sera recursive
  */

  if (a != NULL && a->t !=0) {

    Affichage_Cles_Triees_Recursive(GetFils(a,0));

    printf("%d | ", GetCle(a,0));

    Affichage_Cles_Triees_Recursive(GetFils(a,1));

    if (a->t > 2){ //2 clefs ou plus présentes dans le noeuds
      printf("%d | ", GetCle(a,1));

      Affichage_Cles_Triees_Recursive(GetFils(a,2));

      if (a->t > 3) { // 4 clefs présentes dans le noeud.
              printf("%d | ", GetCle(a,2));

              Affichage_Cles_Triees_Recursive(GetFils(a,3));
      }
    }
  }

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

  printf ("\n==== Afficher arbre ====\n") ;

  afficher_arbre (a, 0) ;

  printf ("\n==== Infos Arbres ====\n") ;

  printf("Nombre de clés : %d\n",NombreCles (a));
  printf("Clé maximale : %d\n",CleMax (a));
  printf("Clé minimale : %d\n",CleMin (a));

  printf ("\n==== Rechercher clé ====\n") ;

  for (int i = 3; i < 10; i+=2) {
    printf("Recherche clé n°%d\n", i);
    afficher_arbre(RechercherCle (a, i),0);
  }


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
  printf("Récursif: ");
  // Affichage_Cles_Noeud(a);
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
