#include <stdlib.h>
#include <stdio.h>
#include "a234.h"
#include "pile.h"

ppile_t creer_pile ()
{
  ppile_t p = (ppile_t) malloc(sizeof(pile_t));
  p->sommet = -1;
  return p;
}

int detruire_pile (ppile_t p)
{
  if (p == NULL){
    return 1;
  }
  free(p);
  return 0;
}

int pile_vide (ppile_t p)
{
  if (p->sommet == -1){
    return 1;
  } else {
    return 0;
  }
}

int pile_pleine (ppile_t p)
 {
   if (p->sommet >= MAX_PILE_SIZE-1){
     return 1;
   } else {
     return 0;
   }
}

int type_sommet (ppile_t p){
  if (pile_vide(p))
    return -1;
  return p->Tab[p->sommet].type;
}

Arbre234 depiler_noeud (ppile_t p)
{
  if (pile_vide(p))
    return NULL;
  Arbre234 noeud = p->Tab[p->sommet].value.noeud;
  p->Tab[p->sommet].value.noeud = NULL;
  p->sommet--;
  return noeud;
}

int depiler_entier (ppile_t p)
{
  if (pile_vide(p))
    return -1;
  int entier = p->Tab[p->sommet].value.entier;
  p->Tab[p->sommet].value.entier = 0;
  p->sommet--;
  return entier;
}

int empiler_noeud (ppile_t p, Arbre234 pn)
{
  if (pile_pleine(p))
    return 1;
  p->sommet++;
  p->Tab[p->sommet].value.noeud = pn;
  p->Tab[p->sommet].type = TYPE_ARBRE;
  return 0;
}

int empiler_entier (ppile_t p, int pn)
{
  if (pile_pleine(p))
    return 1;
  p->sommet++;
  p->Tab[p->sommet].value.entier = pn;
  p->Tab[p->sommet].type = TYPE_ENTIER;
  return 0;
}

//Cette fonction ne marche plus actuellement :
void afficher_pile(ppile_t p){

  if ( p->sommet == -1){
    printf("  ---------\n");
    printf("| pile vide |\n");
    printf("  ---------\n");
  } else {

    printf(" ------\n");
    for (int i = p->sommet; i >= 0; i--) {
      printf("| %4d |", p->Tab[i].value.entier );
      if (i == p->sommet){
        printf("<---- sommet (%d)",p->sommet);
      }
      printf("\n");
    }
    printf(" ------\n");
  }
}
