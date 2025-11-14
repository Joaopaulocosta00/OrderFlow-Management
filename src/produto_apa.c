#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/feature_produto.h"

// Definições
#define NOME_ARQUIVO "produtos.txt"
#define NOME_TEMP "temp.txt"
#define TAMANHO_MAX_LINHA 256
#define LINHAS_A_APAGAR 5 


int apagar_produto() { //codigo principal 
    FILE *arquivo_original, *arquivo_temp;
    char linha[TAMANHO_MAX_LINHA];
    char numero_pedido[20];
    int linhasParaPular = 0;
    int resposta = 0;


    printf("Digite o numero do pedido a ser apagado: ");
    if (scanf("%19s", numero_pedido) != 1) {
        printf("Erro ao ler o pedido.\n");
        return 1;
    }

    // 2. Abrir os arquivos onde esta sendo salvo os dados do produto
    arquivo_original = fopen(NOME_ARQUIVO, "r");
    if (arquivo_original == NULL) {
        perror("Erro ao abrir o arquivo original (produtos.txt)");
        return 1;
    }
    // cria um arquiuvo temporario para clonar os dados 
    arquivo_temp = fopen(NOME_TEMP, "w");
    if (arquivo_temp == NULL) {
        perror("Erro ao criar arquivo temporario (temp.txt)");
        fclose(arquivo_original);
        return 1;
    }

    // processo o arquivo linha por linha 
    while (fgets(linha, TAMANHO_MAX_LINHA, arquivo_original) != NULL) {
        
        
        if (linhasParaPular > 0) {
            linhasParaPular--;
            continue; 
        }


        if (strstr(linha, numero_pedido) != NULL) {
            linhasParaPular = LINHAS_A_APAGAR - 1; 
            continue;
        }

        
        fprintf(arquivo_temp, "%s", linha);
    }

    
    fclose(arquivo_original);
    fclose(arquivo_temp);

    
    if (remove(NOME_ARQUIVO) != 0) {
        perror("Erro ao deletar o arquivo original.");
        return 1;
    }

    if (rename(NOME_TEMP, NOME_ARQUIVO) != 0) {
        perror("Erro ao renomear o arquivo temporario.");
        return 1;
    }

    printf("\nProcesso concluido. O pedido %s e as 4 linhas seguintes foram apagados.\n", numero_pedido);

    printf("Deseja apagar outro pedido ? 0 sim e 1 nao \n");
    scanf("%d", &resposta);

    if (resposta == 0){
        apagar_produto();
    } else {
        printf("Retornando ao menu principal...\n");
    } modulo_produto ();
    
    return 0;
}