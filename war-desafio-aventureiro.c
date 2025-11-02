#include <stdio.h>  // Para entrada/saída (printf, scanf)
#include <stdlib.h> // Para calloc, free, rand, srand
#include <time.h>   // Para time(NULL)

typedef struct
{
    char nome[50];
    int tropas;
    int id;
} Territorio;

// --- PROTÓTIPOS DE FUNÇÕES (Declaração obrigatória antes do main) ---
void inicializar_gerador_aleatorio();
Territorio *criar_territorios(int num_territorios);
void exibir_territorios(Territorio *territorios, int num_territorios);
void simular_ataque(Territorio *atacante, Territorio *defensor);
void liberar_memoria(Territorio *territorios);

// --- FUNÇÃO PRINCIPAL ---
int main()
{
    const int NUM_TERRITORIOS = 5;
    Territorio *mapa = NULL; // Ponteiro declarado dentro do main (boa prática)

    // PASSO 1: Inicializar o gerador de números aleatórios
    inicializar_gerador_aleatorio();

    // PASSO 2: Alocar e cadastrar os territórios (Uso de calloc)
    mapa = criar_territorios(NUM_TERRITORIOS);
    if (mapa == NULL)
    {
        printf("Erro ao alocar memória para os territórios.\n");
        return 1;
    }

    // --- Fase de Ataque (Laço Interativo) ---
    int atacante_id, defensor_id;
    char continuar_ataque;

    do
    {
        exibir_territorios(mapa, NUM_TERRITORIOS);

        printf("\n--- FASE DE ATAQUE ---\n");
        printf("Escolha o Território Atacante (ID 1 a %d): ", NUM_TERRITORIOS);
        if (scanf("%d", &atacante_id) != 1 || atacante_id < 1 || atacante_id > NUM_TERRITORIOS)
        {
            printf("ID de atacante inválido.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        printf("Escolha o Território Defensor (ID 1 a %d): ", NUM_TERRITORIOS);
        if (scanf("%d", &defensor_id) != 1 || defensor_id < 1 || defensor_id > NUM_TERRITORIOS)
        {
            printf("ID de defensor inválido.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        if (atacante_id == defensor_id)
        {
            printf("Um território não pode atacar a si mesmo!\n");
            continue;
        }

        // Corrigido: Passando o endereço (&) da posição no array
        simular_ataque(&mapa[atacante_id - 1], &mapa[defensor_id - 1]);

        printf("\nDeseja realizar outro ataque? (s/n): ");
        while (getchar() != '\n')
            ;
        scanf("%c", &continuar_ataque);

    } while (continuar_ataque == 's' || continuar_ataque == 'S');

    // PASSO 6: Exibir o resultado final e liberar a memória
    printf("\n--- RESULTADO FINAL ---\n");
    exibir_territorios(mapa, NUM_TERRITORIOS);

    liberar_memoria(mapa); // Uso de free
    printf("Memória liberada. Fim do desafio.\n");

    return 0;
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

/**
 * @brief Inicializa a semente do gerador de números aleatórios (rand).
 */
void inicializar_gerador_aleatorio()
{
    srand((unsigned int)time(NULL));
}

/**
 * @brief Aloca dinamicamente a memória para os territórios usando calloc
 * e preenche os dados iniciais.
 */
Territorio *criar_territorios(int num_territorios)
{
    // Alocação Dinâmica com calloc: Inicializa tudo com zero.
    Territorio *t = (Territorio *)calloc(num_territorios, sizeof(Territorio));

    if (t == NULL)
    {
        return NULL;
    }

    printf("--- CADASTRO INICIAL DOS TERRITÓRIOS ---\n");
    for (int i = 0; i < num_territorios; i++)
    {
        t[i].id = i + 1;

        // Cadastro: usa notação de array (t[i]) para acessar a estrutura
        sprintf(t[i].nome, "Territorio_%d", t[i].id);

        // Tropas iniciais: valor aleatório entre 3 e 10
        t[i].tropas = 3 + (rand() % 8);
        printf("Território %d: '%s' com %d tropas.\n", t[i].id, t[i].nome, t[i].tropas);
    }
    printf("----------------------------------------\n");
    return t;
}

/**
 * @brief Exibe o estado atual de todos os territórios.
 */
void exibir_territorios(Territorio *territorios, int num_territorios)
{
    printf("\n*** MAPA ATUAL ***\n");
    for (int i = 0; i < num_territorios; i++)
    {
        // Exibe o endereço de memória, demonstrando o uso de ponteiros
        printf("[ID: %d] %-15s | Tropas: %d | Endereço: %p\n",
               territorios[i].id, territorios[i].nome, territorios[i].tropas,
               (void *)&territorios[i]);
    }
    printf("******************\n");
}

/**
 * @brief Simula uma rodada de ataque.
 * @param atacante Ponteiro para a estrutura do território atacante.
 * @param defensor Ponteiro para a estrutura do território defensor.
 */
void simular_ataque(Territorio *atacante, Territorio *defensor)
{
    if (atacante->tropas <= 0)
    {
        printf("Ataque cancelado: O atacante '%s' não tem tropas para atacar.\n", atacante->nome);
        return;
    }
    if (defensor->tropas <= 0)
    {
        printf("O território '%s' já está sem tropas.\n", defensor->nome);
        return;
    }

    // Geração de dados de batalha (1 a 6)
    int dado_ataque = 1 + (rand() % 6);
    int dado_defesa = 1 + (rand() % 6);

    printf("\n--- SIMULAÇÃO DE ATAQUE ---\n");
    printf("Atacante: %s (Tropas: %d) vs Defensor: %s (Tropas: %d)\n",
           atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);
    printf("Dados Sorteados -> Ataque: %d | Defesa: %d\n", dado_ataque, dado_defesa);

    // Lógica da Batalha: Empates favorecem o atacante
    if (dado_ataque >= dado_defesa)
    {
        printf("Resultado: O **ATACANTE VENCEU** (Dado: %d >= %d).\n", dado_ataque, dado_defesa);

        // Uso de ponteiro para modificar o valor real na memória
        defensor->tropas--;

        printf("-> O Defensor '%s' perdeu 1 tropa. Tropas restantes: %d\n",
               defensor->nome, defensor->tropas);

        // Lógica de Conquista
        if (defensor->tropas <= 0)
        {
            printf("\n!!! CONQUISTA !!! O território '%s' foi conquistado!\n", defensor->nome);

            // Requisito: O território conquistado recebe 1 tropa (a primeira do novo dono)
            defensor->tropas = 1;
            printf("-> O território conquistado foi ocupado com 1 tropa.\n");
        }
    }
    else
    {
        printf("Resultado: O **DEFENSOR VENCEU** (Dado: %d < %d).\n", dado_ataque, dado_defesa);

        // Uso de ponteiro para modificar o valor real na memória
        atacante->tropas--;

        printf("-> O Atacante '%s' perdeu 1 tropa. Tropas restantes: %d\n",
               atacante->nome, atacante->tropas);
    }
    printf("---------------------------\n");
}

/**
 * @brief Libera a memória alocada dinamicamente usando free.
 */
void liberar_memoria(Territorio *territorios)
{
    if (territorios != NULL)
    {
        free(territorios); // Ação final e essencial de gerência de memória
    }
}