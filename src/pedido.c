/*
  modulo_pedidos_corrigido.c
  Módulo de Pedidos (sem clientes) - versão corrigida e mais robusta.
  Integra com seu arquivo produtos.txt (mesmo formato que você já usa).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQ_PEDIDOS "pedidos.txt"
#define ARQ_ITENS   "itens_pedido.txt"
#define ARQ_PRODUTOS "produtos.txt"
#define TMP_FILE "tmp_pedidos.tmp"

void limpar_buffer_p() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* -----------------------------------------------------------------
   Funções utilitárias: verificar existência de produto/pedido
   ----------------------------------------------------------------- */
int produtoExiste_p(int codigo) {
    FILE *arq = fopen(ARQ_PRODUTOS, "r");
    char linha[256];
    int lido;

    if (!arq) return 0;

    while (fgets(linha, sizeof(linha), arq)) {
        if (sscanf(linha, "%d", &lido) == 1) {
            if (lido == codigo) {
                fclose(arq);
                return 1;
            }
        }
    }
    fclose(arq);
    return 0;
}

int pedidoExiste_p(int id) {
    FILE *arq = fopen(ARQ_PEDIDOS, "r");
    char linha[256];
    int idLido;

    if (!arq) return 0;

    while (fgets(linha, sizeof(linha), arq)) {
        if (sscanf(linha, "ID:%d", &idLido) == 1) {
            if (idLido == id) {
                fclose(arq);
                return 1;
            }
        }
    }
    fclose(arq);
    return 0;
}

/* -----------------------------------------------------------------
   Gera ID automático para pedido (se arquivo não existir retorna 1)
   ----------------------------------------------------------------- */
int gerarIdPedido_p() {
    FILE *arq = fopen(ARQ_PEDIDOS, "r");
    char linha[256];
    int ultimo = 0;

    if (!arq) return 1; // primeiro id = 1

    while (fgets(linha, sizeof(linha), arq)) {
        int id;
        if (sscanf(linha, "ID:%d", &id) == 1) {
            if (id > ultimo) ultimo = id;
        }
    }
    fclose(arq);
    return ultimo + 1;
}

/* -----------------------------------------------------------------
   CADASTRAR PEDIDO
   Formato gravado:
   ID:<id>
   DATA:<texto>
   TOTAL:<valor (atualmente 0)>
   ---
   ----------------------------------------------------------------- */
int cadastrarPedido_p() {
    int id = gerarIdPedido_p();
    char data[32];

    printf("\n--- CADASTRAR PEDIDO ---\n");
    limpar_buffer_p(); // garante buffer limpo antes do fgets
    printf("Data (dd/mm/aaaa): ");
    if (!fgets(data, sizeof(data), stdin)) {
        printf("Erro lendo data.\n");
        return 0;
    }
    data[strcspn(data, "\n")] = 0;

    FILE *arq = fopen(ARQ_PEDIDOS, "a");
    if (!arq) {
        perror("Erro abrindo arquivo de pedidos (a)");
        return 0;
    }

    fprintf(arq, "ID:%d\nDATA:%s\nTOTAL:0\n---\n", id, data);
    fclose(arq);

    printf("Pedido criado com sucesso! ID = %d\n", id);
    return id;
}

/* -----------------------------------------------------------------
   LISTAR TODOS PEDIDOS
   ----------------------------------------------------------------- */
void listarPedidos_p() {
    FILE *arq = fopen(ARQ_PEDIDOS, "r");
    char linha[256];

    if (!arq) {
        printf("Nenhum pedido encontrado (arquivo ausente).\n");
        return;
    }

    printf("\n--- LISTA DE PEDIDOS ---\n");
    while (fgets(linha, sizeof(linha), arq)) {
        printf("%s", linha);
    }
    fclose(arq);
}

/* -----------------------------------------------------------------
   CONSULTAR PEDIDO (mostra bloco do pedido e itens associados)
   ----------------------------------------------------------------- */
void consultarPedido_p() {
    int id;
    printf("ID do pedido: ");
    if (scanf("%d", &id) != 1) {
        printf("Entrada invalida.\n");
        limpar_buffer_p();
        return;
    }

    FILE *arq = fopen(ARQ_PEDIDOS, "r");
    char linha[256];
    int achou = 0;
    int idLido = 0;

    if (!arq) {
        printf("Nenhum pedido cadastrado.\n");
        return;
    }

    printf("\n--- DADOS DO PEDIDO ---\n");
    while (fgets(linha, sizeof(linha), arq)) {
        if (sscanf(linha, "ID:%d", &idLido) == 1) {
            if (idLido == id) achou = 1;
            else achou = 0;
        }
        if (achou) {
            printf("%s", linha);
            if (strcmp(linha, "---\n") == 0) break;
        }
    }
    fclose(arq);

    if (!achou) {
        printf("Pedido %d nao encontrado.\n", id);
        return;
    }

    // mostra itens (se existirem)
    FILE *it = fopen(ARQ_ITENS, "r");
    if (!it) {
        printf("\nNenhum item cadastrado para este pedido.\n");
        return;
    }

    printf("\n--- ITENS DO PEDIDO ---\n");
    int ped, prod, quant;
    double subtotal;
    int encontrouItens = 0;
    while (fgets(linha, sizeof(linha), it)) {
        if (sscanf(linha, "%d %d %d %lf", &ped, &prod, &quant, &subtotal) == 4) {
            if (ped == id) {
                printf("Produto %d | Quantidade %d | Subtotal %.2f\n", prod, quant, subtotal);
                encontrouItens = 1;
            }
        }
    }
    if (!encontrouItens) printf("Sem itens para este pedido.\n");
    fclose(it);
}

/* -----------------------------------------------------------------
   Recupera preco do produto no arquivo produtos.txt
   Formato esperado (seu formato):
   linha1: ---------- separador
   linha2: <codigo>
   linha3: <descricao>
   linha4: <valor>    <-- aqui está o preço
   linha5: <quantidade>
   linha6: ---------- separador
   Atenção: sua implementação atual grava: separador, codigo, desc, valor, quantidade, separador.
   Esta função busca a linha com o codigo e lê a próxima linha que contém descricao e depois lê a linha do valor.
   ----------------------------------------------------------------- */
int recuperarPrecoProduto(int codigo, double *preco_out) {
    FILE *prod = fopen(ARQ_PRODUTOS, "r");
    char linha[256];
    if (!prod) return 0;

    while (fgets(linha, sizeof(linha), prod)) {
        int c;
        if (sscanf(linha, "%d", &c) == 1 && c == codigo) {
            // próxima linha = descrição (consumir)
            if (!fgets(linha, sizeof(linha), prod)) { fclose(prod); return 0; }
            // próxima linha = valor
            if (!fgets(linha, sizeof(linha), prod)) { fclose(prod); return 0; }
            // tentar ler valor como double (pode ser int)
            double v = 0;
            if (sscanf(linha, "%lf", &v) == 1) {
                *preco_out = v;
                fclose(prod);
                return 1;
            } else {
                fclose(prod);
                return 0;
            }
        }
    }

    fclose(prod);
    return 0;
}

/* -----------------------------------------------------------------
   ADICIONAR ITEM AO PEDIDO (validando pedido e produto)
   Formato do arquivo de itens: "<pedidoId> <produtoId> <quantidade> <subtotal>\n"
   ----------------------------------------------------------------- */
void adicionarItem_p() {
    int pedidoID, produtoID, quantidade;

    printf("ID do pedido: ");
    if (scanf("%d", &pedidoID) != 1) { limpar_buffer_p(); printf("Entrada invalida.\n"); return; }

    if (!pedidoExiste_p(pedidoID)) {
        printf("Pedido %d nao existe. Crie o pedido antes de adicionar itens.\n", pedidoID);
        return;
    }

    printf("Codigo do produto: ");
    if (scanf("%d", &produtoID) != 1) { limpar_buffer_p(); printf("Entrada invalida.\n"); return; }

    if (!produtoExiste_p(produtoID)) {
        printf("Produto %d nao existe (verifique produtos.txt).\n", produtoID);
        return;
    }

    printf("Quantidade: ");
    if (scanf("%d", &quantidade) != 1 || quantidade <= 0) {
        limpar_buffer_p();
        printf("Quantidade invalida.\n");
        return;
    }

    double preco;
    if (!recuperarPrecoProduto(produtoID, &preco)) {
        printf("Nao foi possivel recuperar preco do produto %d.\n", produtoID);
        return;
    }

    double subtotal = preco * quantidade;

    FILE *it = fopen(ARQ_ITENS, "a");
    if (!it) {
        perror("Erro abrindo arquivo de itens (a)");
        return;
    }
    fprintf(it, "%d %d %d %.2f\n", pedidoID, produtoID, quantidade, subtotal);
    fclose(it);

    printf("Item adicionado com sucesso! Subtotal: %.2f\n", subtotal);
}

/* -----------------------------------------------------------------
   REMOVER PEDIDO COMPLETO + remover itens associados
   ----------------------------------------------------------------- */
void removerPedido_p() {
    int id;
    printf("ID do pedido a remover: ");
    if (scanf("%d", &id) != 1) { limpar_buffer_p(); printf("Entrada invalida.\n"); return; }

    FILE *orig = fopen(ARQ_PEDIDOS, "r");
    if (!orig) {
        printf("Arquivo de pedidos inexistente.\n");
    } else {
        FILE *temp = fopen(TMP_FILE, "w");
        if (!temp) { perror("Erro criando arquivo temporario"); fclose(orig); return; }

        char linha[256];
        int pulando = 0, idLido = 0;

        while (fgets(linha, sizeof(linha), orig)) {
            if (sscanf(linha, "ID:%d", &idLido) == 1) {
                if (idLido == id) pulando = 1;
                else pulando = 0;
            }
            if (pulando) {
                // se for a linha que marca fim do bloco, para pular e não escrever o separador
                if (strcmp(linha, "---\n") == 0) { pulando = 0; continue; }
                else continue;
            } else {
                fprintf(temp, "%s", linha);
            }
        }

        fclose(orig);
        fclose(temp);

        // substitui arquivo somente se tmp foi criado
        if (remove(ARQ_PEDIDOS) != 0) {
            // pode ser que o arquivo original tenha sido removido entre tempo; ignoramos
        }
        rename(TMP_FILE, ARQ_PEDIDOS);
        printf("Bloco do pedido removido (se existia).\n");
    }

    // remover itens associados
    FILE *it = fopen(ARQ_ITENS, "r");
    if (!it) {
        printf("Arquivo de itens inexistente ou sem itens.\n");
        return;
    }
    FILE *tmp2 = fopen(TMP_FILE, "w");
    if (!tmp2) { perror("Erro tmp2"); fclose(it); return; }

    char linha[256];
    while (fgets(linha, sizeof(linha), it)) {
        int ped;
        if (sscanf(linha, "%d", &ped) == 1) {
            if (ped != id) fprintf(tmp2, "%s", linha);
        } else {
            // linha inválida: copia para segurança
            fprintf(tmp2, "%s", linha);
        }
    }
    fclose(it);
    fclose(tmp2);

    if (remove(ARQ_ITENS) != 0) {
        // ok se não existir
    }
    rename(TMP_FILE, ARQ_ITENS);

    printf("Itens associados ao pedido removidos (se existiam).\n");
}

/* -----------------------------------------------------------------
   MENU DO MÓDULO (para integração com seu main atual)
   ----------------------------------------------------------------- */
void menuPedidos() {
    int op;
    do {
        printf("\n===== MODULO DE PEDIDOS =====\n");
        printf("1 - Cadastrar Pedido\n");
        printf("2 - Listar Pedidos\n");
        printf("3 - Consultar Pedido\n");
        printf("4 - Adicionar Item ao Pedido\n");
        printf("5 - Remover Pedido\n");
        printf("6 - Voltar\n");
        printf("Opcao: ");
        if (scanf("%d", &op) != 1) { limpar_buffer_p(); op = 0; }

        switch (op) {
            case 1: cadastrarPedido_p(); break;
            case 2: listarPedidos_p(); break;
            case 3: consultarPedido_p(); break;
            case 4: adicionarItem_p(); break;
            case 5: removerPedido_p(); break;
            case 6: break;
            default: if (op != 0) printf("Opcao invalida.\n"); break;
        }
    } while (op != 6);
}

/* -----------------------------------------------------------------
   Exemplo de main apenas para testar o módulo (você pode integrar ao seu main)
   ----------------------------------------------------------------- */
int main() {
    printf("Executando modulo de pedidos (teste). Integre menuPedidos() ao seu main.\n");
    menuPedidos();
    printf("Saindo do módulo de pedidos.\n");
    return 0;
}
