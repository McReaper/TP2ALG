#include <stdlib.h>
#include <stdio.h>
#include "a234.h"
#include "pile.h"

ppile_t creer_pile ()
{
  ppile_t p = (ppile_t) malloc(sizeof(pile_t));
  p->sommet = -1;
  for (int i = 0; i < MAX_PILE_SIZE; i ++) p->Tab[i] = NULL;
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

pnoeud_t depiler (ppile_t p)
{
  if (pile_vide(p))
    return NULL;
  pnoeud_t noeud = p->Tab[p->sommet];
  p->Tab[p->sommet] = NULL;
  p->sommet--;
  return noeud;
}

int empiler (ppile_t p, pnoeud_t pn)
{
  if (pile_pleine(p))
    return 1;
  p->sommet++;
  p->Tab[p->sommet] = pn;
  return 0;
}

void afficher_pile(ppile_t p){

  if ( p->sommet == -1){
    printf("  ---------\n");
    printf("| pile vide |\n");
    printf("  ---------\n");
  } else {

    printf(" ------\n");
    for (int i = p->sommet; i >= 0; i--) {
      printf("| %4d |", p->Tab[i]->cle );
      if (i == p->sommet){
        printf("<---- sommet (%d)",p->sommet);
      }
      printf("\n");
    }
    printf(" ------\n");
  }
}
