#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "tecla.h"
#include "tela.h"

// definições de constantes
// quantas palavras gerar
#define N_PALAVRAS 10
// quantos segundos para digitar
#define TEMPO 30
// tamanho máximo das palavras
#define TAMANHO_PALAVRA 16
// quantidade total de palavras dentro do arquivo
#define QNTDD_PALAVRAS 920

// remove o dado na posição p de v[t], deslocando os v[>t]
void remove_letra(char v[], char n);
// acha e retorna a linha dentro da matriz que contém a primeira letra igual a letra digitada
int acha_palavra(char v[][TAMANHO_PALAVRA], char n);
// testa se a palavra é válida de acordo com os critérios 
bool testa_Palavra(char v[]);
//preenche uma matriz auxiliar com as palavras que estão dentro dos critérios
int preenche_matriz_aux(char matrizAux[][TAMANHO_PALAVRA]);
// preenche a matriz do jogo a partir da matriz auxiliar com palavras aleatórias
void preenche_matriz_jogo(char v[][TAMANHO_PALAVRA]);
// calcula as posições das palavras, o tempo de ativação e o tempo de digitação de cada uma
void processa_Palavra(char v[][TAMANHO_PALAVRA], double inicio);
// testa se o tempo de digitação da palavra ja passou
bool testa_tempoDigitacao(char palavras[][TAMANHO_PALAVRA], double inicio);

// funções auxiliares

// apresenta o programa
void apresentacao();
// encerra o programa
void encerramento();
// executa uma partida
void jogo();
// verifica a vontade do jogador
bool quer_jogar_de_novo();
// mostra a matriz com as palavras e o tempo restante
void mostra_matriz(char v[][TAMANHO_PALAVRA], double tempo_restante, int indice, double inicio);
// limpa a linha de entrada
void espera_enter();
int qtdePalavras = N_PALAVRAS;

typedef struct {
  int posicao_horizontal;
  int posicao_vertial;
  int hora_ativacao;
  int tempo_digitacao;
  bool ativa;
}Palavra;

Palavra vetPalavras[10];
int main()
{
  tela_ini();
  tecla_ini();
  srand(time(0));
  
  apresentacao();
 
  do {
    jogo();
  } while(quer_jogar_de_novo());

  encerramento();
  tecla_fim();
  tela_fim();
}

void jogo()
{
  //declaração das variáveis
  char palavras[N_PALAVRAS][TAMANHO_PALAVRA];
  char palavraSel[N_PALAVRAS];
  int indice = -1;
  
  preenche_matriz_jogo(palavras);
  
  double inicio = tela_relogio();
  double tempo_restante = TEMPO;
  processa_Palavra(palavras, inicio);
  while (true) {
    char letra; 
    do {  // laço para ler a palavra digitada 
      tempo_restante = TEMPO - (tela_relogio() - inicio);
      if (testa_tempoDigitacao(palavras, inicio) || qtdePalavras == 0) {
        break;
      }
      mostra_matriz(palavras, tempo_restante, indice, inicio);
      letra = tecla_le_char();
      if (letra != '\0') {
        if (indice < 0) { // testa se não existe palavra selecionada
          indice = acha_palavra(palavras, letra); // retorna para variavel indice a linha dentro da matriz da letra digitada 
        }
        if (indice >= 0) { // entra nessa condição quando existe uma palavra selecionada
          
          remove_letra(palavras[indice], letra); // chama a funçao que remove a letra 
          if (palavras[indice][0] == '\0') { // testa se a o primeiro caractere da palavra é \0, ou seja, se a palavra foi removida
            qtdePalavras--; // diminui a quantidade de palavras totais
            indice = -1; // reseta o indice permitindo selecionar uma nova palavra
          }
        }
      }
    } while(true);
      tela_limpa();
      if (qtdePalavras <= 0) {
        printf("Parabéns, você acertou todas as palavras!");
      }
      else {
        printf("Tempo esgotado");  
      }
      break;
  }
  tela_atualiza();
}

void espera_enter()
{
  while (tecla_le_char() != '\n') {
    ; /* não faz nada */
  }
}

void apresentacao()
{
  printf("Você deve digitar as palavras em ordem, que aparecerão na tela.\n");
  printf("Tecle <enter> para iniciar. \n");
  espera_enter();
}

void encerramento()
{
  printf("Até a próxima.\n");
}

bool quer_jogar_de_novo()
{
  // limpa a entrada
  espera_enter();

  printf("Digite 's' para jogar de novo ");
  while (true) {
    char c = tecla_le_char();
    if (c == '\n') {
      return false;
    } else if (c == 's' || c == 'S') {
      espera_enter();
      return true;
    }
  }
}

void mostra_matriz(char v[][TAMANHO_PALAVRA], double tempo_restante, int indice, double inicio)
{
  tela_limpa();
  int i;
  int lin;
  int col;
  int alt = tela_nlin();
  // lacço principal para cada palavra
  for (i = 0; i < N_PALAVRAS; i++) {
    // testa se a palavra está vazia, caso sim não printa
    if (v[i][0] == '\0') {
      continue;
    }
    if (i == indice) {
      tela_cor_letra(0,255,0);
      tela_cor_fundo(143,188,143);     
    } else {
      tela_cor_letra(0,206,209);
      tela_cor_fundo(119,136,153);     
    }
    
    if (TEMPO -  tempo_restante >= vetPalavras[i].hora_ativacao) {
      lin = vetPalavras[i].posicao_vertial;
      col = vetPalavras[i].posicao_horizontal;
      tela_lincol(lin, col);
      printf("%s", v[i]);
      vetPalavras[i].posicao_vertial = 2 + alt * ((tela_relogio() - inicio) - vetPalavras[i].hora_ativacao) / vetPalavras[i].tempo_digitacao;
      vetPalavras[i].ativa = true;
    }
    
  }
  tela_cor_normal();
  tela_lincol(0, 0);
  printf("digite as palavras conforme aparecem:");
  tela_atualiza();
}

void remove_letra(char v[], char n)
{
  char letraAux;
  int i = 0;
  // testa se a letra digitada é a primeira, caso for remove a letra passando para uma posição depois do \0
  if (n == v[0]) {
    while(v[i] != '\0') {
      letraAux = v[i];
      v[i] = v[i + 1];
      v[i + 1] = letraAux;
      if(v[i] == '\0'){
        break;
      }
      i++; 
    }
  } 
}

int acha_palavra(char v[][TAMANHO_PALAVRA], char n)
{
  for (int i = 0; i < N_PALAVRAS; i++) { // varre todas a linhas da matriz
    if (v[i][0] == n && vetPalavras[i].ativa == true) { // testa se a letra digitada é igual a primeira letra de cada linha, caso for retorna a linha da matriz onde está a palavra
      return i;
    }
  }
  return -1;
}

bool testa_Palavra (char v[])
{
  int i;
  int tamanho = strlen(v);
  for (i = 0; i < tamanho; i++) {
    // testa se a letra está no intervalo das letras na tabela asciii, caso não estiver significa que tem acento e retorna falso
    if ((v[i] < 'A' || v[i] > 'Z') && (v[i] < 'a' || v[i] > 'z')) { 
      return false;
    } else if (v[i] >= 'A' && v[i] <= 'Z') { //testa se tem letra maiúscula, e já transforma em minuscula 
      v[i] += ('a' - 'A');
    }   
  }
  return true;
}

void preenche_matriz_jogo(char v[][TAMANHO_PALAVRA])
{
  // preenche a matriz do jogo com palavras aleatórias da matriz auxiliar
  char matrizAux[QNTDD_PALAVRAS][TAMANHO_PALAVRA];
  int totPalavras = preenche_matriz_aux(matrizAux); 
  for (int i = 0; i < N_PALAVRAS; i++) { 
    strcpy(v[i], matrizAux[rand() % totPalavras]);
  }     
}

int preenche_matriz_aux(char matrizAux[][TAMANHO_PALAVRA]) 
{
  // preenche a matri auxiliar apenas com as palavras válidas
  char vetAux[TAMANHO_PALAVRA];
  int i = 0;
  FILE *palavras = fopen("palavras.txt", "r");
  if (palavras == NULL) {
    printf("Não foi possível abrir o arquvio");
    exit(1);
  }
  while (fscanf(palavras, "%s", vetAux) == 1) {
    if (testa_Palavra(vetAux)) {
      strcpy(matrizAux[i], vetAux);
      i++;
    }  
  }
  fclose(palavras);
  return i;
}

void processa_Palavra (char v[][TAMANHO_PALAVRA], double inicio) 
{
  int larg = tela_ncol();
  int alt = tela_nlin();
  int tamPalavra;
  for (int i = 0; i < N_PALAVRAS; i++) {
    vetPalavras[i].ativa = false;
    vetPalavras[i].hora_ativacao = rand() % 21;
    vetPalavras[i].tempo_digitacao = 5 + rand() % 26;
    tamPalavra = strlen(v[i]);
    vetPalavras[i].posicao_horizontal = (larg - tamPalavra)  *  (rand() % 101) / 100;
    vetPalavras[i].posicao_vertial = 2 + alt  * ((tela_relogio() - inicio) - vetPalavras[i].hora_ativacao) / vetPalavras[i].tempo_digitacao;
  }
}

bool testa_tempoDigitacao(char palavras[][TAMANHO_PALAVRA], double inicio)
{
  for(int i = 0; i < N_PALAVRAS; i++) {
    
    if ((vetPalavras[i].tempo_digitacao + vetPalavras[i].hora_ativacao <= (tela_relogio() - inicio)) && palavras[i][0] != '\0') {
      return true;
    }
  }
  return false;
}