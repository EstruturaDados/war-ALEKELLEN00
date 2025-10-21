#include <stdio.h>
#include <string.h>

typedef struct
{
    char nome[50];
    char corExercito[20];
    int numTropas;
} Territorio;

#define NUM_TERRITORIOS 5

int main()
{
    Territorio mapa[NUM_TERRITORIOS];

    for (int i = 0; i < NUM_TERRITORIOS; i++)
    {
        printf("\n--- Cadastro do Território %d ---\n", i + 1, NUM_TERRITORIOS);

        // 1.  NOME (usando fgets e removendo '\n')
        printf("Digite o nome do Território :");
        fgets(mapa[i].nome, 50, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;

        // 2. COR DO EXÉRCITO (usando fgets e removendo '\n')
        printf("Digite a cor do exército dominante: ");
        fgets(mapa[i].corExercito, 20, stdin);
        mapa[i].corExercito[strcspn(mapa[i].corExercito, "\n")] = 0;

        // 3. NÚMERO DE TROPAS (usando scanf e limpando o buffer)
        printf("Digite o número de tropas: ");
        if (scanf("%d", &mapa[i].numTropas) != 1)
        {

            // Tratamento simples de erro para scanf
            printf("Erro na leitura de tropas. Usando 0.\n");
            mapa[i].numTropas = 0;
        }

        // Limpa o buffer do teclado após o scanf (essencial para o próximo fgets)
        while (getchar() != '\n')
            ;
    }

    printf("\n\n*************************************************\n");
    printf("****** ESTADO ATUAL DO MAPA (WAR ESTRUTURADO) *******\n");
    printf("*************************************************\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++)

    {
        printf("Território %d:\n", i + 1);
        printf("  Nome: %s\n", mapa[i].nome);
        printf("  Cor do Exército: %s\n", mapa[i].corExercito);
        printf("  Número de Tropas: %d\n", mapa[i].numTropas);
        printf("---------------------------------------------\n");
    }

    return 0;
}
