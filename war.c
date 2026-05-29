// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO AVENTUREIRO
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================================================
// Struct Territorio
// Armazena os dados de cada território do jogo
// ============================================================================
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// ============================================================================
// Protótipos das funções
// ============================================================================
void limparBufferEntrada();
struct Territorio* alocarMapa(int quantidade);
void cadastrarTerritorios(struct Territorio* mapa, int quantidade);
void exibirMapa(const struct Territorio* mapa, int quantidade);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void liberarMemoria(struct Territorio* mapa);

// ============================================================================
// Função principal
// ============================================================================
int main() {

    // Inicializa números aleatórios
    srand(time(NULL));

    int quantidade;
    int atacanteIndex;
    int defensorIndex;
    char continuar = 's';

    printf("=== WAR ESTRUTURADO ===\n\n");

    // Quantidade de territórios
    printf("Digite a quantidade de territorios: ");
    scanf("%d", &quantidade);

    limparBufferEntrada();

    // Alocação dinâmica de memória
    struct Territorio* mapa = alocarMapa(quantidade);

    // Verifica se a memória foi alocada corretamente
    if (mapa == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    // Cadastro dos territórios
    cadastrarTerritorios(mapa, quantidade);

    // Loop principal de ataques
    do {

        printf("\n=== MAPA ATUAL ===\n");
        exibirMapa(mapa, quantidade);

        // Escolha do território atacante
        printf("\nEscolha o territorio atacante (0 a %d): ", quantidade - 1);
        scanf("%d", &atacanteIndex);

        // Escolha do território defensor
        printf("Escolha o territorio defensor (0 a %d): ", quantidade - 1);
        scanf("%d", &defensorIndex);

        limparBufferEntrada();

        // Validações
        if (atacanteIndex < 0 || atacanteIndex >= quantidade ||
            defensorIndex < 0 || defensorIndex >= quantidade) {

            printf("Indice invalido.\n");
            continue;
        }

        // Não pode atacar território da mesma cor
        if (strcmp(mapa[atacanteIndex].cor,
                   mapa[defensorIndex].cor) == 0) {

            printf("Nao e permitido atacar territorios da mesma cor.\n");
            continue;
        }

        // Verifica tropas suficientes
        if (mapa[atacanteIndex].tropas <= 1) {
            printf("O atacante precisa ter mais de 1 tropa.\n");
            continue;
        }

        // Executa ataque
        atacar(&mapa[atacanteIndex], &mapa[defensorIndex]);

        printf("\nDeseja realizar outro ataque? (s/n): ");
        scanf("%c", &continuar);

        limparBufferEntrada();

    } while (continuar == 's' || continuar == 'S');

    // Exibe estado final
    printf("\n=== ESTADO FINAL DO MAPA ===\n");
    exibirMapa(mapa, quantidade);

    // Libera memória
    liberarMemoria(mapa);

    return 0;
}

// ============================================================================
// Função para limpar o buffer de entrada
// ============================================================================
void limparBufferEntrada() {
    int c;

    while ((c = getchar()) != '\n' && c != EOF);
}

// ============================================================================
// Aloca memória dinamicamente para o mapa
// ============================================================================
struct Territorio* alocarMapa(int quantidade) {

    struct Territorio* mapa;

    mapa = (struct Territorio*) calloc(quantidade,
                                       sizeof(struct Territorio));

    return mapa;
}

// ============================================================================
// Cadastro dos territórios
// ============================================================================
void cadastrarTerritorios(struct Territorio* mapa, int quantidade) {

    int i;

    printf("\n=== CADASTRO DE TERRITORIOS ===\n\n");

    for (i = 0; i < quantidade; i++) {

        printf("Territorio %d\n", i);

        // Nome
        printf("Digite o nome do territorio: ");
        fgets(mapa[i].nome, 30, stdin);

        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        // Cor
        printf("Digite a cor do exercito: ");
        fgets(mapa[i].cor, 10, stdin);

        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        // Tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);

        limparBufferEntrada();

        printf("\n");
    }
}

// ============================================================================
// Exibe todos os territórios
// ============================================================================
void exibirMapa(const struct Territorio* mapa, int quantidade) {

    int i;

    for (i = 0; i < quantidade; i++) {

        printf("\n[%d]\n", i);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);

        printf("-------------------------\n");
    }
}

// ============================================================================
// Função de ataque
// Simula batalha utilizando dados aleatórios
// ============================================================================
void atacar(struct Territorio* atacante,
            struct Territorio* defensor) {

    int dadoAtacante;
    int dadoDefensor;

    // Sorteio dos dados
    dadoAtacante = rand() % 6 + 1;
    dadoDefensor = rand() % 6 + 1;

    printf("\n=== BATALHA ===\n");

    printf("%s atacando %s\n",
           atacante->nome,
           defensor->nome);

    printf("Dado atacante: %d\n", dadoAtacante);
    printf("Dado defensor: %d\n", dadoDefensor);

    // Vitória do atacante
    if (dadoAtacante > dadoDefensor) {

        printf("\nO atacante venceu!\n");

        // Perde uma tropa para ocupar território
        atacante->tropas--;

        // Território muda de dono
        strcpy(defensor->cor, atacante->cor);

        // Metade das tropas do atacante ocupa território
        defensor->tropas = atacante->tropas / 2;

        // Remove tropas enviadas
        atacante->tropas -= defensor->tropas;

        // Garante mínimo de 1 tropa
        if (defensor->tropas < 1) {
            defensor->tropas = 1;
        }

    } else {

        // Derrota do atacante
        printf("\nO defensor venceu!\n");

        atacante->tropas--;

        // Garante pelo menos 1 tropa
        if (atacante->tropas < 1) {
            atacante->tropas = 1;
        }
    }
}

// ============================================================================
// Libera memória alocada
// ============================================================================
void liberarMemoria(struct Territorio* mapa) {

    free(mapa);

    printf("\nMemoria liberada com sucesso.\n");
}