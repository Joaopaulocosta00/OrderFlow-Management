
#include <stdio.h>
#include <string.h>
#include "../include/feature_produto.h"

  // codigo principal da main de produtos 

  int modulo_produto(){

    int opcao;
    printf("Bem vindo ao sistema oque deseja fazer ? \n");
    printf("para cadastrar um produto digite 1 \n");
    printf("para listar produto digite 2 \n ");
    printf("para apagar um produto digite 3 \n");
    scanf("%d", &opcao);

    if (opcao == 1){
        extern int cadastrar_produto();
        cadastrar_produto();
    } else if (opcao == 2){
        extern int listar_produtos();
        listar_produtos();
    } else if (opcao == 3){
        extern int apagar_produto();
        apagar_produto();
    } else {
        printf("opcao invalida \n");
    }

}
