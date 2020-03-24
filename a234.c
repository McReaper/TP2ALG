#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a234.h"
#include "file.h"
#include "pile.h"

#define max(a,b) ((a)>(b)?(a):(b))

int GetNbElems(Arbre234 a){
  //Renvoie l'index du plus grand fils dans un noeud => si 0 alors type noeud = 0
  return (a->t > 0 ? a->t-1 : a->t);
}

int GetCle(Arbre234 a, int index){
  switch (a->t) {
    case 2:
      if(index >= 1) return -1;
      return a->cles[1];
    case 3:
      if(index >= 2) return -1;
      return a->cles[index];
    case 4 :
      return a->cles[index];
    default:
      return -1;
  }
}

Arbre234 GetFils(Arbre234 a, int index){
  switch (a->t) {
    case 2:
      if(index >= 2) return NULL;
      return a->fils[index+1];
    case 3:
      if(index >= 3) return NULL;
      return a->fils[index];
    case 4 :
      return a->fils[index];
    default:
      return NULL;
  }
}

void detruire_arbre(Arbre234 a){

  /*
  Libération en mémoire de l'arbre
  */

  if (a != NULL) {

    detruire_arbre(GetFils(a,0));
    detruire_arbre(GetFils(a,1));
    detruire_arbre(GetFils(a,2));
    detruire_arbre(GetFils(a,3));

    free(a);
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
  while(GetNbElems(current) != 0){
    prec = current;
    current = GetFils(current,GetNbElems(current));
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
    while(i != GetNbElems(a)){
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
      while(i <= GetNbElems(a)){
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
  /*
     calculer le nombre de feuilles, de 2-noeuds, 3-noeuds,et 4-noeuds
  */
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

Arbre234 nmw /*Noeud Max Worker*/ (Arbre234 max, Arbre234 a) {
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
  return nmw(a, a);
}


void Afficher_Cles_Largeur (Arbre234 a)
{
  /*
    Afficher le cles de l'arbre a avec
    un parcours en largeur
  */

  pfile_t f = creer_file();
  int err = enfiler(f, a);
  while (!err && !file_vide(f)) {
    Arbre234 n = defiler(f);
    if (n != NULL) {
      for (int i = 0; i < n->t && !err; i++) {
        err = enfiler(f, GetFils(n, i));
      }
      if (err) break;
      for (int i = 0; i < n->t-1 ; i++) {
        printf("%d | ", GetCle(n, i));
      }
    }
  }
  printf("\n");
  if (err || detruire_file(f)) {
    fprintf(stderr,"Erreur avec la file (enfilement ou destruction) !\n");
    exit(-1);
  }
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

  if (a->t !=0) {

    Affichage_Cles_Triees_Recursive(GetFils(a,0));

    printf("%d | ", GetCle(a,0));

    Affichage_Cles_Triees_Recursive(GetFils(a,1));

    if (a->t > 2){ //3 clefs ou plus présentes dans le noeuds
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
  ppile_t pile = creer_pile();
  empiler_noeud(pile, a);
  while(!pile_vide(pile)){
    if(type_sommet(pile) == TYPE_ENTIER){
      int entier_courant = depiler_entier(pile);
      printf("%d | ", entier_courant);
    } else {
      Arbre234 current = depiler_noeud(pile);
      if (current != NULL && current->t !=0) {
        int nb_elems = GetNbElems(current);
        for (int i = 0; i < nb_elems; i++) {
          empiler_noeud(pile,GetFils(current,nb_elems-i));
          empiler_entier(pile,GetCle(current,nb_elems-1-i));
        }
        empiler_noeud(pile, GetFils(current, 0));
      }
    }
  }
  printf("\n");
  detruire_pile(pile);
}


//Recupere un index adapter au type d arbre
int GetIndex(Arbre234 a, int index){
  if(a->t == 2){
    return index + 1;
  } else {
    return index;
  }
}

//Modifie le noeud et renvoie la cle
Arbre234 RemoveMaxKey(Arbre234 a){
  //Type 3 ou 4 :
  if(a->t == 3){
    //Si c est un type 3 on decale ces cles vers la droite car il devient type 2
    a->cles[2] = a->cles[1];
    a->cles[1] = a->cles[0];
    a->t = a->t-1;
  } else {
    a->t = a->t-1;
  }
  return a;
}

//Modifie le noeud et renvoie la cle
Arbre234 RemoveMinKey(Arbre234 a){
  //Type 3 ou 4 :
  if(a->t == 3){
    a->t = a->t-1;
  } else {
    //On decal les cle vers la gauche
    a->cles[0] = a->cles[1];
    a->cles[1] = a->cles[2];
    a->t = a->t-1;
  }
  return a;
}

//Modifie le noeud et le renvoie
Arbre234 AddMaxKeyToNoeud2(Arbre234 a, int nouvelle_cle){
  a->t = a->t + 1;
  a->cles[2] = nouvelle_cle;
  return a;
}

//Modifie le noeud et le renvoie
Arbre234 AddMinKeyToNoeud2(Arbre234 a, int nouvelle_cle){
  a->t = a->t + 1;
  a->cles[2] = a->cles[1];
  a->cles[1] = nouvelle_cle;
  return a;
}


// Renvoie 1 si l'emprunt a ete effectue 0 sinon
int EmpruntGauche(Arbre234 parent, int cle){
  int i = 0;
  while(i < parent->t-1 && GetCle(parent, i) < cle){
    i++;
  }

  //Ici i vaut l'index de la premiere des cles egal ou superieur a cle
  if(i < parent->t-1){
    //Il n'y a pas de frere gauche on ne peut rien emprunter
    if(i == 0){
      return 0;
    }
    //Le nb de cle dans le frere est trop petit pour un emprunt en renvois 0
    if(GetFils(parent, i-1)->t <= 2){
      return 0;
    }
    //On recupere la cle max du fils gauche
    int max_key = GetCle(GetFils(parent, i-1), GetFils(parent, i-1)->t-2);
    //On enleve cette cle du fils gauche
    parent->fils[GetIndex(parent, i-1)] = RemoveMaxKey(GetFils(parent, i-1));
    //On met la cle du parent dans le fils droit
    parent->fils[GetIndex(parent, i)] = AddMinKeyToNoeud2(GetFils(parent, i), parent->cles[GetIndex(parent, i-1)]);
    //On remplace la cle qu on a enlever au parent par le max du fils gauche
    parent->cles[GetIndex(parent, i-1)] = max_key;
    return 1;
  }
  return 0; //Operation echouer
}

// Renvoie 1 si l'emprunt a ete effectue 0 sinon
int EmpruntDroit(Arbre234 parent, int cle){
  int i = 0;
  while(i < parent->t-1 && GetCle(parent, i) < cle){
    i++;
  }
  //Ici i vaut l'index de la premiere des cles egal ou superieur a cle
  //Si sa nest pas le cas sa veut dire que nous navons pas de fils droit pour faire l emprunt
  if(i + 1 < parent->t-1){
    //Le nb de cle dans le frere est trop petit pour un emprunt en renvois 0
    if(GetFils(parent, i+1)->t <= 2){
      return 0;
    }
    //On recupere la cle min du fils droit
    int min_key = GetCle(GetFils(parent, i+1), 0);
    //On enleve cette cle du fils droit
    parent->fils[GetIndex(parent, i+1)] = RemoveMinKey(GetFils(parent, i+1));
    //On met la cle du parent dans le fils gauche
    parent->fils[GetIndex(parent, i)] = AddMaxKeyToNoeud2(GetFils(parent, i), parent->cles[GetIndex(parent, i)]);
    //On remplace la cle qu on a enlever au parent par le min du fils droit
    parent->cles[GetIndex(parent, i)] = min_key;
    return 1; //Operation reussis
  }
  return 0; //Operation echouer
}

Arbre234 SupprimeCle(Arbre234 a, int cle){
  //Trouver l indice de la cle
  int i = 0;
  while(i < a->t-1 && GetCle(a, i) != cle){
    i++;
  }
  //Decaler vers la gauche toute les cle superieur
  for (int j = i+1; j < a->t-1; j++) {
    a->cles[GetIndex(a, j-1)] = a->cles[GetIndex(a, j)];
  }
  //Si c est un type 3 il vas devenir type 2 on decal donc ces valeur d un cran vers la droite
  if(a->t == 3){
    a->cles[2] = a->cles[1];
    a->cles[1] = a->cles[0];
    a->t = a->t-1;
  }
  a->t = a->t - 1;
  return a;
}
Arbre234 getParent(Arbre234 a){
  return a;
}


void Detruire_Cle_Feuille (Arbre234 a, int cle){
  Arbre234 parent = getParent(a);
  if(EmpruntGauche(parent, cle)){
    a = SupprimeCle(a, cle);
  } else if(EmpruntDroit(parent, cle)){
    a = SupprimeCle(a, cle);
  } else {
    //Cas ou on ne peut pas emprunter sur les cotes :
  }
}

void Detruire_Cle (Arbre234 *a, int cle)
{
  /*
    retirer la cle de l'arbre a
  */
  //On regarde si l'arbre actuel possede la cle:
  // int i = 0;
  // while(i < a->t-1 && GetCle(a, i) < cle){
  //   i++;
  // }
  // if(i < a->t-1 && GetCle(a, i) == cle){
  //   //La cle est presente dans le noeud
  //   if(EstFeuille(a)){
  //     Detruire_Cle_Feuille(a, cle);
  //   } else {
  //     Detruire_Cle_Noeud_Interne(a, cle);
  //   }
  // } else {
  //   if(!EstFeuille(a)){
  //     //Si sa n est pas une feuille on continue a descendre pour le supprime
  //     Detruire_Cle(GetFils(a, i), cle);
  //   }
  // }


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
  detruire_arbre(a);
}
