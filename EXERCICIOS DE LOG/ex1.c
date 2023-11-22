// Crie um algoritmo que receba uma matriz e imprima sua diagonal principal

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int matriz[100][100];
    int linha, coluna;

    printf("Digite o numero de linhas: ");
    scanf("%d", &linha);

    printf("Digite o numero de colunas: ");
    scanf("%d", &coluna);

    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            printf("Digite o valor da posicao [%d][%d]: ", i, j);
            scanf("%d", &matriz[i][j]);
        }
    }

    // Imprimindo a diagonal principal

    printf("Diagonal principal: ");

    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            if (i == j)
            {
                printf("%d ", matriz[i][j]);
            }
            else
            {
                printf("  ");
            }
        }
        printf("\n");
    }
    return 0;
}