#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tela.h"

typedef enum
{
    ouros,
    copas,
    espadas,
    paus
} naipe_t;
typedef enum
{
    as = 1,
    dois,
    tres,
    quatro,
    cinco,
    seis,
    sete,
    oito,
    nove,
    dez,
    valete = 11,
    dama,
    rei
} valor_t;

typedef enum
{
    vermelho,
    preto
} cor_t;

typedef struct
{
    naipe_t naipe;
    valor_t valor;
} carta_t;

typedef struct
{
    int n_cartas;
    carta_t cartas[52];
} pilha_t;

cor_t cor(carta_t c)
{
    if (c.naipe == ouros || c.naipe == copas)
    {
        return vermelho;
    }
    return preto;
}


void desenha(carta_t c)
{
    tela_cor_fundo(100, 100, 100);
    if (cor(c) == vermelho) tela_cor_letra(255, 0, 0);
    else tela_cor_letra(0, 0, 0);

    switch (c.valor)
    {
    case as: putchar('A'); break;
    case valete: putchar('J'); break;
    case dama: putchar('Q'); break;
    case rei: putchar('K'); break;
    default: printf("%d", c.valor);
    }
    switch (c.naipe)
    {
    case copas: printf("\u2665"); break;
    case ouros: printf("\u2666"); break;
    case espadas: printf("\u2660"); break;
    case paus: printf("\u2663"); break;
    }
}


// em uma pilha vazia, pode empilhar somente um rei
// senão, só pode empilhar uma carta de cor diferente e
// com valor um menor que o valor da carta do topo da pilha
// a primeira carta do vetor da pilha é a que está em baixo

bool pode_empilhar(carta_t c, pilha_t p)
{
    if (p.n_cartas == 0)
    {
        return c.valor == rei;
    }
    else
    {
        carta_t topo = p.cartas[p.n_cartas - 1];
        if (cor(c) == cor(topo))
            return false;

        return c.valor == topo.valor - 1;
    }
}

int main()
{
    pilha_t pilha = {2, {{copas, rei}, {valete, ouros}}};
    carta_t c1 = {10, espadas};
    carta_t c2 = {as, paus};
    if (!pode_empilhar(c1, pilha))
    {
        printf("Não pode empilhar\n");
    }
    if (pode_empilhar(c2, pilha))
    {
        printf("Pode empilhar\n");
    }
    for(naipe_t n = ouros; n <= paus; n++)
    {
        for(valor_t v = as; v <= rei; v++)
        {
            carta_t c = {n, v};
            desenha_carta(c);
            printf(" ");
        }
        printf("\n");
    }
}