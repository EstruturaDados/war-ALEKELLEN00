#include <stdio.h>
#include <stdlib.h> // calloc, free, rand, srand
#include <time.h>   // time(NULL)
#include <string.h> // strcmp

// --- CONSTANTES ---
#define NUM_TERRITORIOS 5
#define ID_JOGADOR_HUMANO 1
#define ID_JOGADOR_VERDE 2
#define MAX_TROPAS_INICIAIS 10

#define TIPO_DESTRUIR_VERDE 1
#define TIPO_CONQUISTAR_TOTAL 2
#define TIPO_CONQUISTAR_ESPECIFICO 3

// --- Estruturas de Dados ---

// Estrutura para representar um Território
typedef struct
{
    char nome[50];
    int tropas;
    int id;      // ID do território (1 a NUM_TERRITORIOS)
    int dono_id; // ID do dono (1: Humano, 2: Verde)
} Territorio;

// Estrutura para representar a Missão do Jogador
typedef struct
{
    int tipo;    // 1: Destruir Verde, 2: Conquistar X territórios
    int alvo_id; // ID do alvo (se aplicável, ex: ID_JOGADOR_VERDE)
    int valor;   // Número de territórios a conquistar (se aplicável, ex: 3)
} Missao;

// --- PROTÓTIPOS DE FUNÇÕES (Modularização) ---

// Funções de Inicialização e Boas Práticas
void inicializar_gerador_aleatorio(void);
Territorio *criar_mapa_inicial(const int num_territorios);
void liberar_memoria(Territorio *territorios);

// Funções de Missão
Missao gerar_missao_aleatoria(void);
void exibir_missao(const Missao *missao, const Territorio *mapa, const int num_territorios);
int verificar_missao(const Missao *missao, const Territorio *mapa, const int num_territorios);
int contar_territorios_do_dono(const Territorio *mapa, const int num_territorios, const int dono_id);
int contar_tropas_do_dono(const Territorio *mapa, const int num_territorios, const int dono_id);

const Territorio *encontrar_territorio_por_id(const Territorio *mapa, const int num_territorios, const int id_alvo);

// Funções de Jogo
void exibir_mapa(const Territorio *mapa, const int num_territorios);
void exibir_menu(void);
void realizar_ataque(Territorio *atacante, Territorio *defensor);
int processar_ataque(Territorio *mapa, const int num_territorios);

// --- FUNÇÃO PRINCIPAL ---
int main()
{
    Territorio *mapa = NULL;
    Missao missao_jogador;
    int opcao;

    inicializar_gerador_aleatorio();

    // Alocação e Inicialização
    mapa = criar_mapa_inicial(NUM_TERRITORIOS);
    if (mapa == NULL)
    {
        printf("Erro fatal: Falha ao alocar memória.\n");
        return 1;
    }

    // Atribuir Missão
    missao_jogador = gerar_missao_aleatoria();
    printf("\n>>> MISSAO RECEBIDA <<<\n");
    exibir_missao(&missao_jogador, mapa, NUM_TERRITORIOS);
    printf("--------------------------\n");

    // Loop Principal do Jogo
    do
    {
        exibir_mapa(mapa, NUM_TERRITORIOS);
        exibir_menu();
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1)
        {
            opcao = -1; // Opção inválida
            while (getchar() != '\n')
                ; // Limpa buffer
        }

        switch (opcao)
        {
        case 1: // Atacar
            if (processar_ataque(mapa, NUM_TERRITORIOS))
            {
                // Se houver alguma mudança no mapa (conquista ou perda de tropas)
                if (verificar_missao(&missao_jogador, mapa, NUM_TERRITORIOS))
                {
                    printf("\n##################################\n");
                    printf("### VOCÊ COMPLETOU SUA MISSÃO! ###\n");
                    printf("###           VITÓRIA!         ###\n");
                    printf("##################################\n");
                    opcao = 0; // Encerra o jogo
                }
            }
            break;
        case 2: // Verificar Missão
            exibir_missao(&missao_jogador, mapa, NUM_TERRITORIOS);
            if (verificar_missao(&missao_jogador, mapa, NUM_TERRITORIOS))
            {
                printf("\n##################################\n");
                printf("### VOCÊ COMPLETOU SUA MISSÃO! ###\n");
                printf("###           VITÓRIA!         ###\n");
                printf("##################################\n");
            }
            else
            {
                printf("\n*** Missão em Andamento. Continue a Luta! ***\n");
            }

            printf("\nPressione ENTER para voltar ao Menu...");
            while (getchar() != '\n')
                ;      // Limpa o buffer
            getchar(); // Espera por ENTER

            break;
        case 0: // Sair
            printf("\nEncerrando o jogo. Até a próxima!\n");
            break;
        default:
            printf("\nOpção inválida. Tente novamente.\n");
            break;
        }

    } while (opcao != 0);

    // Limpeza
    liberar_memoria(mapa);
    return 0;
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES DE INICIALIZAÇÃO ---

/**
 * @brief Inicializa a semente do gerador de números aleatórios.
 */
void inicializar_gerador_aleatorio(void)
{
    srand((unsigned int)time(NULL));
}

/**
 * @brief Aloca o mapa e inicializa os territórios com nomes e tropas.
 */
Territorio *criar_mapa_inicial(const int num_territorios)
{
    // Array de nomes para os territórios
    const char *nomes_territorios[] = {
        "Amarelo",
        "Verde",
        "Azul",
        "Branco",
        "Vermelho"
    };

printf("\n--- INICIALIZANDO MAPA ---\n");
for (int i = 0; i < num_territorios; i++)
{
    t[i].id = i + 1;
   
    strncpy(t[i].nome, nomes_territorios[i], sizeof(t[i].nome) - 1);
        t[i].nome[sizeof(t[i].nome) - 1] = '\0'; // Garantir terminação nula

    // Distribuição inicial (Ex: Territórios 1 e 2 para Humano, 3, 4, 5 para Verde)
    if (i < 2)
    {
        t[i].dono_id = ID_JOGADOR_HUMANO;
    }
    else
    {
        t[i].dono_id = ID_JOGADOR_VERDE;
    }
    // Tropas iniciais: valor aleatório de 3 a MAX_TROPAS_INICIAIS
    t[i].tropas = 3 + (rand() % (MAX_TROPAS_INICIAIS - 2));

    printf("[ID %d] %s | Dono: %s | Tropas: %d\n", t[i].id, t[i].nome,
           t[i].dono_id == ID_JOGADOR_HUMANO ? "HUMANO" : "VERDE", t[i].tropas);
}
printf("--------------------------\n");
return t;

void liberar_memoria(Territorio *territorios)
{
    if (territorios != NULL)
    {
        free(territorios);
    }
}


// Linhas 201-232 atualizadas:
Territorio *criar_mapa_inicial(const int num_territorios)
{
    // Array de nomes para os territórios (precisa ter no mínimo NUM_TERRITORIOS nomes)
    const char *nomes_territorios[] = {
        "Amarelo",  
        "Verde",   
        "Azul",   
        "Branco",   
        "Vermelho"     
    };

    Territorio *t = (Territorio *)calloc(num_territorios, sizeof(Territorio)); // ALOCAÇÃO PRIMEIRO

    if (t == NULL)
        return NULL;
       

    printf("\n--- INICIALIZANDO MAPA ---\n");
    for (int i = 0; i < num_territorios; i++)
    {
        t[i].id = i + 1;

        // COPIAR O NOME PERSONALIZADO (AGORA 't' É VÁLIDO)
        strncpy(t[i].nome, nomes_territorios[i], sizeof(t[i].nome) - 1);
        t[i].nome[sizeof(t[i].nome) - 1] = '\0'; // Garantir terminação nula

        // ... restante da lógica ...
        if (i < 2)
        {
            t[i].dono_id = ID_JOGADOR_HUMANO;
        }
        else
        {
            t[i].dono_id = ID_JOGADOR_VERDE;
    }
    t[i].tropas = 3 + (rand() % (MAX_TROPAS_INICIAIS - 2));

        printf("[ID %d] %s | Dono: %s | Tropas: %d\n", t[i].id, t[i].nome,
               t[i].dono_id == ID_JOGADOR_HUMANO ? "HUMANO" : "VERDE", t[i].tropas);
    }
    printf("--------------------------\n");
    // ...
    return t;
}

/**
 * @brief Libera a memória alocada dinamicamente.
 */
void liberar_memoria(Territorio *territorios)
{
    if (territorios != NULL)
    {
        free(territorios);
    }
}


/**
 * @brief Gera uma missão aleatória para o jogador.
 */

Missao gerar_missao_aleatoria(void)
{
    Missao m;
    int tipo_missao = 1 + (rand() % 3); // 1,2 ou 3.

    if (tipo_missao == TIPO_DESTRUIR_VERDE)
    {
        // Missão 1: Destruir o Exército Verde (Reduzir tropas para 0)
        m.tipo = TIPO_DESTRUIR_VERDE;
        m.alvo_id = ID_JOGADOR_VERDE;
        m.valor = 0;
    }

    else if (tipo_missao == TIPO_CONQUISTAR_TOTAL) // NOVO BLOC
    {
        // Missão 2: Conquistar 3 Territórios (Possuir 3 territórios no total)
        m.tipo = TIPO_CONQUISTAR_TOTAL;
        m.alvo_id = ID_JOGADOR_HUMANO; // O alvo é o próprio jogador
        m.valor = 3;
    }

    else
    {
        // Missão 3: Conquistar 3 Territórios 
        m.tipo = TIPO_CONQUISTAR_ESPECIFICO;
        m.alvo_id = 3 + (rand() % (NUM_TERRITORIOS - 2));
        m.valor = m.alvo_id;
        ;
    }
    return m;
}

/**
 * @brief Exibe o objetivo da missão.
 */

void exibir_missao(const Missao *missao, const Territorio *mapa, const int num_territorios)
{
    printf("\n* Sua Missão Atual é: ");
    if (missao->tipo == TIPO_DESTRUIR_VERDE)
    {
        printf("Destruir o EXÉRCITO VERDE (Reduzir o total de tropas do VERDE a 0).\n");
    }
    else if (missao->tipo == TIPO_CONQUISTAR_TOTAL)
    {
        printf("Conquistar e Manter %d Territórios (atualmente você tem %d).\n",
               missao->valor, contar_territorios_do_dono(mapa, num_territorios, ID_JOGADOR_HUMANO)); // Exibe o progresso
    }
    else if (missao->tipo == TIPO_CONQUISTAR_ESPECIFICO)
    {
        const Territorio *alvo = encontrar_territorio_por_id(mapa, num_territorios, missao->valor);
        if (alvo != NULL)
        {
            printf("Conquistar o território ESPECÍFICO: '%s' (ID %d).\n", alvo->nome, alvo->id);
        }
        else
        {
            printf("Conquistar um território específico (ID %d). [Erro ao localizar nome]\n", missao->valor);
        }
    }
}

/**
 * @brief Verifica se a missão foi cumprida.
 * @param missao Ponteiro constante para a missão.
 * @param mapa Ponteiro constante para o mapa.
 * @return 1 se a missão foi cumprida, 0 caso contrário.
 */
int verificar_missao(const Missao *missao, const Territorio *mapa, const int num_territorios)
{

    if (missao->tipo == TIPO_DESTRUIR_VERDE)
    {
        return (contar_tropas_do_dono(mapa, num_territorios, ID_JOGADOR_VERDE) == 0);
    }
    else if (missao->tipo == TIPO_CONQUISTAR_TOTAL)
    {
        return (contar_territorios_do_dono(mapa, num_territorios, ID_JOGADOR_HUMANO) >= missao->valor);
    }
    else if (missao->tipo == TIPO_CONQUISTAR_ESPECIFICO)
    {

        const Territorio *alvo = encontrar_territorio_por_id(mapa, num_territorios, missao->valor);

        return (alvo != NULL && alvo->dono_id == ID_JOGADOR_HUMANO);
    }
    return 0;
}
/**
 * @brief Conta quantos territórios pertencem a um dono específico.
 */
int contar_territorios_do_dono(const Territorio *mapa, const int num_territorios, const int dono_id)
{
    int contador = 0;
    for (int i = 0; i < num_territorios; i++)
    {
        if (mapa[i].dono_id == dono_id)
        {
            contador++;
        }
    }
    return contador;
}

/**
 * @brief Conta o total de tropas de um dono específico.
 */
int contar_tropas_do_dono(const Territorio *mapa, const int num_territorios, const int dono_id)
{
    int total_tropas = 0;
    for (int i = 0; i < num_territorios; i++)
    {
        if (mapa[i].dono_id == dono_id)
        {
            total_tropas += mapa[i].tropas;
        }
    }
    return total_tropas;
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES DE JOGO ---

/**
 * @brief Exibe o estado atual do mapa.
 * @param mapa Ponteiro constante para o array de territórios (não pode ser modificado).
 */
void exibir_mapa(const Territorio *mapa, const int num_territorios)
{
    printf("\n\n*** MAPA ATUAL ***\n");
    for (int i = 0; i < num_territorios; i++)
    {
        // Uso de const correctness no parâmetro
        printf("[ID: %d] %-15s | Dono: %-6s | Tropas: %d\n",
               mapa[i].id, mapa[i].nome,
               mapa[i].dono_id == ID_JOGADOR_HUMANO ? "HUMANO" : "VERDE", mapa[i].tropas);
    }
    printf("******************\n");
}

/**
 * @brief Exibe o menu principal.
 */
void exibir_menu(void)
{
    printf("\n--- MENU ---\n");
    printf("1. Atacar Território\n");
    printf("2. Verificar Missão\n");
    printf("0. Sair do Jogo\n");
    printf("------------\n");
}

/**
 * @brief Simula uma rodada de ataque entre dois territórios.
 * @param atacante Ponteiro para o território atacante (pode ser modificado).
 * @param defensor Ponteiro para o território defensor (pode ser modificado).
 */
void realizar_ataque(Territorio *atacante, Territorio *defensor)
{
    // Verifica se o atacante tem tropas
    if (atacante->tropas <= 1)
    {
        printf("Ataque cancelado: Atacante '%s' precisa de pelo menos 2 tropas para atacar (1 fica de defesa).\n", atacante->nome);
        return;
    }
    if (defensor->tropas <= 0)
    {
        printf("Ataque cancelado: Território já sem tropas.\n");
        return;
    }

    // Dados de batalha (1 a 6)
    int dado_ataque = 1 + (rand() % 6);
    int dado_defesa = 1 + (rand() % 6);

    printf("\n--- Batalha entre %s (HUMANO) vs %s (VERDE) ---\n", atacante->nome, defensor->nome);
    printf("Dados Sorteados -> Ataque: %d | Defesa: %d\n", dado_ataque, dado_defesa);

    // Lógica da Batalha: Empates favorecem o atacante
    if (dado_ataque >= dado_defesa)
    {
        printf("Resultado: ATACANTE VENCEU.\n");
        defensor->tropas--;
        printf("-> O Defensor '%s' perdeu 1 tropa. Tropas restantes: %d\n", defensor->nome, defensor->tropas);

        // Lógica de Conquista
        if (defensor->tropas <= 0)
        {
            printf("\n!!! CONQUISTA !!! O território '%s' foi conquistado pelo HUMANO!\n", defensor->nome);

            // Muda o dono e atribui 1 tropa (Passagem por Referência)
            defensor->dono_id = atacante->dono_id;
            defensor->tropas = 1;
            atacante->tropas--; // O atacante perde 1 tropa para guarnecer o novo território

            printf("-> %s agora é do HUMANO e ficou com 1 tropa do atacante.\n", defensor->nome);
            printf("-> %s perdeu 1 tropa por custo de ocupação. Tropas restantes: %d\n", atacante->nome, atacante->tropas);
        }
    }
    else
    {
        printf("Resultado: DEFENSOR VENCEU.\n");
        atacante->tropas--;
        printf("-> O Atacante '%s' perdeu 1 tropa. Tropas restantes: %d\n", atacante->nome, atacante->tropas);
    }
    printf("--------------------------------------------------\n");
}

/**
 * @brief Coleta as escolhas do jogador e chama a simulação de ataque.
 * @return 1 se o ataque ocorreu, 0 caso contrário.
 */
int processar_ataque(Territorio *mapa, const int num_territorios)
{
    int atacante_id, defensor_id;
    Territorio *atacante, *defensor;

    printf("\nEscolha o Território Atacante (Seu, ID 1 a %d): ", num_territorios);
    if (scanf("%d", &atacante_id) != 1 || atacante_id < 1 || atacante_id > num_territorios)
    {
        printf("ID de atacante inválido.\n");
        while (getchar() != '\n')
            ;
        return 0;
    }

    printf("Escolha o Território Defensor (Inimigo, ID 1 a %d): ", num_territorios);
    if (scanf("%d", &defensor_id) != 1 || defensor_id < 1 || defensor_id > num_territorios)
    {
        printf("ID de defensor inválido.\n");
        while (getchar() != '\n')
            ;
        return 0;
    }

    atacante = &mapa[atacante_id - 1];
    defensor = &mapa[defensor_id - 1];

    if (atacante->dono_id != ID_JOGADOR_HUMANO)
    {
        printf("Erro: O território atacante escolhido não pertence a você.\n");
        return 0;
    }

    if (defensor->dono_id != ID_JOGADOR_VERDE)
    {
        printf("Erro: O território defensor escolhido já é seu ou neutro/inválido.\n");
        return 0;
    }

    if (atacante_id == defensor_id)
    {
        printf("Um território não pode atacar a si mesmo!\n");
        return 0;
    }

    // Chama a simulação de ataque
    realizar_ataque(atacante, defensor);

    printf("\nPressione ENTER para continuar...");
    while (getchar() != '\n')
        ;
    getchar(); // Espera por ENTER

    return 1;
}

const Territorio *encontrar_territorio_por_id(const Territorio *mapa, const int num_territorios, const int id_alvo)
{
    for (int i = 0; i < num_territorios; i++)
    {
        // Uso de const: retorna um ponteiro que não pode modificar a estrutura.
        if (mapa[i].id == id_alvo)
        {
            return &mapa[i];
        }
    }
    return NULL;
}