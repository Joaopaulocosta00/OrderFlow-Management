#include <stdio.h>
#include <ncurses.h>
#include <string.h>

#define MAX_PEDIDOS 100
#define MAX_NOME 50

typedef struct {
    int id;
    char cliente[MAX_NOME];
    char produto[MAX_NOME];
    int quantidade;
} Pedido;

Pedido pedidos[MAX_PEDIDOS];
int total_pedidos = 0;

void adicionarPedido() {
    clear();
    echo();

    Pedido p;
    p.id = total_pedidos + 1;

    mvprintw(2, 2, "Nome do cliente: ");
    getstr(p.cliente);

    mvprintw(3, 2, "Produto: ");
    getstr(p.produto);

    mvprintw(4, 2, "Quantidade: ");
    scanw("%d", &p.quantidade);

    pedidos[total_pedidos] = p;
    total_pedidos++;

    noecho();
    mvprintw(6, 2, "Pedido adicionado com sucesso!");
    getch();
}

void listarPedidos() {
    clear();
    mvprintw(1, 2, "Lista de Pedidos");
    for(int i = 0; i < total_pedidos; i++) {
        mvprintw(i + 3, 2, "ID: %d | Cliente: %s | Produto: %s | Quantidade: %d",
                 pedidos[i].id, pedidos[i].cliente, pedidos[i].produto, pedidos[i].quantidade);
    }
    mvprintw(total_pedidos + 5, 2, "Pressione qualquer tecla para voltar...");
    getch();
}

int main() {
    int opcao;

    initscr();
    noecho();
    cbreak();

    while(1) {
        clear();
        mvprintw(1, 2, "Sistema de Controle de Pedidos");
        mvprintw(3, 2, "1 - Adicionar Pedido");
        mvprintw(4, 2, "2 - Listar Pedidos");
        mvprintw(5, 2, "3 - Sair");
        mvprintw(7, 2, "Escolha uma opcao: ");
        scanw("%d", &opcao);

        switch(opcao) {
            case 1: adicionarPedido(); break;
            case 2: listarPedidos(); break;
            case 3: endwin(); return 0;
            default:
                mvprintw(9, 2, "Opcao invalida!");
                getch();
        }
    }

    endwin();
    return 0;
}
