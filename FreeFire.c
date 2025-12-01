#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 1) Definição da struct Item
// Eu crio a struct Item com os campos nome, tipo e quantidade.
// - nome: até 29 caracteres + terminador
// - tipo: até 19 caracteres + terminador
// - quantidade: inteiro que representa quantas unidades do item
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// 2) Constante para capacidade máxima da mochila
// Eu defino a capacidade MAX_ITENS = 10 conforme especificado.
#define MAX_ITENS 10

// 3) Protótipos das funções obrigatórias
// Eu declaro as funções que vou implementar abaixo.
void inserirItem(Item mochila[], int *qtd);
void removerItem(Item mochila[], int *qtd);
void listarItens(const Item mochila[], int qtd);
int buscarItem(const Item mochila[], int qtd, const char *nome);

// Função auxiliar: leLinha
// Eu crio uma função para ler uma linha do stdin com fgets e remover o '\n'.
void leLinha(char *dest, int tamanho) {
    if (fgets(dest, tamanho, stdin) == NULL) {
        // Em caso de erro, coloco string vazia
        dest[0] = '\0';
        return;
    }
    // Removo o '\n' final, se existir
    size_t len = strlen(dest);
    if (len > 0 && dest[len - 1] == '\n') {
        dest[len - 1] = '\0';
    }
}

int main(void) {
    // 4) Vetor estático que representa a mochila
    // Eu crio um array de Item com capacidade para MAX_ITENS.
    Item mochila[MAX_ITENS];
    int quantidadeItens = 0; // Eu mantenho a quantidade atual de itens na mochila

    int opcao;

    // 5) Menu interativo usando do-while e switch
    // Eu apresento as opções ao usuário e repito até ele escolher sair (0).
    do {
        printf("\n=== Desafio Código da Ilha - Mochila Virtual ===\n");
        printf("1. Inserir item\n");
        printf("2. Remover item por nome\n");
        printf("3. Buscar item por nome\n");
        printf("4. Listar itens\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");

        // Leio a opção com scanf e limpo o buffer de entrada
        if (scanf("%d", &opcao) != 1) {
            // Se leitura falhar, limpo entrada e continuo
            printf("Entrada inválida. Tentando novamente...\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {} // descarto até newline
            opcao = -1; // forço repetir
            continue;
        }
        // descarto o '\n' remanescente após scanf
        int c; while ((c = getchar()) != '\n' && c != EOF) {}

        switch (opcao) {
            case 1:
                inserirItem(mochila, &quantidadeItens);
                listarItens(mochila, quantidadeItens); // exibo após operação
                break;
            case 2:
                removerItem(mochila, &quantidadeItens);
                listarItens(mochila, quantidadeItens); // exibo após operação
                break;
            case 3: {
                char nomeBusca[30];
                printf("Digite o nome do item para buscar: ");
                leLinha(nomeBusca, sizeof(nomeBusca));
                int idx = buscarItem(mochila, quantidadeItens, nomeBusca);
                if (idx >= 0) {
                    printf("\nItem encontrado (posição %d):\n", idx + 1);
                    printf("  Nome: %s\n", mochila[idx].nome);
                    printf("  Tipo: %s\n", mochila[idx].tipo);
                    printf("  Quantidade: %d\n", mochila[idx].quantidade);
                } else {
                    printf("\nItem '%s' não encontrado na mochila.\n", nomeBusca);
                }
                break;
            }
            case 4:
                listarItens(mochila, quantidadeItens);
                break;
            case 0:
                printf("Saindo... Boa sorte na ilha!\n");
                break;
            default:
                printf("Opção inválida. Escolha novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// Implementação das funções

// inserirItem: insere um novo item na mochila se houver espaço
void inserirItem(Item mochila[], int *qtd) {
    // Eu verifico se há espaço disponível
    if (*qtd >= MAX_ITENS) {
        printf("Mochila cheia! Não é possível inserir mais itens.\n");
        return;
    }

    // Preparar variáveis temporárias para leitura
    char buffer[100];

    printf("\n--- Inserir novo item ---\n");

    // Leio o nome
    printf("Nome do item: ");
    leLinha(buffer, sizeof(buffer));
    // Eu copio o texto para o campo nome do próximo slot
    strncpy(mochila[*qtd].nome, buffer, sizeof(mochila[*qtd].nome) - 1);
    mochila[*qtd].nome[sizeof(mochila[*qtd].nome) - 1] = '\0';

    // Leio o tipo
    printf("Tipo do item (ex: arma, munição, cura): ");
    leLinha(buffer, sizeof(buffer));
    strncpy(mochila[*qtd].tipo, buffer, sizeof(mochila[*qtd].tipo) - 1);
    mochila[*qtd].tipo[sizeof(mochila[*qtd].tipo) - 1] = '\0';

    // Leio e valido a quantidade
    while (1) {
        printf("Quantidade: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Erro de leitura. Quantidade definida como 0.\n");
            mochila[*qtd].quantidade = 0;
            break;
        }
        // Tento converter para inteiro
        int val;
        if (sscanf(buffer, "%d", &val) == 1) {
            if (val < 0) {
                printf("Quantidade não pode ser negativa. Tente novamente.\n");
                continue;
            }
            mochila[*qtd].quantidade = val;
            break;
        } else {
            printf("Entrada inválida. Digite um número inteiro.\n");
        }
    }

    // Finalmente incremento a quantidade de itens
    (*qtd)++;
    printf("Item inserido com sucesso!\n");
}

// buscarItem: busca sequencial pelo nome e retorna o índice ou -1 se não encontrado
int buscarItem(const Item mochila[], int qtd, const char *nome) {
    // Eu percorro o vetor do início até qtd-1 comparando os nomes
    for (int i = 0; i < qtd; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            return i; // encontrado
        }
    }
    return -1; // não encontrado
}

// removerItem: remove um item pelo nome, deslocando os seguintes para preencher o espaço
void removerItem(Item mochila[], int *qtd) {
    if (*qtd == 0) {
        printf("A mochila está vazia. Nada a remover.\n");
        return;
    }

    char nomeRemover[30];
    printf("Digite o nome do item a ser removido: ");
    leLinha(nomeRemover, sizeof(nomeRemover));

    int idx = buscarItem(mochila, *qtd, nomeRemover);
    if (idx < 0) {
        printf("Item '%s' não encontrado. Nenhum item foi removido.\n", nomeRemover);
        return;
    }

    // Eu removo o item deslocando os elementos posteriores uma posição para trás
    for (int i = idx; i < (*qtd) - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    // Opcional: limpar o último slot (boa prática)
    mochila[*qtd - 1].nome[0] = '\0';
    mochila[*qtd - 1].tipo[0] = '\0';
    mochila[*qtd - 1].quantidade = 0;

    (*qtd)--; // decremento a contagem
    printf("Item '%s' removido com sucesso.\n", nomeRemover);
}

// listarItens: exibe uma tabela simples com os itens existentes
void listarItens(const Item mochila[], int qtd) {
    printf("\n--- Conteúdo da Mochila (totais: %d/%d) ---\n", qtd, MAX_ITENS);
    if (qtd == 0) {
        printf("(vazia)\n");
        return;
    }

    // Cabeçalho da tabela
    printf("%-3s | %-29s | %-19s | %-9s\n", "#", "Nome", "Tipo", "Quantidade");
    printf("-----+--------------------------------+---------------------+-----------\n");

    for (int i = 0; i < qtd; i++) {
        // Exibo cada item em uma linha formatada
        printf("%-3d | %-29s | %-19s | %-9d\n", i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

