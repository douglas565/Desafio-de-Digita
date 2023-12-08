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

#define TEMPO 20
#define TOTAL_DE_PALAVRAS 10
#define ponts_max 100
#define descont_l_errada 10
#define ranking 3

typedef struct
{
  int pontos;
  char identificador[16];

} recorde;

typedef struct
{
  float pos_horizontal;
  float pos_vertical;
  float hora_ativacao;
  float tempo_digitacao;
  char palavra[16];
  bool ativa;
} palavras;

palavras vetPalavras[10];
recorde recordes[3];

void reorganiza_matriz(char palavras[10][16], int index_acertado);
void remove_letra(char v[], int pos);
void apresentacao();
void encerramento();
void mostrar_tela(double tempo_restante, int indice, double inicio, int ponto);
bool quer_jogar_de_novo();
void preenche_vet(char v[], int min_len, int max_len);
void espera_enter();
void popular_matriz();
void Sorteia_palavra(char v[], int min_len, int max_len);
int encontra_palavra_com_letra(char letra, int posicao);
void processa_Palavra(palavras palavrasJogo[], double inicio);
bool testa_tempoDigitacao(palavras palavrasJogo[], double inicio);
void jogo();

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

void salvar_recorde(recorde recordes[3])
{
  FILE *arquivo = fopen("ranking.txt", "w");
  if (arquivo != NULL)
  {
    for (int i = 0; i < 3; i++)
    {
      fprintf(arquivo, "%s:%d\n", recordes[i].identificador, recordes[i].pontos);
    }
  }

  fclose(arquivo);
}

void popular_matriz()
{
  char banco_de_palavras[920][16];
  int num_palavras = ler_banco_de_palavras(banco_de_palavras, "palavras.txt");

  for (int i = 0; i < 10; i++)
  {
    int indice_aleatorio = rand() % num_palavras;
    strcpy(vetPalavras[i].palavra, banco_de_palavras[indice_aleatorio]);
  }
}

void atualizar_recorde(int pontos, char identificador[16], recorde recordes[3])
{
  // Atualiza os recordes se necessário
  for (int i = 0; i < 3; i++)
  {
    if (pontos > recordes[i].pontos)
    {
      for (int j = 2; j >= i; j--)
      {
        recordes[j] = recordes[j - 1];
      }
      recordes[i].pontos = pontos;
      strcpy(recordes[i].identificador, identificador);
      break;
    }
  }
  // Salva os recordes no arquivo
  salvar_recorde(recordes);
}

// Função para carregar os recordes do arquivo
void carregar_recorde(recorde recordes[3])
{

  FILE *file;
  file = fopen("ranking.txt", "r");

  char tLinha[100], ponto[10], *palavra;
  int i, tam = 0, count = 0, result = 0;
  int linha = 0;
  if(file == NULL){
      while (fgets(tLinha, 100, file))
      {
        tam = 0;
        count = 0;
        palavra = strchr(tLinha, ':');
        if (palavra != NULL)
        {
          for (i = 0; tLinha[i] != ':'; i++)
          {
            recordes[linha].identificador[i] = tLinha[i];
          }
          for(i; tLinha[i] != '\0'; i++){
            ponto[tam] = tLinha[i];
          }
          recordes[linha].pontos = *(ponto -1 ) - '0';
        }
        linha++;
        // printf("%s\n", tLinha);
      }
  }

  fclose(file);
}

void mostrar_tela(double tempo_restante, int indice, double inicio, int pontos)
{
  // tela_limpa();
  int i;
  int lin;
  int col;
  int alt = tela_nlin();
  // lacço principal para cada palavra
  for (i = 0; i < TOTAL_DE_PALAVRAS; i++)
  {
    // testa se a palavra está vazia, caso sim não printa
    if (vetPalavras[i].palavra[0] == '\0')
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
      printf("%s", vetPalavras[i].palavra);
      vetPalavras[i].pos_vertical = 2 + alt * ((tela_relogio() - inicio) - vetPalavras[i].hora_ativacao) / vetPalavras[i].tempo_digitacao;
      vetPalavras[i].ativa = true;
    }
  }
  tela_cor_normal();
  tela_lincol(0, 0);
  printf("digite as palavras conforme aparecem:");
  tela_atualiza();

  tela_cor_letra(0, 205, 10);
  tela_lincol(tela_nlin(), tela_ncol() / 2 - 5 / 2);
  printf("Pontos: %d", pontos); // Mostra a pontuação na tela
  tela_atualiza();
}

int encontra_palavra_com_letra(char letra, int posicao)
{
  for (int i = 0; i < TOTAL_DE_PALAVRAS; i++)
  { // varre todas a linhas da matriz
    if (vetPalavras[i].palavra[0] == letra && vetPalavras[i].ativa == true)
    {
      return i;
    }
  }
  return -1;
}

float calcular_pontos(double tempo_atual, double tempo_anterior)
{
  float tempo_diff = tempo_atual - tempo_anterior;
  if (tempo_diff >= 1.0)
  {
    return 1.0;
  }
  else
  {
    return ponts_max * (1 - tempo_diff);
  }
}
void pontuacao(double *t0, double t1, int *pontos)
{
  double dt = t1 - (*t0);
  if (dt >= 1)
  {
    *pontos += 1;
  }
  else
  {
    *pontos += 100 * (1 - dt);
  }
  *t0 = t1;
}
void errou(int *pontos)
{
  *pontos = (*pontos) - 1;
  if (*pontos < 0)
  {
    *pontos = 0;
  }
}

void mostrar_recordes_final_de_jogo(recorde recordes[3])
{
  // tela_limpa();
  tela_lincol(tela_nlin() / 2, tela_ncol() / 2);
  printf("Recordes:\n");
  for (int indice = 0; indice < 3; indice++)
  {
    tela_lincol(tela_nlin() / 2 + indice + 1, tela_ncol() / 2);
    printf("%s: %d\n", recordes[indice].identificador, recordes[indice].pontos);
  }

  // for (int indice = 0; indice < 3; indice++)
  // {
  //   for (int j = indice + 1; j < 3; j++)
  //   {
  //     if (recordes[indice].pontos < recordes[j].pontos)
  //     {
  //       recorde aux = recordes[indice];
  //       recordes[indice] = recordes[j];
  //       recordes[j] = aux;
  //     }
  //   }
    // tela_atualiza();
  // }
}

void jogo()
{
  int quantidade_de_palavras_acertadas = 0;
  int quantidade_de_palavras = 10; // Novo controle para a quantidade de palavras na matriz.

  popular_matriz(vetPalavras);
  long t0 = tela_relogio();
  processa_Palavra(vetPalavras, t0);

  carregar_recorde(recordes);

  double resta;
  double tempo = tela_relogio();
  double inicio = tela_relogio();

  int pontos = 0;
  int palavra_selecionada = -1; // Nenhuma palavra selecionada inicialmente
  char nome[16];
  while (quantidade_de_palavras_acertadas <= TOTAL_DE_PALAVRAS)
  {
    // printf("%d  %d\n", quantidade_de_palavras_acertadas, TOTAL_DE_PALAVRAS);
    for(int i=0; i < 3; i++){
      printf("%s  %d\n", recordes[i].identificador, recordes[i].pontos);
    }

    resta = TEMPO - (tela_relogio() - t0);
    mostrar_tela(resta, palavra_selecionada, t0, pontos); // Mostra todas as palavras na tela

    char letra = tecla_le_char();
    char letraTemp;
    printf("%c\n", letra);
    printf("%c\n", vetPalavras[palavra_selecionada].palavra[0]);

    if (palavra_selecionada == -1 && letra != 0)
    {
      palavra_selecionada = encontra_palavra_com_letra(letra, 0);
    }
    if (palavra_selecionada > -1)
    {
      // Procura a letra na palavra selecionada e a remove se encontrada
      int letra_encontrada = 0;
      for (int i = 0; i < TOTAL_DE_PALAVRAS && vetPalavras[palavra_selecionada].palavra[0] != '\0'; i++)
      {
        if (vetPalavras[palavra_selecionada].palavra[0] == letra)
        {
          letraTemp = letra;
          remove_letra(vetPalavras[palavra_selecionada].palavra, i);
          letra_encontrada = 1;
          pontuacao(&tempo, tela_relogio(), &pontos);
          break;
        }
        else if (letra != 0)
        {
          errou(&pontos);
        }
      }

      // Verifica se a palavra foi toda acertada
      if (vetPalavras[palavra_selecionada].palavra[0] == '\0')
      {
        quantidade_de_palavras_acertadas++;
        letra = '0';
        quantidade_de_palavras--; // Diminui a contagem total de palavras.
        palavra_selecionada = -1; // Permite selecionar uma nova palavra
      }
    }

    if (quantidade_de_palavras_acertadas == TOTAL_DE_PALAVRAS || testa_tempoDigitacao(vetPalavras, t0))
    {
      // tela_limpa();
      tela_lincol(tela_nlin() / 2, tela_ncol() / 2);
      if (quantidade_de_palavras_acertadas == TOTAL_DE_PALAVRAS)
      {
        printf("Parabéns, você acertou todas as palavras!");
      }
      else
      {
        printf("Tempo Esgotado!");
      }

      if (pontos > recordes[2].pontos)
      {
        printf("Digite seu nome: ");
        fgets(nome, 16, stdin);
      }
      break;
    }
  }

  atualizar_recorde(pontos, nome, recordes);
  mostrar_recordes_final_de_jogo(recordes);
  tela_atualiza();
}

void espera_enter()
{
  while (getchar() != '\n')
  {
    // não faz nada
  }
}

void apresentacao()
{
  printf("Você deve digitar as palavras que aparecerão na tela.\n");
  printf("A ordem é importante, e reduzira pontos se errar a letra!!\n");
  printf("Tecle <enter> para iniciar. ");
  tela_atualiza();
  espera_enter();
}

void encerramento()
{
  printf("Até a próxima.\n");
  printf("obrigado por jogar.\n");
  tela_atualiza();
}

bool quer_jogar_de_novo()
{
  espera_enter();
  tela_lincol(0 , 0);
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

void processa_Palavra(palavras palavrasJogo[], double inicio)
{
  int larg = tela_ncol();
  int alt = tela_nlin();
  int tamPalavra;
  for (int i = 0; i < TOTAL_DE_PALAVRAS; i++)
  {
    vetPalavras[i].ativa = false;
    vetPalavras[i].hora_ativacao = rand() % 21;
    vetPalavras[i].tempo_digitacao = 5 + rand() % 26;
    tamPalavra = strlen(palavrasJogo[i].palavra);
    vetPalavras[i].pos_horizontal = (larg - tamPalavra) * (rand() % 101) / 100;
    vetPalavras[i].pos_vertical = 2 + alt * ((tela_relogio() - inicio) - vetPalavras[i].hora_ativacao) / vetPalavras[i].tempo_digitacao;
  }
}

bool testa_tempoDigitacao(palavras palavrasJogo[], double inicio)
{
  for (int i = 0; i < TOTAL_DE_PALAVRAS; i++)
  {
    if ((vetPalavras[i].tempo_digitacao + vetPalavras[i].hora_ativacao <= (tela_relogio() - inicio)) && palavrasJogo[i].palavra[0] != '\0')
    {
      return true;
    }
  }
  return false;
}