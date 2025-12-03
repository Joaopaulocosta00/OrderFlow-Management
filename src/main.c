
#include "feature_interface.h"
#include "feature_cliente.h"
#include "feature_produto.h"

int main() {
    iniciarInterface(); // deixa centralizado

    // Carrega os dados da me moria 
    carregarClientesCSV();
    carregarProdutosCSV();

    int opcao = -1;
    while (opcao != 0) {
        opcao = menuPrincipal(); // Interface desenha

        switch (opcao) {
            case 1:
                mostrarMenuClientes(); // Chama módulo cliente
                break;
            case 2:
                mostrarMenuProdutos(); // Chama módulo produto
                break;
            case 0:
                printw("\nSalvando dados e saindo...\n");
                break;
            default:
                printw("\nOpcao invalida!\n");
                pausa();
                break;
        }
    }

    // Persiste dados antes de fechar
    salvarClientesCSV();
    salvarProdutosCSV();

    fecharInterface(); // Encerra ncurses
    return 0;
}
