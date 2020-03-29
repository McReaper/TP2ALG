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

    detruire_arbre(a->fils[0]);
    detruire_arbre(a->fils[1]);
    detruire_arbre(a->fils[2]);
    detruire_arbre(a->fils[3]);

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

    Arbre234 buffer = a->fils[1];
    a->fils[1] = a->fils[0];
    a->fils[0] = a->fils[3];
    a->fils[3] = a->fils[2];
    a->fils[2] = buffer;
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
    Arbre234 buffer = a->fils[1];
    a->fils[1] = a->fils[0];
    a->fils[0] = a->fils[3];
    a->fils[3] = a->fils[2];
    a->fils[2] = buffer;
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
  if(EstFeuille(a)){
    a->t = a->t + 1;
    a->cles[2] = nouvelle_cle;
    a->cles[0] = a->cles[1];
    a->cles[1] = a->cles[2];
  } else {
    a->t = a->t + 1;
    a->cles[0] = a->cles[1];
    a->cles[1] = GetCle(a->fils[2] , 0);
    a->fils[2]->cles[1] = nouvelle_cle;
  }
  Arbre234 buffer = a->fils[0];
  a->fils[0] = a->fils[1];
  a->fils[1] = a->fils[2];
  a->fils[2] = a->fils[3];
  a->fils[3] = buffer;

  return a;
}

//Modifie le noeud et le renvoie
Arbre234 AddMinKeyToNoeud2(Arbre234 a, int nouvelle_cle){
  if (EstFeuille(a)){
    a->t = a->t + 1;
    a->cles[2] = nouvelle_cle;
    a->cles[0] = a->cles[1];
    a->cles[1] = a->cles[2];

  } else {
    a->t = a->t + 1;
    a->cles[0] = GetCle(a->fils[1] , 0);
    a->fils[1]->cles[1] = nouvelle_cle;

  }
  Arbre234 buffer = a->fils[0];
  a->fils[0] = a->fils[1];
  a->fils[1] = a->fils[2];
  a->fils[2] = a->fils[3];
  a->fils[3] = buffer;
  return a;
}


// Renvoie 1 si l'emprunt a ete effectue 0 sinon
int EmpruntGauche(Arbre234 *parent, int cle){
  int i = 0;
  while(i < (*parent)->t-1 && GetCle((*parent), i) < cle){
    i++;
  }

  //Ici i vaut l'index de la premiere des cles egal ou superieur a cle
  if(i <= (*parent)->t-1){
    //Il n'y a pas de frere gauche on ne peut rien emprunter
    if(i == 0){
      return 0;
    }
    //Le nb de cle dans le frere est trop petit pour un emprunt en renvois 0
    if(GetFils((*parent), i-1)->t <= 2){
      return 0;
    }
    //On recupere la cle max du fils gauche
    int max_key = GetCle(GetFils((*parent), i-1), GetFils((*parent), i-1)->t-2);
    //On enleve cette cle du fils gauche
    (*parent)->fils[GetIndex((*parent), i-1)] = RemoveMaxKey(GetFils((*parent), i-1));
    //On met la cle du parent dans le fils droit
    (*parent)->fils[GetIndex((*parent), i)] = AddMinKeyToNoeud2(GetFils((*parent), i), (*parent)->cles[GetIndex((*parent), i-1)]);
    //On remplace la cle qu on a enlever au parent par le max du fils gauche
    (*parent)->cles[GetIndex((*parent), i-1)] = max_key;
    return 1;
  }
  return 0; //Operation echouer
}

// Renvoie 1 si l'emprunt a ete effectue 0 sinon
int EmpruntDroit(Arbre234 *parent, int cle){
  int i = 0;
  while(i < (*parent)->t-1 && GetCle((*parent), i) < cle){
    i++;
  }
  //Ici i vaut l'index de la premiere des cles egal ou superieur a cle
  //Si sa nest pas le cas sa veut dire que nous navons pas de fils droit pour faire l emprunt
  if(i + 1 <= (*parent)->t-1){
    //Le nb de cle dans le frere est trop petit pour un emprunt en renvois 0
    if(GetFils((*parent), i+1)->t <= 2){
      return 0;
    }
    //On recupere la cle min du fils droit
    int min_key = GetCle(GetFils((*parent), i+1), GetIndex(GetFils((*parent), i+1), 0));
    //On enleve cette cle du fils droit
    (*parent)->fils[GetIndex((*parent), i+1)] = RemoveMinKey(GetFils((*parent), i+1));
    //On met la cle du parent dans le fils gauche
    (*parent)->fils[GetIndex((*parent), i)] = AddMaxKeyToNoeud2(GetFils((*parent), i), (*parent)->cles[GetIndex((*parent), i)]);
    //On remplace la cle qu on a enlever au parent par le min du fils droit
    (*parent)->cles[GetIndex((*parent), i)] = min_key;
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

    Arbre234 buffer = a->fils[3];
    a->fils[3] = a->fils[2];
    a->fils[2] = a->fils[1];
    a->fils[1] = a->fils[0];
    a->fils[0] = buffer;
  }
  a->t = a->t - 1;
  return a;
}

//--------------------------------------------------------------------------------
// Valeur global nescessaire pour la destruction de cle
Arbre234 parent = NULL;
void SetParent(Arbre234 a){
  parent = a;
}

Arbre234 GetParent(){
  return parent;
}

int global_niveau = 0;
void SetNiveau(int nouvelle_valeur){
  global_niveau = nouvelle_valeur;
}

int GetNiveau(){
  return global_niveau;
}

void IncrNiveau(){
  global_niveau = global_niveau + 1;
}

//--------------------------------------------------------------------------------
Arbre234 merge2noeud(Arbre234 dst, Arbre234 src, int cle);

int MergeGauche(Arbre234 *a, int cle){
  int i = 0;
  Arbre234 parent = GetParent();
  while(i < GetParent()->t-1 && GetCle(GetParent(), i) < cle){
    i++;
  }
  if(i > 0 && i <= parent->t-1){
    parent->fils[GetIndex(parent, i-1)] = merge2noeud(parent->fils[GetIndex(parent, i-1)], parent->fils[GetIndex(parent, i)],GetCle(parent, i));
    if(parent->t == 3){
      //Cest des noeud3
      if(i == 1){
        Arbre234 buff = parent->fils[1];
        parent->fils[1] = parent->fils[0];
        parent->fils[0] = buff;
        parent->fils[0]->t = 0;
        (*a) = parent->fils[1];
      } else {
        // i == 2
        Arbre234 buff = parent->fils[2];
        parent->fils[2] = parent->fils[1];
        parent->fils[1] = parent->fils[0];

        parent->fils[0] = buff;
        parent->fils[0]->t = 0;

        parent->cles[1] = parent->cles[0];

        (*a) = parent->fils[2];
      }
    } else {
      //Cest des noeud4
      if(i == 1){
        Arbre234 buffer = parent->fils[1];
        parent->fils[1] = parent->fils[2];
        parent->fils[2] = parent->fils[3];

        parent->fils[3] = buffer;
        parent->fils[3]->t = 0;

        parent->cles[0] = parent->cles[1];
        parent->cles[1] = parent->cles[2];

        (*a) = parent->fils[0];
      } else if(i == 2){
        Arbre234 buffer = parent->fils[2];
        parent->fils[2] = parent->fils[3];

        parent->fils[3] = buffer;
        parent->fils[3]->t = 0;

        parent->cles[1] = parent->cles[2];

        (*a) = parent->fils[1];
      } else {
        //i == 3
        parent->fils[3]->t = 0;

        (*a) = parent->fils[2];
      }
    }
    parent->t = parent->t - 1 ;
    return 1;
  }
  return 0;
}

int MergeDroite(Arbre234 *a, int cle){
  int i = 0;
  while(i < GetParent()->t-1 && GetCle(GetParent(), i) < cle){
    i++;
  }
  if(i + 1 <= parent->t-1){
    parent->fils[GetIndex(parent, i)] = merge2noeud(parent->fils[GetIndex(parent, i)], parent->fils[GetIndex(parent, i + 1)],GetCle(parent, i));
    if(parent->t == 3){
      //Cest des noeud3
      if(i + 1 == 1){
        Arbre234 buff = parent->fils[1];
        parent->fils[1] = parent->fils[0];
        parent->fils[0] = buff;
        parent->fils[0]->t = 0;
        (*a) = parent->fils[1];
      } else {
        // i + 1 == 2
        Arbre234 buff = parent->fils[2];
        parent->fils[2] = parent->fils[1];
        parent->fils[1] = parent->fils[0];

        parent->fils[0] = buff;
        parent->fils[0]->t = 0;

        parent->cles[1] = parent->cles[0];

        (*a) = parent->fils[2];
      }
    } else {
      //Cest des noeud4
      if(i + 1 == 1){
        Arbre234 buffer = parent->fils[1];
        parent->fils[1] = parent->fils[2];
        parent->fils[2] = parent->fils[3];

        parent->fils[3] = buffer;
        parent->fils[3]->t = 0;

        parent->cles[0] = parent->cles[1];
        parent->cles[1] = parent->cles[2];

        (*a) = parent->fils[0];
      } else if(i + 1 == 2){
        Arbre234 buffer = parent->fils[2];
        parent->fils[2] = parent->fils[3];

        parent->fils[3] = buffer;
        parent->fils[3]->t = 0;

        parent->cles[1] = parent->cles[2];

        (*a) = parent->fils[1];
      } else {
        //i == 3
        parent->fils[3]->t = 0;

        (*a) = parent->fils[2];
      }
    }
    parent->t = parent->t - 1 ;
    return 1;
  }
  return 0;
}

//Fonction qui insere le noeud source dans le noeud destination avec la cle entre les 2
Arbre234 merge2noeud(Arbre234 dst, Arbre234 src, int cle){
  int taille = dst->t + src->t;
  //On verifie que la fusion fera bien un noeud de taille reglementaire
  //  et qu aucun des noeud est de type 0
  if(taille <= 4 && dst->t != 0 && src->t != 0){
    Arbre234 plusPetit = (GetCle(dst, 0) < GetCle(src, 0) ? dst : src);
    Arbre234 plusGrand = (GetCle(dst, 0) > GetCle(src, 0) ? dst : src);

    plusPetit->fils[0] = plusPetit->fils[1];
    plusPetit->fils[1] = plusPetit->fils[2];

    plusPetit->fils[2] = plusGrand->fils[1];
    plusPetit->fils[3] = plusGrand->fils[2];

    plusPetit->cles[0] = plusPetit->cles[1];
    plusPetit->cles[1] = cle;
    plusPetit->cles[2] = plusGrand->cles[1];

    dst = plusPetit;
    dst->t = taille;
    return dst;
  }
  return NULL;
}

void Detruire_Cle_Feuille (Arbre234 a, int cle){
  if(a->t > 2){
    //Cas 1 si on est dans une feuille avec t > 2
    //On supprime
    a = SupprimeCle(a, cle);
  } else {
    //Sinon on essaye d emprunter
    Arbre234 parent = GetParent(a);
    //Nous supprimons la seul cle dans la racine donc
    if(parent == NULL){
      // detruire_arbre(a);
      return ;
    }

    if(EmpruntGauche(&parent, cle)){
      a = SupprimeCle(a, cle);
    } else if(EmpruntDroit(&parent, cle)){
      a = SupprimeCle(a, cle);
    } else {
      //Cas ou on ne peut pas emprunter sur les cotes :
      //Cas de merge : si parent t > 2
      if(GetParent()->t > 2){
        if(MergeGauche(&a, cle)){
          a = SupprimeCle(a, cle);
        } else if(MergeDroite(&a, cle)){
          afficher_arbre(a, cle);
          a = SupprimeCle(a, cle);
        }
      }
      //Tout les cas de noeud feuille on ete traiter normalement
    }
  }
}

void Detruire_Cle (Arbre234 a, int cle);

int RemplaceGauche(Arbre234 a, int indice){
  Arbre234 fils_gauche = GetFils(a, indice);
  if(fils_gauche->t > 2){
    //Prendre la plus grande cle a gauche et la mettre a la place de la cle a supprimer
    a->cles[GetIndex(a, indice)] = GetCle(fils_gauche, fils_gauche->t-2);
    //Puis supprimer de façons recursive
    Arbre234 context_parent = GetParent();
    int context_niveau = GetNiveau();
    SetParent(a);
    IncrNiveau();
    Detruire_Cle(fils_gauche,  GetCle(fils_gauche, fils_gauche->t-2));
    SetNiveau(context_niveau);
    SetParent(context_parent);

    return 1;
  }
  return 0;
}

int RemplaceDroite(Arbre234 a, int indice){
  Arbre234 fils_droit = GetFils(a, indice+1);
  if(fils_droit->t > 2){
    //Prendre la plus petite cle a droite et la mettre a la place de la cle a supprimer
    a->cles[GetIndex(a, indice)] = GetCle(fils_droit, 0);
    //Puis supprimer de façons recursive
    Arbre234 context_parent = GetParent();
    int context_niveau = GetNiveau();
    SetParent(a);
    IncrNiveau();
    Detruire_Cle(fils_droit,  GetCle(fils_droit, 0));
    SetNiveau(context_niveau);
    SetParent(context_parent);
    // a->fils[GetIndex(a, indice+1)] = fils_droit;
    return 1;
  }
  return 0;
}

// Renvoie 1 si l'emprunt a ete effectue 0 sinon
int EmpruntGaucheInterne(Arbre234 *parent, int cle){
  afficher_arbre((*parent), 0);
  int i = 0;
  while(i < (*parent)->t-1 && GetCle((*parent), i) < cle){
    i++;
  }

  //Ici i vaut l'index de la premiere des cles egal ou superieur a cle
  if(i <= (*parent)->t-1){
    //Il n'y a pas de frere gauche on ne peut rien emprunter
    if(GetIndex((*parent), i) == GetIndex((*parent), 0)){
      return 0;
    }
    //Le nb de cle dans le frere est trop petit pour un emprunt en renvois 0
    if(GetFils((*parent), i-1)->t <= 2){
      return 0;
    }
    //On recupere la cle max du fils gauche
    int max_key = CleMax(GetFils((*parent), i-1));
    // int max_key = GetCle(GetFils((*parent), i-1), GetFils((*parent), i-1)->t-2);

    //On enleve cette cle du fils gauche
    Arbre234 context_parent = GetParent();
    int context_niveau = GetNiveau();
    SetParent((*parent));
    Detruire_Cle((*parent)->fils[GetIndex((*parent), i-1)], max_key);
    SetParent(context_parent);
    SetNiveau(context_niveau);
    //On met la cle du parent dans le fils droit
    int cle_parent = (*parent)->cles[GetIndex((*parent), i-1)];
    //On remplace la cle qu on a enlever au parent par le max du fils gauche
    (*parent)->cles[GetIndex((*parent), i-1)] = max_key;
    ajouter_cle(&((*parent)->fils[GetIndex((*parent), i)]), cle_parent, GetNiveau(), (*parent));

    return 1;
  }
  return 0; //Operation echouer
}

// Renvoie 1 si l'emprunt a ete effectue 0 sinon
int EmpruntDroitInterne(Arbre234 *parent, int cle){
  afficher_arbre((*parent), 0);
  int i = 0;
  while(i < (*parent)->t-1 && GetCle((*parent), i) < cle){
    i++;
  }
  //Ici i vaut l'index de la premiere des cles egal ou superieur a cle
  //Si sa nest pas le cas sa veut dire que nous navons pas de fils droit pour faire l emprunt
  if(i + 1 <= (*parent)->t-1){
    //Le nb de cle dans le frere est trop petit pour un emprunt en renvois 0
    if(GetFils((*parent), i+1)->t <= 2){
      return 0;
    }
    //On recupere la cle min du fils droit
    int min_key = CleMin(GetFils((*parent), i+1));
    // int min_key = GetCle(GetFils((*parent), i+1), GetIndex(GetFils((*parent), i+1), 0));
    //On enleve cette cle du fils droit
    Arbre234 context_parent = GetParent();
    int context_niveau = GetNiveau();
    SetParent((*parent));
    Detruire_Cle((*parent)->fils[GetIndex((*parent), i+1)], min_key);
    SetParent(context_parent);
    SetNiveau(context_niveau);

    int cle_parent = (*parent)->cles[GetIndex((*parent), i)];
    //On remplace la cle qu on a enlever au parent par le min du fils droit
    (*parent)->cles[GetIndex((*parent), i)] = min_key;

    //On met la cle du parent dans le fils gauche
    ajouter_cle(&((*parent)->fils[GetIndex((*parent), i)]), cle_parent, GetNiveau(), (*parent));

    return 1; //Operation reussis

  }
  return 0; //Operation echouer
}

void Detruire_Cle_Noeud_Interne (Arbre234 a, int cle){
  //Trouver l indice de la cle a supprimer
  int i = 0;
  while(i < a->t-1 && GetCle(a, i) != cle){
    i++;
  }

  // 3 cas possible
  if (RemplaceGauche(a, i)){
  } else if(RemplaceDroite(a, i)) {
  } else {
    if (a->t > 2){
      //Sinon on fusionne les fils entourant la cle
      a->fils[GetIndex(a, i)] = merge2noeud(GetFils(a, i), GetFils(a, i+1), cle);
      //On decale les fils qu il reste :

      if(a->t == 3){
        //Cest des noeud3
        if(i == 0){
          Arbre234 buffer = a->fils[1];
          a->fils[1] = a->fils[0];
          a->fils[0] = buffer;
        } else {
          a->cles[1] = a->cles[0];

          Arbre234 buffer = a->fils[2];
          a->fils[2] = a->fils[1];
          a->fils[1] = a->fils[0];
          a->fils[0] = buffer;
        }
      } else {
        //Cest des noeud4
        if(i == 0){
          a->cles[0] = a->cles[1];
          a->cles[1] = a->cles[2];

          Arbre234 buffer = a->fils[1];
          a->fils[1] = a->fils[2];
          a->fils[2] = a->fils[3];
          a->fils[3] = buffer;
        } else if(i == 1){
          a->cles[1] = a->cles[2];

          Arbre234 buffer = a->fils[2];
          a->fils[2] = a->fils[3];
          a->fils[3] = buffer;
        }
      }
      a->t = a->t - 1 ;

      //Et on supprime recursivement

      Arbre234 context_parent = GetParent();
      int context_niveau = GetNiveau();
      Detruire_Cle(a, cle);
      SetParent(context_parent);
      SetNiveau(context_niveau);

    } else {
      if(a->t <= 2){
        if(EmpruntGaucheInterne(&parent, cle)){
          a = SupprimeCle(a, cle);
        } else if(EmpruntDroitInterne(&parent, cle)){
          a = SupprimeCle(a, cle);
        }
      }
    }
  }
}

Arbre234 Fusion(Arbre234 *a){
  if((*a)->t == 2){
    Arbre234 f_gauche = (*a)->fils[1];
    Arbre234 f_droit = (*a)->fils[2];
    if(f_gauche->t == 2 && f_droit->t == 2 && !EstFeuille(f_gauche) && !EstFeuille(f_droit)){
      int sauvCle = (*a)->cles[1];

      free((*a)->fils[0]);
      free((*a)->fils[3]);

      free(f_gauche->fils[0]);
      free(f_droit->fils[0]);

      free(f_gauche->fils[3]);
      free(f_droit->fils[3]);


      (*a)->fils[0] = f_gauche->fils[1];
      (*a)->fils[1] = f_gauche->fils[2];

      (*a)->fils[2] = f_droit->fils[1];
      (*a)->fils[3] = f_droit->fils[2];

      (*a)->cles[0] = f_gauche->cles[1];
      (*a)->cles[1] = sauvCle;
      (*a)->cles[2] = f_droit->cles[1];
      (*a)->t = 4;

      free(f_gauche);
      free(f_droit);
    }
  }
  return (*a);
}

void Equilibrage(Arbre234 a, int index){
  if(index == a->t-1){
    if(NombreCles(GetFils(a, index-1)) > NombreCles(GetFils(a, index))){
      int max_key = CleMax(GetFils(a, index-1));
      Arbre234 context_parent = GetParent();
      int context_niveau = GetNiveau();
      Detruire_Cle(a->fils[GetIndex(a, index-1)], max_key);
      SetParent(context_parent);
      SetNiveau(context_niveau);

      int cle_act = GetCle(a, index-1);
      a->cles[GetIndex(a, index-1)] = max_key;

      ajouter_cle(&(a->fils[GetIndex(a, index)]), cle_act, GetNiveau(), a);
    }
  } else if(GetIndex(a, index) == GetIndex(a, 0)) {
    if(NombreCles(GetFils(a, index+1)) > NombreCles(GetFils(a, index))){
      int min_key = CleMin(GetFils(a, index+1));
      Arbre234 context_parent = GetParent();
      int context_niveau = GetNiveau();
      Detruire_Cle(a->fils[GetIndex(a, index+1)], min_key);
      SetParent(context_parent);
      SetNiveau(context_niveau);

      int cle_act = GetCle(a, index);
      a->cles[GetIndex(a, index)] = min_key;
      ajouter_cle(&(a->fils[GetIndex(a, index)]), cle_act, GetNiveau(), a);
    }
  } else {
    if(NombreCles(GetFils(a, index-1)) > NombreCles(GetFils(a, index))){
      int max_key = CleMax(GetFils(a, index-1));
      Arbre234 context_parent = GetParent();
      int context_niveau = GetNiveau();
      Detruire_Cle(a->fils[GetIndex(a, index-1)], max_key);
      SetParent(context_parent);
      SetNiveau(context_niveau);

      int cle_act = GetCle(a, index-1);
      a->cles[GetIndex(a, index-1)] = max_key;

      ajouter_cle(&(a->fils[GetIndex(a, index)]), cle_act, GetNiveau(), a);
    } else if(NombreCles(GetFils(a, index+1)) > NombreCles(GetFils(a, index))){
      int min_key = CleMin(GetFils(a, index+1));
      Arbre234 context_parent = GetParent();
      int context_niveau = GetNiveau();
      Detruire_Cle(a->fils[GetIndex(a, index+1)], min_key);
      SetParent(context_parent);
      SetNiveau(context_niveau);

      int cle_act = GetCle(a, index);
      a->cles[GetIndex(a, index)] = min_key;

      ajouter_cle(&(a->fils[GetIndex(a, index)]), cle_act, GetNiveau(), a);
    }
  }
}

void Detruire_Cle (Arbre234 a, int cle)
{
  /*
    retirer la cle de l'arbre a
  */
  //Fusion des noeuds si nous somme dans un cas aberant pour un arbre 234

  //Si on est a la racine on verifie que la cle est presente pour la supprime
  if(GetParent()==NULL){
    SetNiveau(0);
    if(RechercherCle(a, cle)==NULL){
      return ;
    }
    a = Fusion(&a);
  }

  //On cherche l'indice a partir du quel les cle sont superieur ou egale a la cle rechercher
  int i = 0;
  while(i < a->t-1 && GetCle(a, i) < cle){
    i++;
  }

  if(i < a->t-1 && GetCle(a, i) == cle){
    //La cle est presente dans le noeud
    if(EstFeuille(a)){
      Detruire_Cle_Feuille(a, cle);
    } else {
      Detruire_Cle_Noeud_Interne(a, cle);
    }
  } else {
    if(!EstFeuille(a)){
      //Partie equilibrage
      Equilibrage(a, i);

      SetParent(a);
      IncrNiveau();
      //Si sa n est pas une feuille on continue a descendre pour le supprime
      Detruire_Cle(GetFils(a, i), cle);
    }
  }
  SetParent(NULL);
  SetNiveau(0);
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
    Detruire_Cle (a, i);
    afficher_arbre(a,0);
  }

  printf("Destruction clé n°%d\n", 300);
  Detruire_Cle (a, 300);
  afficher_arbre(a,0);

  printf("Destruction clé n°%d\n", 82);
  Detruire_Cle (a, 82);
  afficher_arbre(a,0);

  printf("Destruction clé n°%d\n", 85);
  Detruire_Cle (a, 85);
  afficher_arbre(a,0);

  printf("Destruction clé n°%d\n", 80);
  Detruire_Cle (a, 80);
  afficher_arbre(a,0);

  printf("Destruction clé n°%d\n", 30);
  Detruire_Cle (a, 30);
  afficher_arbre(a,0);

  printf("Destruction clé n°%d\n", 285);
  Detruire_Cle (a, 285);
  afficher_arbre(a,0);

  printf("Destruction clé n°%d\n", 200);
  Detruire_Cle (a, 200);
  afficher_arbre(a,0);

  printf("Destruction clé n°%d\n", 20);
  Detruire_Cle (a, 20);
  afficher_arbre(a,0);

  printf("Destruction clé n°%d\n", 350);
  Detruire_Cle (a, 350);
  afficher_arbre(a,0);

  printf("Destruction clé n°%d\n", 50);
  Detruire_Cle (a, 50);
  afficher_arbre(a,0);

  printf("Destruction clé n°%d\n", 10);
  Detruire_Cle (a, 10);
  afficher_arbre(a,0);

  detruire_arbre(a);
}
