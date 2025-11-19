// main.c
#include <stdio.h>
// Inclui o cabeçalho da subpasta
#include "../include/feature_produto.h" 

#include "../include/feature_cliente.h"

#include "../include/feature_pedido.h"

int main(){

    int opcao;
    printf("Bem vindo ao sistema oque deseja fazer ? \n");
    printf("Para entrar na area de cliente digite 1.\n");
    printf("Para entrar na area de produto digite 2. \n");
    printf("para entrar na area de pedido digite 3.\n");
   
    
    // Verifica se a leitura foi bem-sucedida
    if (scanf("%d", &opcao) != 1) {
        printf("Entrada invalida.\n");
        return 1; // Retorna erro
    }
    if (opcao== 1){
        cliente();
    }
    // A função modulo_produto() JÁ FOI DECLARADA pelo #include acima.
    else if (opcao == 2){
        modulo_produto(); // Apenas chama
    }

    else if (opcao== 3){
        pedido();
    }
    else {
        printf("opção invalida.\n");
    }
    void cliente(void) {
    printf("Você entrou na área de cliente.\n");
}

void modulo_produto(void) {
    printf("Você entrou na área de produto.\n");
}

void pedido(void) {
    printf("Você entrou na área de pedido.\n");
}
    // Indica que o programa terminou com sucesso
    return 0; 
}
