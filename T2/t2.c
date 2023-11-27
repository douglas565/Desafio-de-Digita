// l123b - t1 (acerte a letra)
// programa para adivinhar uma string gerada aleatoriamente
// Douglas Ramos Charqueiro
// 2023-09-29

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "tecla.h"
#include "tela.h"

#define TEMPO 100
#define TOTAL_DE_PALAVRAS 10

typedef struct
{
  int pos_horizontal;
  int pos_vertical;
  int hora_ativacao;
  int tempo_digitacao;
  bool ativa;
} palavras;

palavras vetPalavras[10];

void reorganiza_matriz(char palavras[10][16], int index_acertado);
void remove_letra(char v[], int pos);
void apresentacao();
void encerramento();
void jogo();
bool quer_jogar_de_novo();
void preenche_vet(char v[], int min_len, int max_len);
void mostra_vet(char v[]);
void espera_enter();
void popular_matriz(char palavras[10][16]);

int main()
{

  srand(time(0));

  apresentacao();
  // gera um tamanho aleatorio
  //  for que vai gera uma letra aleatoria
  do
  {
    tela_ini();
    tecla_ini();
    jogo();
    tecla_fim();
    tela_fim();

  } while (quer_jogar_de_novo());

  encerramento();
}

int ler_banco_de_palavras(char palavras[920][16], const char *nome_do_arquivo)
{
  FILE *arquivo = fopen(nome_do_arquivo, "r");
  if (arquivo == NULL)
  {
    perror("Erro ao abrir o arquivo");
    tela_atualiza();
    return 0;
  }

  char linha[16];
  int i = 0;
  while (fgets(linha, sizeof(linha), arquivo) != NULL && i < 920)
  {
    // Remover o caractere de nova linha se estiver presente
    size_t len = strlen(linha);
    if (len > 0 && linha[len - 1] == '\n')
    {
      linha[len - 1] = '\0';
    }

    // Converter para minúsculas e filtrar palavras não válidas
    int valida = 1;
    for (int j = 0; linha[j] != '\0' && j < 15; j++)
    {
      if (linha[j] >= 'A' && linha[j] <= 'Z')
      {
        linha[j] = linha[j] - 'A' + 'a';
      }
      else if (linha[j] < 'a' || linha[j] > 'z')
      {
        valida = 0;
        break;
      }
    }
    if (valida)
    {
      strcpy(palavras[i], linha);
      i++;
    }
  }

  fclose(arquivo);
  return i; // Retorna o número de palavras lidas
}

void Sorteia_palavra(char v[], int min_len, int max_len)
{
  int len = min_len + rand() % (max_len - min_len + 1);
  for (int i = 0; i < len; i++)
  {
    v[i] = 'a' + rand() % 26;
  }
  v[len] = '\0'; // Termina a string com o caractere nulo
}

void popular_matriz(char palavras[10][16])
{
  char banco_de_palavras[920][16];
  int num_palavras = ler_banco_de_palavras(banco_de_palavras, "palavras.txt");

  for (int i = 0; i < 10; i++)
  {
    int indice_aleatorio = rand() % num_palavras;
    strcpy(palavras[i], banco_de_palavras[indice_aleatorio]);
    printf("Palavra %d: %s\n", i + 1, palavras[i]); // Imprime cada palavra
    tela_atualiza();
  }
}

void mostra_matriz(char palavras[10][16], double tempo_restante, int indice, double inicio)
{
  tela_limpa();
  int i;
  int lin;
  int col;
  int alt = tela_nlin();
  // lacço principal para cada palavra
  for (i = 0; i < 10; i++)
  {
    // testa se a palavra está vazia, caso sim não printa
    if (palavras[i][0] == '\0')
    {
      continue;
    }
    if (i == indice)
    {
      tela_cor_letra(0, 255, 0);
      tela_cor_fundo(143, 188, 143);
    }
    else
    {
      tela_cor_letra(0, 206, 209);
      tela_cor_fundo(119, 136, 153);
    }

    if (TEMPO - tempo_restante >= vetPalavras[i].hora_ativacao)
    {
      lin = vetPalavras[i].pos_vertical;
      col = vetPalavras[i].pos_horizontal;
      tela_lincol(lin, col);
      printf("%s", palavras[i]);
      vetPalavras[i].pos_vertical = 2 + alt * ((tela_relogio() - inicio) - vetPalavras[i].hora_ativacao) / vetPalavras[i].tempo_digitacao;
      vetPalavras[i].ativa = true;
    }
  }
  tela_cor_normal();
  tela_lincol(0, 0);
  printf("digite as palavras conforme aparecem:");
  tela_atualiza();
}

int encontra_palavra_com_letra(char palavras[10][16], char letra, int posicao)
{
  for (int i = 0; i < 10; i++)
  { // varre todas a linhas da matriz
    if (palavras[10][16] == letra && vetPalavras[i].ativa == true)
    { // testa se a letra digitada é igual a primeira letra de cada linha, caso for retorna a linha da matriz onde está a palavra
      return i;
    }
  }
  return -1;
}

bool testa_tempoDigitacao(char palavras[][16], double inicio)
{
  for (int i = 0; i < 10; i++)
  {

    if ((vetPalavras[i].tempo_digitacao + vetPalavras[i].hora_ativacao <= (tela_relogio() - inicio)) && palavras[i][0] != '\0')
    {
      return true;
    }
  }
  return false;
}
void jogo()
{
  char palavras[10][16];
  char Selecionadas[10][16];
  int indice = -1;
  int qtdePalavras = 10;
  double inicio = tela_relogio(); // Fix: Define the variable "inicio" and initialize it with the current time
  double tempo_restante;          // Fix: Declare the variable "tempo_restante"

  while (true)
  {
    tempo_restante = TEMPO - (tela_relogio() - inicio); // Fix: Calculate the remaining time
    if (testa_tempoDigitacao(palavras, inicio) || qtdePalavras == 0)
    {
      break;
    }
    mostra_matriz(palavras, tempo_restante, indice, inicio);
    char letra = tecla_le_char();
    if (letra != '\0')
    {
      if (indice < 0)
      {
        indice = encontra_palavra_com_letra(palavras, letra, indice);
      }
      if (indice >= 0)
      {
        remove_letra(palavras[indice], letra);
        if (palavras[indice][0] == '\0')
        {
          qtdePalavras--;
          indice = -1;
        }
      }
    }
  }
}

void apresentacao()
{
  printf("Você deve digitar as palavras que aparecerão na tela.\n");
  printf("A ordem nao é importante.\n");
  printf("Tecle <enter> para iniciar. ");
  tela_atualiza();
  espera_enter();
}

void encerramento()
{
  printf("Até a próxima.\n");
  tela_atualiza();
}

bool quer_jogar_de_novo()
{
  espera_enter();

  printf("Digite 's' para jogar de novo: ");
  tela_atualiza();
  while (true)
  {
    char c = getchar();
    if (c == '\n')
    {
      return false;
    }
    else if (c == 's' || c == 'S')
    {
      espera_enter();
      return true;
    }
  }
}

void remove_letra(char v[], int pos)
{
  if (pos >= 0)
  {
    int len = strlen(v);
    for (int i = pos; i < len - 1; i++)
    {
      v[i] = v[i + 1];
    }
    v[len - 1] = '\0'; // Define o último caractere como nulo
  }
}

void reorganiza_matriz(char palavras[10][16], int index_acertado)
{
  for (int i = index_acertado; i < 9; i++)
  {
    int j;
    for (j = 0; palavras[i + 1][j] != '\0'; j++)
    {
      palavras[i][j] = palavras[i + 1][j];
    }
    palavras[i][j] = '\0'; // Termina a string atual após a cópia.
  }
  // Limpa a última posição da matriz após mover todas as palavras.
  for (int j = 0; palavras[9][j] != '\0'; j++)
  {
    palavras[9][j] = '\0';
  }
}

void processa_Palavra(char palavras[][16], int indice, double inicio)
{
  int larg = tela_ncol();
  int alt = tela_nlin();
  int tamPalavra;
  for (int i = 0; i < 10; i++)
  {
    vetPalavras[i].ativa = false;
    vetPalavras[i].hora_ativacao = rand() % 21;
    vetPalavras[i].tempo_digitacao = 5 + rand() % 26;
    tamPalavra = strlen(palavras[i]);
    vetPalavras[i].pos_horizontal = (larg - tamPalavra) * (rand() % 101) / 100;
    vetPalavras[i].pos_vertical = 2 + alt * ((tela_relogio() - inicio) - vetPalavras[i].hora_ativacao) / vetPalavras[i].tempo_digitacao;
  }
}
