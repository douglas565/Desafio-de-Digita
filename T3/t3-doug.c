// Programa de exemplo de uso de typedef, struct e enum
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "tela.h"

// enums para dar nomes a valores constantes, de forma organizada
typedef enum {
  ouros,
  copas,
  espadas,
  paus
} naipe_t;

typedef enum { 
  as = 1,
  // os números 2-10 se auto representam
  valete = 11,
  dama,
  rei
} valor_t;

typedef enum {
  vermelho,
  preto
} cor_t;

// registro que representa uma carta de baralho
typedef struct {
  valor_t valor;
  naipe_t naipe;
  bool aberta;
} carta_t;

// registro que representa uma pilha de cartas
typedef struct {
  int n_cartas;
  carta_t cartas[52];
} pilha_t;

// retorna a cor de uma carta
cor_t cor(carta_t c)
{
  if (c.naipe == ouros || c.naipe == copas) {
    return vermelho;
  } else {
    return preto;
  }
}

void desenha_carta(carta_t c)
{
  tela_cor_fundo(200, 200, 200);
  if (cor(c) == vermelho) tela_cor_letra(200, 0, 0);
  else tela_cor_letra(0, 0, 0);
  switch (c.valor) {
    case as:     putchar('A'); break;
    case valete: putchar('J'); break;
    case dama:   putchar('Q'); break;
    case rei:    putchar('K'); break;
    default:     printf("%d", c.valor);
  }
  switch (c.naipe) {
    // peguei o código dos símbolos em https://www.unicode.org/charts/PDF/U2600.pdf
    case copas:   printf("\u2665"); break;
    case ouros:   printf("\u2666"); break;
    case paus:    printf("\u2663"); break;
    case espadas: printf("\u2660"); break;
  }
  tela_cor_normal();
}


// teste se a carta c pode ser empilhada na pilha p, no jogo "solitaire"
// em uma pilha vazia, pode-se empilhar somente um rei
// senão, só pode ser empilhada uma carta de cor diferente e
//   com valor um menor que o valor da carta que está do topo
// a primeira carta do vetor da pilha é a que está embaixo
bool pode_empilhar(carta_t c, pilha_t p)
{
  if (p.n_cartas == 0) {
    return c.valor == rei;
  } else {
    carta_t topo = p.cartas[p.n_cartas - 1];
    if (cor(c) == cor(topo)) return false;
    return c.valor == topo.valor - 1;
  }
}

bool pilha_vazia(pilha_t *p)
{

    return p->n_cartas == 0;

}

bool pilha_cheia(pilha_t *p)
{
    return p->n_cartas == 52;
}

void empilha_carta(carta_t c, pilha_t *p)
{
    p->cartas[p->n_cartas] = c;
    p->n_cartas++;
}

void retona_top(pilha_t *p, carta_t *c)
{
    *c = p->cartas[p->n_cartas - 1];
}

carta_t remove_do_topo(pilha_t *p)
{
  assert(!pilha_vazia(p));
   carta_t c = p->cartas[p->n_cartas - 1];
    p->n_cartas--;
  return c;
}

void move_cartas_entre_pilhas(pilha_t *p1, pilha_t *p2, int n)
{
  assert(p1->n_cartas >= n);
  assert(p2->n_cartas + n <= 52);
  
  for (int i = 0; i < n; i++) {
    empilha_carta(remove_do_topo(p1), p2);
  }
}


void abre_card_no_top_da_pilha(pilha_t *p)
{
  assert(!pilha_vazia(p));
  p->cartas[p->n_cartas - 1].aberta = true;
}
 

void abre_cartas_no_topo(pilha_t *p, int n)
{
  assert(p->n_cartas >= n);
  for (int i = 0; i < n; i++) {
    abre_card_no_top_da_pilha(p);
  }
}



void descricao_carta(carta_t c, char *descricao)
{
  char *valores[] = {"", "A", "2", "3", "4", "5", "6", "7", "8", "9",
                     "10", "J", "Q", "K"};
  char *naipes[] = {"", "ouros", "copas", "espadas", "paus"};
  sprintf(descricao, "%s de %s", valores[c.valor], naipes[c.naipe]);
}

 

int main(int argc, char *argv[])
{
  // inicializa algumas variáveis para testar a função pode_empilhar
  pilha_t pilha = { 2, {{rei, copas}, {valete, ouros}}};
  pilha_t pilhav = { 0 };
  carta_t c1 = { 10, espadas };
  carta_t c2 = { as, paus };

if(pilha_vazia(&pilha)) printf("pilha vazia\n");
if(!pilha_vazia(&pilhav)) printf("pilha não vazia\n");

  if (!pode_empilhar(c1, pilha)) printf("deveria poder empilhar c1\n");
  if (pode_empilhar(c2, pilha)) printf("não deveria poder empilhar c2\n");
  // desenha todas as cartas
  for (naipe_t n = ouros; n <= paus; n++) {
    for (valor_t v = as; v <= rei; v++) {
      carta_t c = { v, n };
      desenha_carta(c);
      printf("  ");
    }
    printf("\n");
  } 
}