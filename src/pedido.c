#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

// Simulações de "bancos de dados"
#define MAX_PEDIDOS 10
#define MAX_CLIENTES 5
#define MAX_PRODUTOS 5

typedef struct {
    int codigo;
    int cliente;
    int produtos[10];
    int num_produtos;
} Pedido;

Pedido pedidos[MAX_PEDIDOS];
int num_pedidos = 0;

int clientes[MAX_CLIENTES] = {100, 101, 102, 200, 201};
int produtos[MAX_PRODUTOS] = {10, 11, 12, 20, 21};

// Funções auxiliares de verificação
int pedido_existe(int codigo) {
    for (int i = 0; i < num_pedidos; i++)
        if (pedidos[i].codigo == codigo)
            return 1;
    return 0;
}

int cliente_existe(int codigo) {
    for (int i = 0; i < MAX_CLIENTES; i++)
        if (clientes[i] == codigo)
            return 1;
    return 0;
}

int produto_existe(int codigo) {
    for (int i = 0; i < MAX_PRODUTOS; i++)
        if (produtos[i] == codigo)
            return 1;
    return 0;
}

void mostrar_msg(WINDOW *win, const char *msg) {
    int y, x;
    getmaxyx(win, y, x);
    mvwprintw(win, y - 2, 2, "Mensagem: %-60s", msg);
    wrefresh(win);
}

// Função principal de cadastro de pedido
void cadastrar_pedido(WINDOW *win) {
    int codigo_pedido, codigo_cliente, codigo_produto;
    char entrada[20];
    Pedido novo;

    // Entrada do código do pedido
    while (1) {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 1, 2, "=== Cadastro de Pedido ===");
        mvwprintw(win, 3, 2, "Codigo do Pedido: ");
        wrefresh(win);

        echo();
        mvwgetnstr(win, 3, 22, entrada, 10);
        noecho();
        codigo_pedido = atoi(entrada);

        if (pedido_existe(codigo_pedido)) {
            mostrar_msg(win, "Erro: Codigo de pedido ja existe!");
            wgetch(win);
            continue; // volta para o campo código
        }
        novo.codigo = codigo_pedido;
        break;
    }

    // Entrada do código do cliente
    while (1) {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 1, 2, "=== Cadastro de Pedido ===");
        mvwprintw(win, 3, 2, "Codigo do Pedido: %d", novo.codigo);
        mvwprintw(win, 5, 2, "Codigo do Cliente: ");
        wrefresh(win);

        echo();
        mvwgetnstr(win, 5, 23, entrada, 10);
        noecho();
        codigo_cliente = atoi(entrada);

        if (!cliente_existe(codigo_cliente)) {
            mostrar_msg(win, "Erro: Cliente nao existe!");
            wgetch(win);
            continue;
        }
        novo.cliente = codigo_cliente;
        break;
    }

    // Cadastrar produtos
    novo.num_produtos = 0;
    while (1) {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 1, 2, "=== Cadastro de Pedido ===");
        mvwprintw(win, 3, 2, "Pedido: %d | Cliente: %d", novo.codigo, novo.cliente);
        mvwprintw(win, 5, 2, "Informe codigo do produto (0 para finalizar): ");
        wrefresh(win);

        echo();
        mvwgetnstr(win, 5, 48, entrada, 10);
        noecho();
        codigo_produto = atoi(entrada);

        if (codigo_produto == 0) break;

        if (!produto_existe(codigo_produto)) {
            mostrar_msg(win, "Erro: Produto nao encontrado!");
            wgetch(win);
            continue;
        }

        novo.produtos[novo.num_produtos++] = codigo_produto;
        mostrar_msg(win, "Produto adicionado com sucesso!");
        wgetch(win);
    }

    // Salva pedido
    pedidos[num_pedidos++] = novo;
    mostrar_msg(win, "Pedido cadastrado com sucesso!");
    wgetch(win);
}

int teste_make() {
    initscr();
    noecho();
    cbreak();

    WINDOW *janela = newwin(15, 70, 3, 5);
    box(janela, 0, 0);

    int opcao;
    while (1) {
        werase(janela);
        box(janela, 0, 0);
        mvwprintw(janela, 1, 2, "=== Sistema de Pedidos ===");
        mvwprintw(janela, 3, 4, "1 - Cadastrar Pedido");
        mvwprintw(janela, 4, 4, "2 - Sair");
        mvwprintw(janela, 6, 4, "Opcao: ");
        wrefresh(janela);

        echo();
        wscanw(janela, "%d", &opcao);
        noecho();

        if (opcao == 1)
            cadastrar_pedido(janela);
        else if (opcao == 2)
            break;
    }

    endwin();
    return 0;
}
