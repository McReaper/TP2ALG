#include <stdlib.h>
#include <stdio.h>
#include "a234.h"
#include "file.h"

pfile_t creer_file ()
{
  pfile_t nouvelleFile = malloc(sizeof(file_t));
  nouvelleFile->tete = 0;
  nouvelleFile->queue = 0;
  for (int i = 0; i < MAX_FILE_SIZE; i ++) nouvelleFile->Tab[i] = NULL;
  return nouvelleFile;
}

int detruire_file (pfile_t f)
{
  if(f==NULL)
    return 1;
  free(f);
  return 0;
}

int file_vide (pfile_t f)
{
  return (f->queue == f->tete && f->Tab[f->tete] == NULL ? 1 : 0);
}

int file_pleine (pfile_t f)
{
  if (f->queue != f->tete) return 0;
  int plein = 1;
  for (int i = 0; i < MAX_FILE_SIZE && plein; i++) {
    if (f->Tab[i] == NULL) plein = 0;
  }
  return plein;
}

Arbre234 defiler (pfile_t f)
  {
    if(file_vide(f))
      return NULL;
    Arbre234 elem = f->Tab[f->tete];
    f->Tab[f->tete] = NULL;
    f->tete = (f->tete >= MAX_FILE_SIZE-1 ? 0 : f->tete + 1);
    return elem;
  }

int enfiler (pfile_t f, Arbre234 p)
{
  if (file_pleine(f))
    return 1;
  f->Tab[f->queue]=p;
  f->queue = (f->queue >= MAX_FILE_SIZE-1 ? 0: f->queue + 1);
  return 0 ;
}