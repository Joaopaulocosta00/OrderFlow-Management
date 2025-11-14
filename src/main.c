// main.c

#include <stdio.h>
// Inclui o cabeçalho da subpasta
#include "../include/feature_produto.h" 

int main(){

    int opcao;
    printf("Bem vindo ao sistema oque deseja fazer ? \n");
    printf("Para entrar na area de produto digite 1 \n");
    
    // Verifica se a leitura foi bem-sucedida
    if (scanf("%d", &opcao) != 1) {
        printf("Entrada invalida.\n");
        return 1; // Retorna erro
    }

    // A função modulo_produto() JÁ FOI DECLARADA pelo #include acima.
    if (opcao == 1){
        modulo_produto(); // Apenas chama
    } else {
        printf("Opcao invalida \n");
    }
    
    // Indica que o programa terminou com sucesso
    return 0; 
}