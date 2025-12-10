
#include "feature_interface.h"
#include "feature_cliente.h"
#include "feature_produto.h"

int main() {
    iniciarInterface(); // deixa centralizado

    // Carrega os dados da memoria 
    carregarClientesCSV();
    carregarProdutosCSV();

    int opcao = -1;
    while (opcao != 0) {
        opcao = menuPrincipal(); // vai desenhar a interrface  do menu principal da main

        switch (opcao) {
            
            case 1:
                mostrarMenuClientes(); // Chama o modulo de cliente 
                break;
            case 2:
                mostrarMenuProdutos(); // Chama o modulo de produto
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
