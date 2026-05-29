// ============================================================================
//                PROJETO WAR ESTRUTURADO - NÍVEL MESTRE
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================================================
// STRUCT TERRITORIO
// ============================================================================
struct Territorio {

    char nome[30];
    char cor[15];
    int tropas;
};

// ============================================================================
// PROTÓTIPOS DAS FUNÇÕES
// ============================================================================
void limparBufferEntrada();

void pausarJogo();

void exibirMenu();

void exibirMissaoJogador(char* corJogador,
                         char* missao);

struct Territorio* alocarMapa(int quantidade);

void cadastrarTerritorios(struct Territorio* mapa,
                          int quantidade);

void exibirMapa(const struct Territorio* mapa,
                int quantidade);

void atacar(struct Territorio* atacante,
            struct Territorio* defensor);

void liberarMemoria(struct Territorio* mapa,
                    char* missaoJogador);

void atribuirMissao(char* destino,
                    char* missoes[],
                    int totalMissoes);

void exibirMissao(const char* missao);

int verificarMissao(char* missao,
                    struct Territorio* mapa,
                    int tamanho,
                    char* corJogador);

// ============================================================================
// FUNÇÃO PRINCIPAL
// ============================================================================
int main() {

    srand(time(NULL));

    int quantidade;
    int atacanteIndex;
    int defensorIndex;
    int opcao;

    // =========================================================================
    // COR DO JOGADOR
    // =========================================================================
    char corJogador[15];

    // =========================================================================
    // MISSÕES
    // =========================================================================
    char* missoes[] = {

        "Conquistar 3 territorios",
        "Conquistar 5 territorios",
        "Dominar metade do mapa",
        "Eliminar uma cor inimiga",
        "Possuir pelo menos 20 tropas"
    };

    int totalMissoes = 5;

    // =========================================================================
    // TÍTULO
    // =========================================================================
    printf("=====================================\n");
    printf("      WAR ESTRUTURADO - MESTRE\n");
    printf("=====================================\n\n");

    // =========================================================================
    // QUANTIDADE DE TERRITÓRIOS
    // =========================================================================
    printf("Digite a quantidade de territorios: ");
    scanf("%d", &quantidade);

    limparBufferEntrada();

    // =========================================================================
    // COR DO JOGADOR
    // =========================================================================
    printf("Digite a cor do jogador: ");
    fgets(corJogador, 15, stdin);

    corJogador[strcspn(corJogador,
                       "\n")] = '\0';

    // =========================================================================
    // ALOCA MAPA
    // =========================================================================
    struct Territorio* mapa =
        alocarMapa(quantidade);

    if (mapa == NULL) {

        printf("Erro ao alocar memoria.\n");

        return 1;
    }

    // =========================================================================
    // CADASTRO
    // =========================================================================
    cadastrarTerritorios(mapa,
                         quantidade);

    // =========================================================================
    // ALOCA MISSÃO
    // =========================================================================
    char* missaoJogador =
        (char*) malloc(100 * sizeof(char));

    if (missaoJogador == NULL) {

        printf("Erro ao alocar memoria da missao.\n");

        free(mapa);

        return 1;
    }

    // =========================================================================
    // ATRIBUI MISSÃO
    // =========================================================================
    atribuirMissao(missaoJogador,
                   missoes,
                   totalMissoes);

    // =========================================================================
    // LOOP PRINCIPAL
    // =========================================================================
    do {

        system("cls || clear");

        // =========================================================================
        // MAPA
        // =========================================================================
        printf("\n=====================================\n");
        printf("             MAPA DO MUNDO\n");
        printf("=====================================\n");

        exibirMapa(mapa,
                   quantidade);

        // =========================================================================
        // MISSÃO
        // =========================================================================
        exibirMissaoJogador(corJogador,
                            missaoJogador);

        // =========================================================================
        // MENU
        // =========================================================================
        exibirMenu();

        printf("\nEscolha sua acao: ");
        scanf("%d", &opcao);

        limparBufferEntrada();

        switch (opcao) {

            // =============================================================
            // ATACAR
            // =============================================================
            case 1:

                printf("\nEscolha o territorio atacante: ");
                scanf("%d", &atacanteIndex);

                printf("Escolha o territorio defensor: ");
                scanf("%d", &defensorIndex);

                limparBufferEntrada();

                // =============================================================
                // VALIDAÇÕES
                // =============================================================
                if (atacanteIndex < 0 ||
                    atacanteIndex >= quantidade ||
                    defensorIndex < 0 ||
                    defensorIndex >= quantidade) {

                    printf("\nIndices invalidos.\n");

                    pausarJogo();

                    continue;
                }

                if (atacanteIndex == defensorIndex) {

                    printf("\nVoce nao pode atacar o mesmo territorio.\n");

                    pausarJogo();

                    continue;
                }

                if (strcmp(mapa[atacanteIndex].cor,
                           mapa[defensorIndex].cor) == 0) {

                    printf("\nNao e permitido atacar territorios aliados.\n");

                    pausarJogo();

                    continue;
                }

                if (mapa[atacanteIndex].tropas <= 1) {

                    printf("\nO atacante precisa ter mais de 1 tropa.\n");

                    pausarJogo();

                    continue;
                }

                // =============================================================
                // ATAQUE
                // =============================================================
                atacar(&mapa[atacanteIndex],
                        &mapa[defensorIndex]);

                // =============================================================
                // VERIFICA MISSÃO
                // =============================================================
                if (verificarMissao(missaoJogador,
                                    mapa,
                                    quantidade,
                                    corJogador)) {

                    printf("\n=====================================\n");
                    printf("         MISSAO CUMPRIDA!\n");
                    printf("         VOCE VENCEU!\n");
                    printf("=====================================\n");

                    pausarJogo();

                    opcao = 0;
                }

                pausarJogo();

                break;

            // =============================================================
            // MOSTRAR MISSÃO
            // =============================================================
            case 2:

                exibirMissaoJogador(corJogador,
                                    missaoJogador);

                pausarJogo();

                break;

            // =============================================================
            // SAIR
            // =============================================================
            case 0:

                printf("\nSaindo do jogo...\n");

                break;

            // =============================================================
            // OPÇÃO INVÁLIDA
            // =============================================================
            default:

                printf("\nOpcao invalida.\n");

                pausarJogo();
        }

    } while (opcao != 0);

    // =========================================================================
    // MAPA FINAL
    // =========================================================================
    printf("\n=====================================\n");
    printf("             MAPA FINAL\n");
    printf("=====================================\n");

    exibirMapa(mapa,
               quantidade);

    // =========================================================================
    // LIBERA MEMÓRIA
    // =========================================================================
    liberarMemoria(mapa,
                   missaoJogador);

    return 0;
}

// ============================================================================
// LIMPA BUFFER
// ============================================================================
void limparBufferEntrada() {

    int c;

    while ((c = getchar()) != '\n' &&
           c != EOF);
}

// ============================================================================
// PAUSAR JOGO
// ============================================================================
void pausarJogo() {

    printf("\nPressione Enter para continuar...");
    getchar();
}

// ============================================================================
// MENU
// ============================================================================
void exibirMenu() {

    printf("\n=====================================\n");
    printf("            MENU DE ACOES\n");
    printf("=====================================\n");

    printf("1 - Atacar\n");
    printf("2 - Verificar Missao\n");
    printf("0 - Sair\n");
}

// ============================================================================
// MISSÃO DO JOGADOR
// ============================================================================
void exibirMissaoJogador(char* corJogador,
                         char* missao) {

    printf("\n--- SUA MISSAO (%s) ---\n",
           corJogador);

    printf("%s\n",
           missao);
}

// ============================================================================
// ALOCA MAPA
// ============================================================================
struct Territorio* alocarMapa(int quantidade) {

    struct Territorio* mapa;

    mapa = (struct Territorio*)
           calloc(quantidade,
                  sizeof(struct Territorio));

    return mapa;
}

// ============================================================================
// CADASTRA TERRITÓRIOS
// ============================================================================
void cadastrarTerritorios(struct Territorio* mapa,
                          int quantidade) {

    int i;

    printf("\n=====================================\n");
    printf("      CADASTRO DE TERRITORIOS\n");
    printf("=====================================\n\n");

    for (i = 0; i < quantidade; i++) {

        printf("Territorio %d\n", i);

        // Nome
        printf("Digite o nome do territorio: ");
        fgets(mapa[i].nome,
              30,
              stdin);

        mapa[i].nome[strcspn(mapa[i].nome,
                             "\n")] = '\0';

        // Cor
        printf("Digite a cor do exercito: ");
        fgets(mapa[i].cor,
              15,
              stdin);

        mapa[i].cor[strcspn(mapa[i].cor,
                            "\n")] = '\0';

        // Tropas
        printf("Digite a quantidade de tropas: ");

        if (scanf("%d",
                  &mapa[i].tropas) != 1) {

            printf("\nValor invalido.\n");
            printf("Definindo tropas como 1.\n");

            mapa[i].tropas = 1;
        }

        limparBufferEntrada();

        printf("\n");
    }
}

// ============================================================================
// EXIBE MAPA
// ============================================================================
void exibirMapa(const struct Territorio* mapa,
                int quantidade) {

    int i;

    for (i = 0; i < quantidade; i++) {

        printf("%d. %-15s",
               i,
               mapa[i].nome);

        printf("(Exercito: %-10s",
               mapa[i].cor);

        printf(", Tropas: %d)\n",
               mapa[i].tropas);
    }

    printf("=====================================\n");
}

// ============================================================================
// ATAQUE
// ============================================================================
void atacar(struct Territorio* atacante,
            struct Territorio* defensor) {

    int dadoAtacante;
    int dadoDefensor;

    dadoAtacante = rand() % 6 + 1;
    dadoDefensor = rand() % 6 + 1;

    printf("\n=====================================\n");
    printf("               BATALHA\n");
    printf("=====================================\n");

    printf("%s atacando %s\n",
           atacante->nome,
           defensor->nome);

    printf("\nDado atacante: %d\n",
           dadoAtacante);

    printf("Dado defensor: %d\n",
           dadoDefensor);

    // =========================================================================
    // VITÓRIA DO ATACANTE
    // =========================================================================
    if (dadoAtacante > dadoDefensor) {

        printf("\nVITORIA DO ATAQUE!\n");

        atacante->tropas--;

        strcpy(defensor->cor,
               atacante->cor);

        defensor->tropas =
            atacante->tropas / 2;

        atacante->tropas -=
            defensor->tropas;

        if (defensor->tropas < 1) {

            defensor->tropas = 1;
        }

    } else {

        printf("\nVITORIA DA DEFESA! ");
        printf("O atacante perdeu 1 tropa.\n");

        atacante->tropas--;

        if (atacante->tropas < 1) {

            atacante->tropas = 1;
        }
    }
}

// ============================================================================
// ATRIBUI MISSÃO
// ============================================================================
void atribuirMissao(char* destino,
                    char* missoes[],
                    int totalMissoes) {

    int indice;

    indice = rand() % totalMissoes;

    strcpy(destino,
           missoes[indice]);
}

// ============================================================================
// EXIBE MISSÃO
// ============================================================================
void exibirMissao(const char* missao) {

    printf("\n=====================================\n");
    printf("         SUA MISSAO SECRETA\n");
    printf("=====================================\n");

    printf("%s\n",
           missao);

    printf("=====================================\n");
}

// ============================================================================
// VERIFICA MISSÃO
// ============================================================================
int verificarMissao(char* missao,
                    struct Territorio* mapa,
                    int tamanho,
                    char* corJogador) {

    int i;

    int territoriosJogador = 0;
    int totalTropas = 0;

    for (i = 0; i < tamanho; i++) {

        if (strcmp(mapa[i].cor,
                   corJogador) == 0) {

            territoriosJogador++;

            totalTropas += mapa[i].tropas;
        }
    }

    // =========================================================================
    // CONQUISTAR 3 TERRITÓRIOS
    // =========================================================================
    if (strcmp(missao,
        "Conquistar 3 territorios") == 0) {

        return territoriosJogador >= 3;
    }

    // =========================================================================
    // CONQUISTAR 5 TERRITÓRIOS
    // =========================================================================
    if (strcmp(missao,
        "Conquistar 5 territorios") == 0) {

        return territoriosJogador >= 5;
    }

    // =========================================================================
    // DOMINAR METADE DO MAPA
    // =========================================================================
    if (strcmp(missao,
        "Dominar metade do mapa") == 0) {

        return territoriosJogador >= tamanho / 2;
    }

    // =========================================================================
    // POSSUIR 20 TROPAS
    // =========================================================================
    if (strcmp(missao,
        "Possuir pelo menos 20 tropas") == 0) {

        return totalTropas >= 20;
    }

    // =========================================================================
    // ELIMINAR UMA COR INIMIGA
    // =========================================================================
    if (strcmp(missao,
        "Eliminar uma cor inimiga") == 0) {

        return territoriosJogador == tamanho;
    }

    return 0;
}

// ============================================================================
// LIBERA MEMÓRIA
// ============================================================================
void liberarMemoria(struct Territorio* mapa,
                    char* missaoJogador) {

    free(mapa);

    free(missaoJogador);

    printf("\nMemoria liberada com sucesso.\n");
}