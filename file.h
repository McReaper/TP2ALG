
#define MAX_FILE_SIZE       64


typedef struct {
  int tete, queue ;
  Arbre234 Tab [MAX_FILE_SIZE] ;
} file_t, *pfile_t ;

typedef file_t file;

/*
Résultat: une File F
Post-condition: F est crée en mémoire.
*/
pfile_t creer_file () ;

/*
Résultat: 1 si erreur; 0 sinon
Donnée : une file F.
Post-condition: F est libérée en mémoire.
Effets de bord: F n'existe plus
*/
int detruire_file (pfile_t f) ;

/*
Résultat: 1 si f est vide; 0 sinon
Donnée : une pile f.
*/
int file_vide (pfile_t f) ;

/*
Résultat: 1 si f est plein; 0 sinon
Donnée : une pile f.
*/
int file_pleine (pfile_t f) ;

/*
Résultat: renvoie le noeud supprimé. NULL si f est vide
Donnée : une pile f.
Pré-condition: f est non vide.
Post-condition: supprime l'élément à la queue de f.
Effets de bord: f est modifiée
*/
Arbre234 defiler (pfile_t f)  ;

/*
Résultat: 1 si Erreur (file déjà pleine); 0 sinon
Données : une pile f.
Post-condition: ajoute le noeud p a la tete de f.
Effets de bord: f est modifiée
*/
int enfiler (pfile_t f, Arbre234 p) ;
