#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#define ARQ_CLIENTES "clientes.dat"
#define ARQ_PRODUTOS "produtos.dat"
#define ARQ_PEDIDOS "pedidos.dat"
#define ARQ_ITENS "itenspedido.dat"

// ========================= ESTRUTURAS =========================
typedef struct {
    int id;
    char nome[100];
    char endereco[200];
    char telefone[20];
} Cliente;

typedef struct {
    int id;
    char descricao[100];
    double preco;
    int estoque;
} Produto;

typedef struct {
    int id;
    int clienteId;
    char data[11];
    double total;
} Pedido;

typedef struct {
    int pedidoId;
    int produtoId;
    int quantidade;
    double subtotal;
} ItemPedido;

// ========================= FUNÇÕES AUXILIARES =========================
int cliente_existe(int id) {
    FILE *f = fopen(ARQ_CLIENTES, "rb");
    if (!f) return 0;

    Cliente c;
    while (fread(&c, sizeof(Cliente), 1, f))
        if (c.id == id) { fclose(f); return 1; }

    fclose(f);
    return 0;
}

int produto_existe(int id, Produto *out) {
    FILE *f = fopen(ARQ_PRODUTOS, "rb");
    if (!f) return 0;

    Produto p;
    while (fread(&p, sizeof(Produto), 1, f)) {
        if (p.id == id) {
            if (out) *out = p;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int pedido_existe(int id) {
    FILE *f = fopen(ARQ_PEDIDOS, "rb");
    if (!f) return 0;

    Pedido p;
    while (fread(&p, sizeof(Pedido), 1, f))
        if (p.id == id) { fclose(f); return 1; }

    fclose(f);
    return 0;
}

// ========================= CADASTRAR PEDIDO =========================
void cadastrar_pedido(WINDOW *win) {
    char entrada[20];
    Pedido ped;
    int codigo_produto;
    Produto prod;
    int quantidade;
    double total = 0;

    // -------- 1. Ler código do pedido --------
    while (1) {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 1, 2, "=== Cadastro de Pedido ===");
        mvwprintw(win, 3, 2, "Código do Pedido: ");
        wrefresh(win);

        echo();
        mvwgetnstr(win, 3, 22, entrada, 10);
        noecho();
        ped.id = atoi(entrada);

        if (pedido_existe(ped.id)) {
            mvwprintw(win, 10, 2, "ERRO: Pedido já existe!");
            wgetch(win);
            continue;
        }
        break;
    }

    // -------- 2. Ler cliente --------
    while (1) {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 1, 2, "=== Cadastro de Pedido ===");
        mvwprintw(win, 3, 2, "Código do Pedido: %d", ped.id);
        mvwprintw(win, 5, 2, "Código do Cliente: ");
        wrefresh(win);

        echo();
        mvwgetnstr(win, 5, 22, entrada, 10);
        noecho();
        ped.clienteId = atoi(entrada);

        if (!cliente_existe(ped.clienteId)) {
            mvwprintw(win, 10, 2, "ERRO: Cliente não existe!");
            wgetch(win);
            continue;
        }
        break;
    }

    // -------- 3. Cadastrar itens --------
    FILE *fItens = fopen(ARQ_ITENS, "ab");

    while (1) {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 1, 2, "=== Cadastro de Itens ===");
        mvwprintw(win, 3, 2, "Pedido: %d  Cliente: %d", ped.id, ped.clienteId);
        mvwprintw(win, 5, 2, "Código do produto (0 para finalizar): ");
        wrefresh(win);

        echo();
        mvwgetnstr(win, 5, 42, entrada, 10);
        noecho();
        codigo_produto = atoi(entrada);

        if (codigo_produto == 0) break;

        if (!produto_existe(codigo_produto, &prod)) {
            mvwprintw(win, 10, 2, "ERRO: Produto não existe!");
            wgetch(win);
            continue;
        }

        mvwprintw(win, 7, 2, "Quantidade: ");
        echo();
        mvwgetnstr(win, 7, 14, entrada, 10);
        noecho();
        quantidade = atoi(entrada);

        ItemPedido item;
        item.pedidoId = ped.id;
        item.produtoId = codigo_produto;
        item.quantidade = quantidade;
        item.subtotal = quantidade * prod.preco;

        fwrite(&item, sizeof(ItemPedido), 1, fItens);
        total += item.subtotal;

        mvwprintw(win, 10, 2, "Item adicionado!");
        wgetch(win);
    }

    fclose(fItens);

    // -------- 4. Salvar pedido --------
    strcpy(ped.data, "01/01/2025"); // padrão
    ped.total = total;

    FILE *fPed = fopen(ARQ_PEDIDOS, "ab");
    fwrite(&ped, sizeof(Pedido), 1, fPed);
    fclose(fPed);

    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 5, 2, "Pedido cadastrado com sucesso!");
    mvwprintw(win, 7, 2, "Total: %.2f", total);
    wrefresh(win);
    wgetch(win);
}

// ========================= MENU PRINCIPAL =========================
int main() {
    initscr();
    cbreak();
    noecho();

    WINDOW *win = newwin(20, 70, 2, 5);
    box(win, 0, 0);

    int op;
    do {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 1, 2, "=== MÓDULO PEDIDOS ===");
        mvwprintw(win, 3, 2, "1 - Cadastrar Pedido");
        mvwprintw(win, 4, 2, "2 - Sair");
        mvwprintw(win, 6, 2, "Opção: ");
        wrefresh(win);

        echo();
        wscanw(win, "%d", &op);
        noecho();

        if (op == 1) cadastrar_pedido(win);

    } while (op != 2);

    endwin();
    return 0;
}
