#include <stdbool.h>
#include <stdio.h>
#include "tela.h"
#include <assert.h>

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
    // os números 2-10 se auto representam
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
    valor_t valor;
    naipe_t naipe;
    bool aberta;
} carta_t;
typedef struct
{
    int n_cartas;
    carta_t cartas[52];
    bool cartas_fechadas;
} pilha_t;
typedef struct
{
    pilha_t monte;
    pilha_t descarte;
    pilha_t saida[4];
    pilha_t principais[7];
} jogo_t;

cor_t cor(carta_t c)
{
    if (c.naipe == ouros || c.naipe == copas)
    {
        return vermelho;
    }
    else
    {
        return preto;
    }
}

void desenha_carta(int linha, int coluna, carta_t c)
{
    tela_lincol(linha, coluna);
    tela_cor_fundo(200, 200, 200);

    if (c.aberta)
    {
        if (cor(c) == vermelho)
            tela_cor_letra(200, 0, 0);
        else
            tela_cor_letra(0, 0, 0);
        switch (c.valor)
        {
        case as:
            putchar('A');
            break;
        case valete:
            putchar('J');
            break;
        case dama:
            putchar('Q');
            break;
        case rei:
            putchar('K');
            break;
        default:
            printf("%d", c.valor);
        }
        switch (c.naipe)
        {
        case copas:
            printf("\u2665");
            break;
        case ouros:
            printf("\u2666");
            break;
        case paus:
            printf("\u2663");
            break;
        case espadas:
            printf("\u2660");
            break;
        }
    }
    else
    {
        printf("FECHADA");
    }

    tela_cor_normal();
}

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

bool pilha_vazia(pilha_t *p)
{
    return (p->n_cartas == 0);
}

bool pilha_cheia(pilha_t *p)
{
    return (p->n_cartas == 52);
}

carta_t pilha_topo(pilha_t p)
{
    return p.cartas[p.n_cartas - 1];
}

carta_t pilha_remove_topo(pilha_t *p)
{
    assert(!pilha_vazia(p));
    carta_t c = pilha_topo(*p);
    p->n_cartas--;
    return c;
}

void empilhar(carta_t c, pilha_t *p)
{
    assert(!pilha_cheia(p));
    p->cartas[p->n_cartas++] = c;
}

void desenhar_pilha(int linha, int coluna, pilha_t p)
{
    for (int i = 0; i < p.n_cartas; i++)
    {
    }
}

void desenha_carta_vazia(int linha, int coluna)
{
    tela_lincol(linha, coluna);
    printf("VAZIO");
}

void desenhar_topo(int linha, int coluna, pilha_t p)
{
    if (p.n_cartas > 0)
    {
        carta_t cartaTopo = pilha_topo(p);
        desenha_carta(linha, coluna, cartaTopo);
    }
    else
    {
        desenha_carta_vazia(linha, coluna);
    }
}

void mover_cartas(pilha_t *origem, pilha_t *destino, int quantidade)
{
    assert(!pilha_vazia(origem) && origem->n_cartas >= quantidade);
    assert(destino->n_cartas + quantidade <= 52);

    for (int i = 0; i < quantidade; i++)
    {
        carta_t carta = pilha_remove_topo(origem);
        empilhar(carta, destino);
    }
}

void abrir_carta(pilha_t *p)
{
    if (!pilha_vazia(p))
    {
        carta_t topo_carta = pilha_topo(*p);
        if (!topo_carta.aberta)
        {
            topo_carta.aberta = true;
            p->cartas_fechadas--;
        }
    }
}

void fechar_cartas(pilha_t *p)
{
    for (int i = 0; i < p->n_cartas; i++)
    {
        p->cartas[i].aberta = false;
    }
    p->cartas_fechadas = p->n_cartas;
}

void esvaziar_pilha(pilha_t *p)
{
    p->n_cartas = 0;
    p->cartas_fechadas = 0;
}

void gerar_baralho(pilha_t *baralho)
{
    esvaziar_pilha(baralho);
    for (int n = ouros; n <= paus; n++)
    {
        for (int v = as; v <= rei; v++)
        {
            carta_t carta = {v, n, false};
            empilhar(carta, baralho);
        }
    }
}

void embaralhar_cartas(pilha_t *p)
{
    int meio = p->n_cartas / 2;
    for (int i = 0; i < meio; i++)
    {
        carta_t temp = p->cartas[i];
        p->cartas[i] = p->cartas[p->n_cartas - 1 - i];
        p->cartas[p->n_cartas - 1 - i] = temp;
    }
}

int contar_cartas(pilha_t *p)
{
    return p->n_cartas;
}

int contar_cartas_fechadas(pilha_t *p)
{
    return p->cartas_fechadas;
}

int contar_cartas_abertas(pilha_t *p)
{
    return p->n_cartas - p->cartas_fechadas;
}

carta_t obter_carta(pilha_t *p, int indice, bool *sucesso)
{
    *sucesso = (indice >= 0 && indice < p->n_cartas);
    if (*sucesso)
    {
        return p->cartas[indice];
    }
    else
    {
        carta_t carta_vazia = {0, 0, false};
        return carta_vazia;
    }
}

carta_t criar_carta(valor_t valor, naipe_t naipe)
{
    carta_t nova_carta;
    nova_carta.valor = valor;
    nova_carta.naipe = naipe;
    nova_carta.aberta = false;
    return nova_carta;
}

naipe_t obter_naipe(carta_t carta)
{
    return carta.naipe;
}

valor_t obter_valor(carta_t carta)
{
    return carta.valor;
}

cor_t obter_cor(carta_t carta)
{
    return (carta.naipe == copas || carta.naipe == ouros) ? vermelho : preto;
}

void inicializando_jogo(jogo_t *jogo)
{
    // Inicializando o monte
    esvaziar_pilha(&jogo->monte);
    gerar_baralho(&jogo->monte);
    embaralhar_cartas(&jogo->monte);

    // Inicializando minhas outras pilhas (principais e saida);
    for (int i = 0; i < 4; i++)
    {
        esvaziar_pilha(&jogo->saida[i]);
    }
    for (int i = 0; i < 7; ++i)
    {
        esvaziar_pilha(&jogo->principais[i]);
    }
}

bool pode_mover_para_saida(jogo_t *jogo, int pilha_principal_indice, carta_t carta)
{
    // verificando se o indice da pilha principal eh valida
    if (pilha_principal_indice < 0 || pilha_principal_indice >= 7)
    {
        return false;
    }

    // obtendo a pilha principal correspondente
    pilha_t *pilha_principal = &jogo->principais[pilha_principal_indice];

    // verificando se a pilha principal nao esta vazia e se s vsrta a ser movida é o topo da pilha
    if (!pilha_vazia(pilha_principal) && pilha_topo(*pilha_principal).valor == carta.valor - 1 && obter_naipe(pilha_topo(*pilha_principal)) == carta.naipe)
    {
        // verifica se a pilha de saida correspondente está vazia ou se a carta é do mesmo naipes e o valuer -1
        int saida_indice = obter_naipe(carta);
        pilha_t *pilha_saida = &jogo->saida[saida_indice];
        if (pilha_vazia(pilha_saida) || (pilha_topo(*pilha_saida).valor == carta.valor - 1))
        {
            return true;
        }
    }
    // caso nao posssa mover para pilha de saida.
    return false;
}

bool pode_mover_para_pilha_jogo(jogo_t *jogo, int pilha_principal_indice, carta_t carta)
{
    if (pilha_principal_indice < 0 || pilha_principal_indice >= 7)
    {
        return false;
    }
    pilha_t *pilha_principal = &jogo->principais[pilha_principal_indice];

    if (pilha_vazia(pilha_principal) || (pode_empilhar(carta, *pilha_principal) && carta.aberta))
    {
        return true;
    }
    return false;
}

bool pode_mover_cartas_para_pilha_principal(jogo_t *jogo, int origem_indice, int destino_indice, int quantidade)
{
    if (origem_indice < 0 || origem_indice >= 7 || destino_indice < 0 || destino_indice >= 7)
    {
        return false;
    }
    // Obtendo as pilhas principais
    pilha_t *pilha_origem = &jogo->principais[origem_indice];
    pilha_t *pilha_destino = &jogo->principais[destino_indice];

    // Verificar se há cartas suficientes na pilha de origem
    if (quantidade > pilha_origem->n_cartas)
    {
        return false;
    }

    // Verificar se as cartas podem ser movidas para a pilha de destino
    for (int i = pilha_origem->n_cartas - quantidade; i < pilha_origem->n_cartas; i++)
    {
        carta_t carta = pilha_origem->cartas[i];
        if (!pode_empilhar(carta, *pilha_destino) || !carta.aberta)
        {
            return false;
        }
    }
    // Pode mover as cartas para a pilha principal de destino
    return true;
}

bool verifica_vitoria(jogo_t *jogo)
{
    for (int i = 0; i < 4; i++)
    {
        if (jogo->saida[i].n_cartas != 13)
        {
            return false;
        }
    }
    return true;
}

bool abre_uma_carta(jogo_t *jogo)
{
    if (pilha_vazia(&jogo->monte))
    {
        return false;
    }
    carta_t carta = pilha_remove_topo(&jogo->monte);
    carta.aberta = true;
    empilhar(carta, &jogo->descarte);
    return true;
}

bool recicla_o_descarte(jogo_t *jogo)
{
    if (pilha_vazia(&jogo->monte) || pilha_vazia(&jogo->descarte))
    {
        return false;
    }
    while (!pilha_vazia(&jogo->descarte))
    {
        carta_t carta = pilha_remove_topo(&jogo->descarte);
        carta.aberta = false;
        empilhar_no_fundo(carta, &jogo->monte);
    }
    return true;
}

bool move_do_descarte_para_uma_saida(jogo_t *jogo, int saida_indice)
{
    if (saida_indice < 0 || saida_indice >= 4)
    {
        return false;
    }
    pilha_t *pilha_saida = &jogo->saida[saida_indice];
    if (pilha_vazia(&jogo->descarte))
    {
        return false;
    }
    carta_t carta = pilha_remove_topo(&jogo->descarte);
    if (pilha_vazia(pilha_saida) && carta.valor == as)
    {
        empilhar(carta, pilha_saida);
        return true;
    }
    if (pilha_topo(*pilha_saida).valor == carta.valor - 1 && obter_naipe(pilha_topo(*pilha_saida)) == carta.naipe)
    {
        empilhar(carta, pilha_saida);
        return true;
    }
    return false;
}

bool move_do_descarte_para_a_pilha_do_jogo(jogo_t *jogo, int pilha_principal_indice)
{
    if (pilha_principal_indice < 0 || pilha_principal_indice >= 7)
    {
        return false;
    }
    pilha_t *pilha_principal = &jogo->principais[pilha_principal_indice];
    if (pilha_vazia(&jogo->descarte))
    {
        return false;
    }
    carta_t carta = pilha_remove_topo(&jogo->descarte);
    if (pilha_vazia(pilha_principal) && carta.valor == rei)
    {
        empilhar(carta, pilha_principal);
        return true;
    }
    if (pode_empilhar(carta, *pilha_principal) && carta.aberta)
    {
        empilhar(carta, pilha_principal);
        return true;
    }
    return false;
}

bool move_de_uma_pilha_do_jogo_para_uma_saida(jogo_t *jogo, int pilha_principal_indice, int saida_indice)
{
    if (pilha_principal_indice < 0 || pilha_principal_indice >= 7 || saida_indice < 0 || saida_indice >= 4)
    {
        return false;
    }
    pilha_t *pilha_principal = &jogo->principais[pilha_principal_indice];
    pilha_t *pilha_saida = &jogo->saida[saida_indice];
    if (pilha_vazia(pilha_principal))
    {
        return false;
    }
    carta_t carta = pilha_topo(*pilha_principal);
    if (pilha_vazia(pilha_saida) && carta.valor == as)
    {
        empilhar(carta, pilha_saida);
        pilha_remove_topo(pilha_principal);
        return true;
    }
    if (pilha_topo(*pilha_saida).valor == carta.valor - 1 && obter_naipe(pilha_topo(*pilha_saida)) == carta.naipe)
    {
        empilhar(carta, pilha_saida);
        pilha_remove_topo(pilha_principal);
        return true;
    }
    return false;
}

bool move_de_uma_saida_para_a_pilha_do_jogo(jogo_t *jogo, int saida_indice, int pilha_principal_indice)
{
    if (pilha_principal_indice < 0 || pilha_principal_indice >= 7 || saida_indice < 0 || saida_indice >= 4)
    {
        return false;
    }
    pilha_t *pilha_principal = &jogo->principais[pilha_principal_indice];
    pilha_t *pilha_saida = &jogo->saida[saida_indice];
    if (pilha_vazia(pilha_saida))
    {
        return false;
    }
    carta_t carta = pilha_topo(*pilha_saida);
    if (pilha_vazia(pilha_principal) && carta.valor == rei)
    {
        empilhar(carta, pilha_principal);
        pilha_remove_topo(pilha_saida);
        return true;
    }
    if (pode_empilhar(carta, *pilha_principal) && carta.aberta)
    {
        empilhar(carta, pilha_principal);
        pilha_remove_topo(pilha_saida);
        return true;
    }
    return false;
}

bool move_tantas_cartas_de_uma_pilha_do_jogo_para_outra(jogo_t *jogo, int origem_indice, int destino_indice, int quantidade)
{
    if (origem_indice < 0 || origem_indice >= 7 || destino_indice < 0 || destino_indice >= 7)
    {
        return false;
    }
    pilha_t *pilha_origem = &jogo->principais[origem_indice];
    pilha_t *pilha_destino = &jogo->principais[destino_indice];
    if (quantidade > pilha_origem->n_cartas)
    {
        return false;
    }
    for (int i = pilha_origem->n_cartas - quantidade; i < pilha_origem->n_cartas; i++)
    {
        carta_t carta = pilha_origem->cartas[i];
        if (!pode_empilhar(carta, *pilha_destino) || !carta.aberta)
        {
            return false;
        }
    }
    for (int i = pilha_origem->n_cartas - quantidade; i < pilha_origem->n_cartas; i++)
    {
        carta_t carta = pilha_remove_topo(pilha_origem);
        empilhar(carta, pilha_destino);
    }
    return true;
}

bool move_cartas_de_uma_pilha_do_jogo_para_outra(jogo_t *jogo, int origem_indice, int destino_indice)
{
    if (origem_indice < 0 || origem_indice >= 7 || destino_indice < 0 || destino_indice >= 7)
    {
        return false;
    }
    pilha_t *pilha_origem = &jogo->principais[origem_indice];
    pilha_t *pilha_destino = &jogo->principais[destino_indice];
    if (pilha_vazia(pilha_origem))
    {
        return false;
    }
    int quantidade = 1; // Initialize the quantity of cards to be moved as 1
    carta_t carta = pilha_topo(*pilha_origem);
    while (pode_empilhar(carta, *pilha_destino) && carta.aberta)
    {
        quantidade++; // Increment the quantity of cards to be moved
        carta = pilha_origem->cartas[pilha_origem->n_cartas - quantidade];
    }
    if (quantidade > pilha_origem->n_cartas)
    {
        return false;
    }
    for (int i = pilha_origem->n_cartas - quantidade; i < pilha_origem->n_cartas; i++)
    {
        carta_t carta = pilha_origem->cartas[i];
        if (!pode_empilhar(carta, *pilha_destino) || !carta.aberta)
        {
            return false;
        }
    }
    for (int i = pilha_origem->n_cartas - quantidade; i < pilha_origem->n_cartas; i++)
    {
        carta_t carta = pilha_remove_topo(pilha_origem);
        empilhar(carta, pilha_destino);
    }
    return true;
}

bool realizar_jogada(jogo_t *jogo, const char *jogada)
{
    if (strlen(jogada) < 1 || strlen(jogada) > 2)
    {
        return false;
    }

    char origem = tolower(jogada[0]);
    char destino = tolower(jogada[1]);

    int origem_indice = -1;
    int destino_indice = -1;

    if (origem == 'm' || origem == 'p')
    {
        origem_indice = 0;
    }
    else if (origem >= 'a' && origem <= 'd')
    {
        origem_indice = origem - 'a' + 1;
    }
    else if (origem >= '1' && origem <= '7')
    {
        origem_indice = origem - '1' + 5;
    }
    else
    {
        return false;
    }

    if (destino == 'm' || destino == 'p')
    {
        destino_indice = 0;
    }
    else if (destino >= 'a' && destino <= 'd')
    {
        destino_indice = destino - 'a' + 1;
    }
    else if (destino >= '1' && destino <= '7')
    {
        destino_indice = destino - '1' + 5;
    }
    else
    {
        return false;
    }

    if (jogada[1] == '\0')
    {
        destino_indice = 2;
    }

    if (jogada[0] == 'f')
    {
        jogo->finalizar_jogo = true;
        return true;
    }
    else if (jogada[0] == 'm' || jogada[0] == 'p')
    {
        return move_carta_do_monte_para_pilha(jogo, destino_indice);
    }
    else
    {
        return faz_uma_jogada(jogo, origem_indice, destino_indice, 1);
    }
}

void desenha_de_local(int linha, int coluna)
{
    printf("Linha: %d, Coluna: %d\n", linha, coluna);
    printf(" _______\n");
    printf("|       |\n");
    printf("|       |\n");
    printf("|_______|\n");
}

void desenho_de_carta_fechada(int linha, int coluna)
{
    tela_lincol(linha, coluna);
    tela_cor_fundo(200, 200, 200);
    printf(" _______\n");
    printf("|       |\n");
    printf("|       |\n");
    printf("|_______|\n");
    tela_cor_normal();
}

void desenho_de_carta_aberta(int linha, int coluna, carta_t carta)
{
    tela_lincol(linha, coluna);
    tela_cor_fundo(200, 200, 200);
    if (cor(carta) == vermelho)
        tela_cor_letra(200, 0, 0);
    else
        tela_cor_letra(0, 0, 0);

    char descricao[20];
    obter_descricao_carta(carta, descricao); // Assuming obter_descricao_carta is the function from Part I

    printf("%s", descricao);

    tela_cor_normal();
}

void desenho_de_pilha_fechada(int linha, int coluna)
{
    tela_lincol(linha, coluna);
    tela_cor_fundo(200, 200, 200);
    printf(" _______\n");
    printf("|       |\n");
    printf("|       |\n");
    printf("|_______|\n");
    tela_cor_normal();
}

void desenho_de_pilha_topo(int linha, int coluna, pilha_t pilha)
{

    int n_cartas = pilha.n_cartas;
    for (int i = 0; i < n_cartas; i++)
    {

        if (pilha.vazia)
        {
            desenho_de_pilha_fechada(linha, coluna);
        }
        else
        {
            carta_t topo = pilha_topo(pilha);
            if (carta_aberta(topo))
            {
                desenho_de_carta_aberta(linha, coluna, topo);
            }
            else
            {
                desenho_de_carta_fechada(linha, coluna);
            }
        }
    }

    void desenho_de_pilha_aberta(int linha, int coluna, pilha_t pilha)
    {

        if (pilha.vazia)
        {
            desenho_de_pilha_fechada(linha, coluna);
        }
        else
        {
            for (int i = 0; i < pilha.n_cartas; i++)
            {
                carta_t carta = pilha.cartas[i];
                desenho_de_carta_aberta(linha + i, coluna, carta);
            }
        }
    }

    void desenho_compacto_de_pilha_aberta(int linha, int coluna, pilha_t pilha)
    {
        if (pilha.vazia)
        {
            desenho_de_pilha_fechada(linha, coluna);
        }
        else
        {
            int n_cartas_abertas = pilha.n_cartas - 2;
            carta_t primeira_carta = pilha.cartas[0];
            carta_t ultima_carta = pilha.cartas[pilha.n_cartas - 1];

            desenho_de_carta_fechada(linha, coluna);
            tela_lincol(linha + 1, coluna);
            printf("  (%d)", n_cartas_abertas);
            desenho_de_carta_aberta(linha + 2, coluna, primeira_carta);
            desenho_de_carta_aberta(linha + 3, coluna, ultima_carta);
        }
    }
}
void jogo()
{
    jogo_t jogo;
    inicializar_jogo(&jogo);
    while (!verifica_vitoria(&jogo))
    {
        desenhar_jogo(&jogo);
        char jogada[10];
        printf("Jogada: ");
        scanf("%s", jogada);
        realizar_jogada(&jogo, jogada);
    }
    desenhar_jogo(&jogo);
    printf("Parabéns, você ganhou!\n");
}

void desenhar_jogo(jogo_t *jogo)
{
    tela_limpar();
    tela_cor_normal();
    tela_texto(0, 0, 20, preto, "Monte");
    desenho_de_pilha_topo(0, 0, jogo->monte);
    tela_texto(0, 0, 20, preto, "Descarte");
    desenho_de_pilha_topo(0, 0, jogo->descarte);
    tela_texto(0, 0, 20, preto, "Saída");
    for (int i = 0; i < 4; i++)
    {
        desenho_de_pilha_topo(0, 0, jogo->saida[i]);
    }
    tela_texto(0, 0, 20, preto, "Principais");
    for (int i = 0; i < 7; i++)
    {
        desenho_de_pilha_topo(0, 0, jogo->principais[i]);
    }
}

int main()
{
    jogo_t meu_jogo;

    // sempre que eu quiser inicializar um novo jogo eu chamo essa função
    inicializando_jogo(&meu_jogo);

    pilha_t pilha = {2, {{rei, copas}, {valete, ouros}}};

    carta_t nova_carta = criar_carta(7, ouros);

    empilhar(nova_carta, &pilha);

    desenho_de_pilha_topo(0, 0, pilha);

    return 0;
}
