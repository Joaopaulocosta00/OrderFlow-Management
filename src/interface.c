#include "feature_interface.h"
#include "feature_cliente.h" // chamar o cadastro/listagem

void iniciarInterface() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void fecharInterface() {
    endwin();
}

void limparTela() {
    clear();
}

void pausa() {
    printw("\nPressione qualquer tecla para continuar");
    getch();
}

void mostrarCabecalho(const char* titulo) {
    clear();
    printw("========================================\n");
    printw("      %s\n", titulo);
    printw("========================================\n");
}

// Menu Principal (Retorna a opção para o main)
int menuPrincipal() {
    mostrarCabecalho("Bem vindo ao sistema");
    printw("1 - Gerenciar Clientes\n");
    printw("2 - Gerenciar Produtos\n");
    printw("0 - Sair\n");
    printw("\nEscolha: ");
    
    int opcao;
    echo();
    scanw("%d", &opcao);
    noecho();
    
    return opcao;
}

// Menu de Clientes (Fica aqui conforme Diagrama pág. 12 do PDF)
void mostrarMenuClientes() {
    int opcao = -1;
    while(opcao != 0) {
        mostrarCabecalho("GESTAO DE CLIENTES");
        printw("1 - Cadastrar Cliente\n");
        printw("2 - Listar Clientes\n");
        printw("3 - Consultar Cliente\n"); // <--- NOVA OPÇÃO
        printw("4 - Remover Cliente\n");  // <--- MUDOU O NÚMERO
        printw("0 - Voltar ao Menu Principal\n");
        printw("\nEscolha: ");
        
        echo();
        scanw("%d", &opcao);
        noecho();

        switch(opcao) {
            case 1: cadastrarCliente(); break;
            case 2: listarClientes(); break;
            case 3: consultarCliente(); break; // <--- CHAMA A FUNÇÃO NOVA
            case 4: removerClienteInterface(); break;
            case 0: break;
            default: printw("Opcao invalida!\n"); pausa(); break;
        }
    }
}
void mostrarMenuProdutos() {
    int opcao = -1;
    while(opcao != 0) {
        mostrarCabecalho("GESTAO DE PRODUTOS");
        printw("1 - Cadastrar Produto\n");
        printw("2 - Consultar Produto\n");
        printw("3 - Remover Produto\n");
        printw("4 - Listar Produtos\n");
        printw("0 - Voltar ao Menu Principal\n");
        printw("\nEscolha: ");
        
        echo();
        scanw("%d", &opcao);
        noecho();

        switch(opcao) {
            case 1: cadastrarProduto(); break;
            case 2: consultarProduto(); break;
            case 3: removerProdutoInterface(); break;
            case 4: listarProdutos(); break;
            case 0: break;
            default: printw("Opcao invalida!\n"); pausa(); break;
        }
    }
}