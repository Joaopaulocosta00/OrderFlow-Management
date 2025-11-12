#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definições
#define NOME_ARQUIVO "produtos.txt"
#define NOME_TEMP "temp.txt"
#define TAMANHO_MAX_LINHA 256
#define LINHAS_A_APAGAR 5 


int main() { //codigo principal 
    FILE *arquivo_original, *arquivo_temp;
    char linha[TAMANHO_MAX_LINHA];
    char numero_pedido[20];
    int linhasParaPular = 0;


    printf("Digite o numero do pedido a ser apagado: ");
    if (scanf("%19s", numero_pedido) != 1) {
        printf("Erro ao ler o pedido.\n");
        return 1;
    }

    // 2. Abrir os arquivos
    arquivo_original = fopen(NOME_ARQUIVO, "r");
    if (arquivo_original == NULL) {
        perror("Erro ao abrir o arquivo original (produtos.txt)");
        return 1;
    }

    arquivo_temp = fopen(NOME_TEMP, "w");
    if (arquivo_temp == NULL) {
        perror("Erro ao criar arquivo temporario (temp.txt)");
        fclose(arquivo_original);
        return 1;
    }

    // 3. Processar o arquivo linha por linha
    while (fgets(linha, TAMANHO_MAX_LINHA, arquivo_original) != NULL) {
        
        // A. Verifica se precisa pular (apagar) a linha atual
        if (linhasParaPular > 0) {
            linhasParaPular--;
            // Nao escreve a linha no arquivo temporario (ela e ignorada/apagada)
            continue; 
        }

        // B. Verifica se encontrou o numero do pedido na linha atual
        // strstr() busca uma string dentro de outra e retorna NULL se nao encontrar.
        if (strstr(linha, numero_pedido) != NULL) {
            // Se encontrar, define o contador para pular a linha atual + as 4 seguintes
            linhasParaPular = LINHAS_A_APAGAR - 1; 
            // Nao escreve a linha no arquivo temporario (ela e ignorada/apagada)
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
    
    return 0;
}