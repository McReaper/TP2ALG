
#define MAX_PILE_SIZE       32


typedef union data {
  int entier;
  Arbre234 noeud;
} *Data;

typedef struct {
  int sommet ;
  Data Tab [MAX_PILE_SIZE] ;
} pile_t, *ppile_t ;


/*
Résultat: une Pile P
Post-condition: P est crée en mémoire.
*/
ppile_t creer_pile () ;

/*
Résultat: 1 si erreur; 0 sinon
Donnée : une pile P.
Post-condition: P est libérée en mémoire.
Effets de bord: P n'existe plus
*/
int detruire_pile (ppile_t p) ;

/*
Résultat: 1 si P est vide; 0 sinon
Donnée : une pile P.
*/
int pile_vide (ppile_t p) ;

/*
Résultat: 1 si P est plein; 0 sinon
Donnée : une pile P.
*/
int pile_pleine (ppile_t p) ;

/*
Résultat: renvoie le noeud supprimé. NULL si P est vide
Donnée : une pile P.
Pré-condition: P est non null.
Post-condition: supprime l'élément au sommet de P.
Effets de bord: P est modifiée
*/
Arbre234 depiler_noeud (ppile_t p) ;

int depiler_entier (ppile_t p) ;
/*
Résultat: 1 si Erreur (pile déjà pleine); 0 sinon
Données : une pile P.
Post-condition: ajoute le noeud Pn au sommet de P.
Effets de bord: P est modifiée
*/
int empiler_noeud (ppile_t p, Arbre234 pn) ;

int empiler_entier (ppile_t p, int pn) ;
