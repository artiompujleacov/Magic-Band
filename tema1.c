/*PUJLEACOV ARTIOM 315CB*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 20
#define MLC 15
#define MRC 16
#define W 6
#define IR 13
#define IL 12

// Declaratia celulei pentru lista
typedef struct cel {
  char info;
  struct cel *urm;
  struct cel *ant;
} Celula;

// Declaratia listei care contine degetul si santinela,degetul se numeste
// "curent" si santinela "cap"
typedef struct banda {
  Celula *cap;
  Celula *curent;
} * BandaMagica;

// Declaratia celulei pentru coada
typedef struct celula {
  char *comanda;
  struct celula *urm;
} * TCelula, *TLista;

typedef struct coada {
  TCelula inc, sf;
} TCoada;

// Declaratia celulei pentru stiva
typedef struct nod {
  Celula *pozitie;
  struct nod *next;
} Nod;

// Declaratia stivei
typedef struct stiva {
  Nod *varf;
} * Stiva;

// Functia care initializeaza Banda,returneaza o lista cu 2 celule,santinela si
// nod curent
BandaMagica AlocBanda() {
  BandaMagica aux = (BandaMagica)malloc(sizeof(struct banda));
  aux->cap = (Celula *)malloc(sizeof(struct cel));
  aux->curent = (Celula *)malloc(sizeof(struct cel));
  if (!aux) {
    return NULL;
  }
  aux->cap->info = ' ';
  aux->cap->ant = NULL;
  aux->cap->urm = aux->curent;
  aux->curent->info = '#';
  aux->curent->urm = NULL;
  aux->curent->ant = aux->cap;
  return aux;
}

// Functia care aloca o celula care urmeaza sa fie adaugata in lista
Celula *AlocCelula(BandaMagica Banda) {
  Celula *new = (Celula *)malloc(sizeof(struct cel));
  new->ant = new->urm = NULL;
  new->info = '#';
  return new;
}

// Functia care adauga o celula alocata in lista
void AdaugCelula(BandaMagica Banda) {
  Celula *new = AlocCelula(Banda);
  new->ant = Banda->curent;
  if (Banda->curent->urm == NULL) {
    new->urm = NULL;
  } else {
    new->urm = Banda->curent->urm;
  }
  Banda->curent->urm = new;
  Banda->curent = new;
}

// Functia care initializeaza o coada goala
TCoada *AlocaCoada() {
  TCoada *c = (TCoada *)malloc(sizeof(struct coada));
  if (!c)
    return NULL;
  c->inc = c->sf = NULL;
  return c;
}

// Functia care adauga o instructiune in coada,primeste un string ca parametru
// si il copiaza in coada
int Adauga_in_coada(TCoada *c, char *instructiune) {
  TLista aux;
  aux = (TLista)malloc(sizeof(struct celula));
  if (!aux)
    return 0;
  int lungime = strlen(instructiune) + 1;
  aux->comanda = malloc(lungime * sizeof(char));
  strcpy(aux->comanda, instructiune);
  aux->urm = NULL;
  if (c->sf != NULL)
    c->sf->urm = aux;
  else
    c->inc = aux;
  c->sf = aux;
  return 1;
}

// Functia care extrage o instructiune din coada si goleste celula in care se
// afla aceasta
int Extrage_din_coada(TCoada *c) {
  if (c->inc == NULL)
    return 0;
  TLista p = c->inc;
  c->inc = p->urm;
  if (c->inc == NULL)
    c->sf = NULL;
  free(p->comanda);
  free(p);
  return 1;
}

// Functia care initializeaza o stiva goala
Stiva AlocaStiva() {
  Stiva aux = (Stiva)malloc(sizeof(struct stiva));
  aux->varf = NULL;
  return aux;
}

// Functia care adauga o celula in stiva,in celula se adauga pointerul la
// pozitia curenta in banda
void adauga_in_stiva(Stiva s, BandaMagica Banda) {
  Nod *aux = (Nod *)malloc(sizeof(struct nod));
  aux->pozitie = Banda->curent;
  aux->next = s->varf;
  s->varf = aux;
}

// Functia care extrage ultima celula din stiva si o goleste
void extrage_din_stiva(Stiva s) {
  Nod *aux = s->varf;
  aux->next = s->varf->next;
  s->varf = aux->next;
  free(aux);
}
// Functia tip UPDATE-MOVE_RIGHT
void Move_Right(BandaMagica Banda) {
  if (Banda->curent->urm == NULL) {
    AdaugCelula(Banda);
  } else {
    Banda->curent = Banda->curent->urm;
  }
}

// Functia tip UPDATE-MOVE_LEFT
void Move_Left(BandaMagica Banda) { Banda->curent = Banda->curent->ant; }

// Functia tip UPDATE-WRITE
void write_curent(BandaMagica Banda, char caracter) {
  Banda->curent->info = caracter;
}

// Functia tip UPDATE-INSERT_RIGHT,care adauga o celula in dreapta
// degetului,insereaza caracterul primit ca parametru
void insert_right(BandaMagica Banda, char caracter) {
  Celula *new = AlocCelula(Banda);
  if (Banda->curent->urm == NULL) {
    new->urm = NULL;
    new->ant = Banda->curent;
    new->info = caracter;
    Banda->curent->urm = new;
    Banda->curent = new;
  } else {
    new->info = caracter;
    new->ant = Banda->curent;
    new->urm = Banda->curent->urm;
    Banda->curent->urm->ant = new;
    Banda->curent->urm = new;
    Banda->curent = new;
  }
}

// Functia de tip UPDATE-MOVE_RIGHT_CHAR care se deplaseaza in dreapta pana nu
// gaseste caracterul primit ca parametru
void Move_Right_Char(BandaMagica Banda, char caracter) {
  while (Banda->curent) {
    if (Banda->curent->info == caracter) {
      break;
    } else if (Banda->curent->urm == NULL) {
      insert_right(Banda, '#');
      break;
    } else {
      Move_Right(Banda);
    }
  }
}

// Functia de tip UPDATE-MOVE_LEFT_CHAR care se deplaseaza in stanga pana nu
// gaseste caracterul primit ca parametru
void Move_Left_Char(BandaMagica Banda, char caracter, FILE *fp) {
  Celula *aux = Banda->curent;
  while (Banda->curent) {
    if (Banda->curent->info == caracter) {
      break;
    } else if (Banda->curent == Banda->cap) {
      fprintf(fp, "ERROR\n");
      Banda->curent = aux;
      break;
    } else {
      Move_Left(Banda);
    }
  }
}

// Functia de tip UPDATE-INSERT_LEFT care insereaza o celula in stanga de la
// elementul curent in banda
void insert_left(BandaMagica Banda, char caracter) {
  Celula *new = AlocCelula(Banda);
  new->info = caracter;
  new->ant = Banda->curent->ant;
  new->urm = Banda->curent;
  Banda->curent->ant->urm = new;
  Banda->curent->ant = new;
  Banda->curent = new;
}

// Functia SHOW care afiseaza toata banda
void show(BandaMagica Banda, FILE *fp) {
  Celula *iter = Banda->cap->urm;
  while (iter) {
    if (iter != Banda->curent) {
      fprintf(fp, "%c", iter->info);
    } else {
      fprintf(fp, "|%c|", iter->info);
    }
    iter = iter->urm;
  }
  fprintf(fp, "\n");
}

// Functia SHOW_CURRENT care afiseaza elementul curent din banda
void show_current(BandaMagica Banda, FILE *fp) {
  fprintf(fp, "%c\n", Banda->curent->info);
}

// Functia care executa operatia undo,ooperand cu stiva undo si banda
void executie_undo(BandaMagica Banda, Stiva undo, Stiva redo) {
  if (undo->varf) {
    adauga_in_stiva(redo, Banda);
    Banda->curent = undo->varf->pozitie;
    extrage_din_stiva(undo);
  }
}

// Functia care executa operatia redo,operand cu stiva redo si banda
void executie_redo(BandaMagica Banda, Stiva redo, Stiva undo) {
  if (redo->varf) {
    adauga_in_stiva(undo, Banda);
    Banda->curent = redo->varf->pozitie;
    extrage_din_stiva(redo);
  }
}

// Functia care executa operatia de executie a comenzilor din coada
void executie_coada(BandaMagica Banda, TCoada *coada_execute, Stiva undo,
                    Stiva redo, FILE *fp) {
  if (coada_execute->inc) {
    if (strcmp(coada_execute->inc->comanda, "MOVE_RIGHT") == 0) {
      adauga_in_stiva(undo, Banda);
      Move_Right(Banda);
      Extrage_din_coada(coada_execute);
    } else if (strcmp(coada_execute->inc->comanda, "MOVE_LEFT") == 0) {
      if (Banda->curent->ant != Banda->cap) {
        adauga_in_stiva(undo, Banda);
        Move_Left(Banda);
      }
      Extrage_din_coada(coada_execute);
    } else if (strcmp(strtok(coada_execute->inc->comanda, " "),
                      "MOVE_LEFT_CHAR") == 0) {
      Move_Left_Char(Banda, coada_execute->inc->comanda[MLC], fp);
      Extrage_din_coada(coada_execute);
    } else if (strcmp(strtok(coada_execute->inc->comanda, " "),
                      "MOVE_RIGHT_CHAR") == 0) {
      Move_Right_Char(Banda, coada_execute->inc->comanda[MRC]);
      Extrage_din_coada(coada_execute);
    } else if (strcmp(strtok(coada_execute->inc->comanda, " "), "WRITE") == 0) {
      write_curent(Banda, coada_execute->inc->comanda[W]);
      Extrage_din_coada(coada_execute);
    } else if (strcmp(strtok(coada_execute->inc->comanda, " "),
                      "INSERT_RIGHT") == 0) {
      insert_right(Banda, coada_execute->inc->comanda[IR]);
      Extrage_din_coada(coada_execute);
    } else if (strcmp(strtok(coada_execute->inc->comanda, " "),
                      "INSERT_LEFT") == 0) {
      if (Banda->curent->ant != Banda->cap) {
        insert_left(Banda, coada_execute->inc->comanda[IL]);
      } else {
        fprintf(fp, "ERROR\n");
      }
      Extrage_din_coada(coada_execute);
    }
  }
}

// Functia care citeste comenzile din fisier si le executa in dependenta de tip
void citire_comenzi_din_fisier(BandaMagica Banda, TCoada *coada_execute,
                               Stiva undo, Stiva redo) {
  FILE *input = fopen("tema1.in", "r");
  FILE *output = fopen("tema1.out", "w");
  int nr_comenzi = 0;
  char *buffer = (char *)malloc(MAX * sizeof(char));
  size_t max = MAX;
  fscanf(input, "%d\n", &nr_comenzi);
  for (int i = 0; i < nr_comenzi; i++) {
    getline(&buffer, &max, input);
    int length = strlen(buffer) + 1;
    char *comenzi = malloc(length * sizeof(char));
    strcpy(comenzi, buffer);
    if (strcmp(strtok(buffer, "\n"), "MOVE_RIGHT") == 0) {
      Adauga_in_coada(coada_execute, buffer);
    } else if (strcmp(strtok(buffer, "\n"), "MOVE_LEFT") == 0) {
      Adauga_in_coada(coada_execute, buffer);
    } else if (strcmp(strtok(buffer, " "), "MOVE_LEFT_CHAR") == 0) {
      Adauga_in_coada(coada_execute, comenzi);
    } else if (strcmp(strtok(buffer, " "), "MOVE_RIGHT_CHAR") == 0) {
      Adauga_in_coada(coada_execute, comenzi);
    } else if (strcmp(strtok(buffer, " "), "WRITE") == 0) {
      Adauga_in_coada(coada_execute, comenzi);
    } else if (strcmp(strtok(buffer, " "), "INSERT_RIGHT") == 0) {
      Adauga_in_coada(coada_execute, comenzi);
    } else if (strcmp(strtok(buffer, " "), "INSERT_LEFT") == 0) {
      Adauga_in_coada(coada_execute, comenzi);
    } else if (strcmp(strtok(buffer, " "), "EXECUTE") == 0) {
      executie_coada(Banda, coada_execute, undo, redo, output);
    } else if (strcmp(buffer, "UNDO") == 0) {
      executie_undo(Banda, undo, redo);
    } else if (strcmp(buffer, "REDO") == 0) {
      executie_redo(Banda, redo, undo);
    } else if (strcmp(buffer, "SHOW") == 0) {
      show(Banda, output);
    } else if (strcmp(buffer, "SHOW_CURRENT") == 0) {
      show_current(Banda, output);
    }
    free(comenzi);
  }
  free(buffer);
  fclose(input);
  fclose(output);
}

// Functia de dezalocare a memoriei pentru banda
void distruge_banda(BandaMagica Banda) {
  Celula *aux = Banda->cap;
  while (aux != NULL) {
    Celula *urm = aux->urm;
    free(aux);
    aux = urm;
  }
  free(Banda);
}

// Functia de dezalocare a memoriei pentru coada
void distruge_coada(TCoada *c) {
  TLista aux = c->inc;
  while (aux != NULL) {
    TLista urm = aux->urm;
    free(aux->comanda);
    free(aux);
    aux = urm;
  }
  free(c);
}

// Functia de dezalocare a memoriei pentru stiva
void distruge_stiva(Stiva s) {
  Nod *aux = s->varf;
  while (aux != NULL) {
    Nod *urm = aux->next;
    free(aux);
    aux = urm;
  }
  free(s);
}

// Main-ul in care apelez functiile de initializare si citire,mai amanuntit am
// descris totul in README
int main() {
  BandaMagica Banda = AlocBanda();
  Stiva stiva_undo = AlocaStiva(), stiva_redo = AlocaStiva();
  TCoada *coada_execute = AlocaCoada();
  citire_comenzi_din_fisier(Banda, coada_execute, stiva_undo, stiva_redo);
  distruge_coada(coada_execute);
  distruge_banda(Banda);
  distruge_stiva(stiva_undo);
  distruge_stiva(stiva_redo);
  return 0;
}
