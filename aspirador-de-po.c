#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <windows.h>

#define MAX_TAMANHO 10

void inicializar_ambiente(int ambiente[MAX_TAMANHO][MAX_TAMANHO], int tamanho, int quantidade_sujeira)
{
    srand(time(NULL));

    // Inicializa o ambiente como limpo
    for (int i = 0; i < tamanho; i++)
    {
        for (int j = 0; j < tamanho; j++)
        {
            ambiente[i][j] = 0;
        }
    }

    // Coloca sujeira aleatoriamente no ambiente
    for (int k = 0; k < quantidade_sujeira; k++)
    {
        int x = rand() % tamanho;
        int y = rand() % tamanho;
        ambiente[x][y] = 1;
    }
}

void menu(int *modo_jogo, int *tamanho, int *quantidade_sujeira, int *x, int *y, int ambiente[MAX_TAMANHO][MAX_TAMANHO])
{
    int opcao;

    do
    {
        printf("Escolha uma opção:\n");
        printf("1. Modo IA - Ambiente Observável\n");
        printf("2. Modo IA - Ambiente Parcialmente Observável A\n");
        printf("3. Modo IA - Ambiente Parcialmente Observável B\n");
        scanf("%d", &opcao);
        system("cls");
    } while (opcao < 1 || opcao > 3); // Validação de entrada

    *modo_jogo = opcao;

    printf("Digite o tamanho do ambiente (max %d): ", MAX_TAMANHO);
    scanf("%d", tamanho);
    printf("Digite a quantidade de sujeira: ");
    scanf("%d", quantidade_sujeira);
    system("cls");

    *x = rand() % *tamanho;
    *y = rand() % *tamanho;

    inicializar_ambiente(ambiente, *tamanho, *quantidade_sujeira);
}

void imprimir_ambiente(int ambiente[MAX_TAMANHO][MAX_TAMANHO], int tamanho, int x, int y)
{
    for (int i = 0; i < tamanho; i++)
    {
        for (int j = 0; j < tamanho; j++)
        {
            if (i == x && j == y && ambiente[i][j] == 1)
            {
                printf(" * ");
            }
            else if (i == x && j == y)
            {
                printf(" A ");
            }
            else if (ambiente[i][j] == 1)
            {
                printf(" S ");
            }
            else
            {
                printf(" . ");
            }
        }
        printf("\n");
    }
}

void imprimir_sala_atual(int ambiente[MAX_TAMANHO][MAX_TAMANHO], int x, int y)
{
    if (ambiente[x][y] == 1)
    {
        printf("Você está na sala %d, %d com sujeira.\n", x, y);
    }
    else
    {
        printf("Você está na sala %d, %d limpa.\n", x, y);
    }
}

void imprimir_salas_adjacentes(int ambiente[MAX_TAMANHO][MAX_TAMANHO], int tamanho, int x, int y)
{
    printf("Você está na sala %d, %d.\n", x, y);

    // Imprime as salas adjacentes
    for (int i = x - 1; i <= x + 1; i++)
    {
        for (int j = y - 1; j <= y + 1; j++)
        {
            if (i >= 0 && i < tamanho && j >= 0 && j < tamanho && (i != x || j != y))
            {
                if (ambiente[i][j] == 1)
                {
                    printf("Sala %d, %d: Suja\n", i, j);
                }
                else
                {
                    printf("Sala %d, %d: Limpa\n", i, j);
                }
            }
        }
    }
}

void mover_aspirador(int *x, int *y, char movimento, int tamanho)
{
    if (movimento == 'w' && *x > 0)
    {
        (*x)--;
    }
    else if (movimento == 's' && *x < tamanho - 1)
    {
        (*x)++;
    }
    else if (movimento == 'a' && *y > 0)
    {
        (*y)--;
    }
    else if (movimento == 'd' && *y < tamanho - 1)
    {
        (*y)++;
    }
}

void aspirar(int ambiente[MAX_TAMANHO][MAX_TAMANHO], int x, int y, int *sujeiras_restantes)
{
    if (ambiente[x][y] == 1)
    {
        ambiente[x][y] = 0; // Limpa a sujeira
        (*sujeiras_restantes)--; // Decrementa a quantidade de sujeiras restantes
        printf("Sala limpa! Restam %d sujeiras.\n", *sujeiras_restantes);
    }
    else
    {
        printf("Essa sala já está limpa.\n");
    }
}

int dentro_limites(int x, int y, int tamanho)
{
    return x >= 0 && x < tamanho && y >= 0 && y < tamanho;
}

void ia_observavel(int ambiente[MAX_TAMANHO][MAX_TAMANHO], int tamanho, int *x, int *y, int *sujeiras_restantes)
{
    // Loop até que não haja mais sujeiras restantes
    while (*sujeiras_restantes > 0)
    {
        int encontrado = 0; // Variável para verificar se encontramos sujeira

        for (int i = 0; i < tamanho; i++)
        {
            for (int j = 0; j < tamanho; j++)
            {
                if (ambiente[i][j] == 1) // Verifica se há sujeira na posição (i, j)
                {
                    encontrado = 1; // Marcar que encontramos sujeira
                    // Mover o aspirador para a posição (i, j) gradualmente
                    while (*x != i || *y != j)
                    {
                        if (*x < i && *x + 1 < tamanho)
                        {
                            (*x)++;
                        }
                        else if (*x > i && *x - 1 >= 0)
                        {
                            (*x)--;
                        }

                        if (*y < j && *y + 1 < tamanho)
                        {
                            (*y)++;
                        }
                        else if (*y > j && *y - 1 >= 0)
                        {
                            (*y)--;
                        }

                        // Limpar a tela e imprimir a posição atual do aspirador
                        system("cls");
                        imprimir_ambiente(ambiente, tamanho, *x, *y);
                        Sleep(500);  // Pausa para visualizar o movimento
                    }

                    // Aspirar a sujeira após chegar na posição
                    aspirar(ambiente, *x, *y, sujeiras_restantes);
                    Sleep(2000); // Pausa após aspirar

                    // Verificar se ainda há sujeira restante
                    if (*sujeiras_restantes == 0)
                    {
                        system("cls");
                        imprimir_ambiente(ambiente, tamanho, *x, *y); // Imprime a última posição do aspirador
                        printf("Todas as sujeiras foram limpas!\n"); // Mensagem de finalização
                        return; // Retorna da função
                    }

                    // Para sair do loop após encontrar e limpar uma sujeira
                    return;
                }
            }
        }

        // Se nenhuma sujeira foi encontrada, encerra o loop
        if (!encontrado)
        {
            break; // Sai do loop se não houver sujeira restante
        }
    }
}

void Parcialmente_Observavel_A(int ambiente[MAX_TAMANHO][MAX_TAMANHO], int tamanho, int *x, int *y, int *sujeiras_restantes)
{
    int visitado[MAX_TAMANHO][MAX_TAMANHO] = {0}; // Matriz para posições visitadas
    char direcoes[4] = {'w', 's', 'a', 'd'};
    srand(time(NULL));

    while (*sujeiras_restantes > 0)
    {
        visitado[*x][*y] = 1; // Marca a posição atual como visitada

        // Aspira se houver sujeira na posição atual
        if (ambiente[*x][*y] == 1) {
            aspirar(ambiente, *x, *y, sujeiras_restantes);
        }

        int moveu = 0;
        // Tenta mover para uma posição com sujeira
        for (int i = 0; i < 4; i++)
        {
            int nova_x = *x, nova_y = *y;

            switch (direcoes[i])
            {
                case 'w': nova_x--; break;
                case 's': nova_x++; break;
                case 'a': nova_y--; break;
                case 'd': nova_y++; break;
            }

            if (dentro_limites(nova_x, nova_y, tamanho) && ambiente[nova_x][nova_y] == 1 && !visitado[nova_x][nova_y])
            {
                *x = nova_x; *y = nova_y; // Atualiza a posição do aspirador
                moveu = 1; // Marca que o aspirador se moveu
                break; // Sai do loop ao encontrar uma sujeira
            }
        }

        // Se não encontrou sujeira, move-se aleatoriamente
        if (!moveu)
        {
            while (1)
            {
                int nova_x = rand() % tamanho;
                int nova_y = rand() % tamanho;

                if (dentro_limites(nova_x, nova_y, tamanho) && !visitado[nova_x][nova_y])
                {
                    *x = nova_x; *y = nova_y; // Atualiza a posição do aspirador
                    break; // Sai do loop ao mover-se
                }
            }
        }

        system("cls");
        imprimir_ambiente(ambiente, tamanho, *x, *y);
        Sleep(500); // Pausa para visualizar o movimento
    }
}

void Parcialmente_Observavel_B(int ambiente[MAX_TAMANHO][MAX_TAMANHO], int tamanho, int *x, int *y, int *sujeiras_restantes)
{
    // Imprime as salas adjacentes
    imprimir_salas_adjacentes(ambiente, tamanho, *x, *y);

    // Tenta limpar a sala atual
    if (ambiente[*x][*y] == 1)
    {
        aspirar(ambiente, *x, *y, sujeiras_restantes);
        printf("Limpeza realizada na sala atual: (%d, %d)\n", *x, *y);
        Sleep(1500);
        system("cls");
    }

    // Verifica as salas adjacentes para sujeira
    int encontrado = 0;
    for (int i = *x - 1; i <= *x + 1; i++)
    {
        for (int j = *y - 1; j <= *y + 1; j++)
        {
            // Ignora a sala atual e verifica se a sala adjacente está suja
            if (i >= 0 && i < tamanho && j >= 0 && j < tamanho && (i != *x || j != *y))
            {
                if (ambiente[i][j] == 1)
                {
                    // Mover para a sala adjacente suja
                    *x = i;
                    *y = j;
                    aspirar(ambiente, *x, *y, sujeiras_restantes);
                    encontrado = 1;
                    printf("Movendo para sala suja adjacente: (%d, %d)\n", *x, *y);
                    Sleep(1500);
                    system("cls");
                    return;  // Sai da função após limpar uma sala suja
                }
            }
        }
    }

    // Se não encontrou sujeira adjacente, mover aleatoriamente
    if (!encontrado)
    {
        int direcao = rand() % 4;
        switch (direcao)
        {
            case 0: // Mover para cima
                if (*x > 0) (*x)--;
                break;
            case 1: // Mover para baixo
                if (*x < tamanho - 1) (*x)++;
                break;
            case 2: // Mover para a esquerda
                if (*y > 0) (*y)--;
                break;
            case 3: // Mover para a direita
                if (*y < tamanho - 1) (*y)++;
                break;
        }
        printf("Movendo aleatoriamente para a posição: (%d, %d)\n", *x, *y);
        Sleep(1500);
        system("cls");
    }
}

void opcoes(int modo_jogo, int tamanho, int quantidade_sujeira, int x, int y, int ambiente[MAX_TAMANHO][MAX_TAMANHO])
{
    int sujeiras_restantes = quantidade_sujeira;
    char opcao2;

    switch (modo_jogo)
    {
        case 1:
            printf("Escolha uma opção:\n");
            printf("'M'. Manual\n");
            printf("'I'. Inteligência artificial.\n");
            scanf(" %c", &opcao2);
            system("cls");

            if (opcao2 == 'M' || opcao2 == 'm')
            {
                while (sujeiras_restantes > 0)
                {
                    imprimir_ambiente(ambiente, tamanho, x, y);
                    char movimento;
                    printf("Digite o movimento (w: cima, s: baixo, a: esquerda, d: direita) ou 'c' para limpar: ");
                    scanf(" %c", &movimento);
                    if (movimento == 'c')
                    {
                        aspirar(ambiente, x, y, &sujeiras_restantes);
                    }
                    else
                    {
                        mover_aspirador(&x, &y, movimento, tamanho);
                    }
                }
            }
            else if (opcao2 == 'I' || opcao2 == 'i')
            {
                while (sujeiras_restantes > 0)
                {
                    imprimir_ambiente(ambiente, tamanho, x, y);
                    ia_observavel(ambiente, tamanho, &x, &y, &sujeiras_restantes);
                }
            }
            else
            {
                printf("Essa opção não existe.");
                return;
            }
            break;

        case 2:
            printf("Escolha uma opção:\n");
            printf("'M'. Manual\n");
            printf("'I'. Inteligência artificial\n");
            scanf(" %c", &opcao2);
            system("cls");

            if (opcao2 == 'M' || opcao2 == 'm')
            {
                while (sujeiras_restantes > 0)
                {
                    imprimir_sala_atual(ambiente, x, y);
                    system("cls");
                    Parcialmente_Observavel_A(ambiente, tamanho, &x, &y, &sujeiras_restantes);
                    system("cls");
                }
            }
            else if (opcao2 == 'I' || opcao2 == 'i')
            {
                while (sujeiras_restantes > 0)
                {
                    Parcialmente_Observavel_A(ambiente, tamanho, &x, &y, &sujeiras_restantes);
                }
            }
            else
            {
                printf("Essa opção não existe.\n");
                return;
            }
            break;

        case 3:
            while (sujeiras_restantes > 0)
            {
                Parcialmente_Observavel_B(ambiente, tamanho, &x, &y, &sujeiras_restantes);
            }
            break;
    }

    printf("Todas as sujeiras foram limpas!\n");
}

int main()
{
    setlocale(LC_ALL, "Portuguese");
    int ambiente[MAX_TAMANHO][MAX_TAMANHO];
    int modo_jogo, tamanho, quantidade_sujeira, x, y;

    menu(&modo_jogo, &tamanho, &quantidade_sujeira, &x, &y, ambiente);
    opcoes(modo_jogo, tamanho, quantidade_sujeira, x, y, ambiente);

    return 0;
}