
#include <stdio.h>
#include <string.h>
#include "../include/feature_produto.h"




int modulo_novo_produto () {
    int code,valor,quant,resposta;
    int salvamento;
    char descricao[100];
    int salvar;

    printf("insira o codigo do produto   :");
    scanf("%d", &code);

    if (codigoExiste(code)) {
        printf("O código %d JÁ existe.\n", code);
    } else {
        printf("O código %d não existe adicionando ao sistema.\n", code);
    }
    if (codigoExiste(code)) {
        printf("");
    } else {

        limpar_buffer();

        printf("Digita a descrição do produto");
        fgets(descricao, sizeof(descricao), stdin);

        printf("Digita o valor do produto");
        scanf("%d", &valor);

        printf("Quantos produtos tem no estoque");
        scanf("%d", &quant);}

//--------------------------- confirmar cadastro ---------------

        printf("salvar dados ? 0 sim e 1 nao");
        scanf("%d"), &resposta;

        if (resposta == 0){
            
     
        (modulo_novo_produto());

        printf("Os seguintes dados foram salvados \n");
        printf("codigo de produto : %d \n", code);
        printf("Descrição do produto : %s \n", descricao);
        printf("O valor do produto e: %d \n", valor);
        printf("A quantidade de produto e: %d \n", quant);


        
        } else {
            printf("Os dados não foram salvos \n");

            //////////////////////////////////////////////////////////////

            printf("Deseja iniciar o cadastro novamente ? 0 sim e 1 nao");
            scanf("%d", &resposta);


            if (resposta == 0){
                modulo_novo_produto();
            } else {
                printf("Encerrando o programa \n");
            }
        }

        salvamento = novo_produto(code, descricao, valor, quant);
        
///////////////////////////////////////////////

    return 0;
}

// Função para verificar se o código existe no arquivo
int codigoExiste(int codigoAVerificar) {
    FILE *arquivo;
    char linha[256];
    int codigoLido;
    int encontrado = 0; 

    //abre o arquivo para leitura
    arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {

        return 0; // Não existe
    }

    // loop para ler o arquivo linha por linha
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
    
    
        if (sscanf(linha, "%d,", &codigoLido) == 1) {
        // compara o código lido com o código procurado
            if (codigoLido == codigoAVerificar) {
                encontrado = 1;
                break; 
            }
        }
    }

    fclose(arquivo);

    return encontrado; 
}


////////////////////////////////////      ////////////////////////////////////////////



int novo_produto(int code,char descricao [], int valor, int quant){
    FILE *arquivo;

    arquivo = fopen("produtos.txt", "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir ou criar o arquivo!\n");
        return; // Sai da função se houver erro
    }

    fprintf(arquivo, "------------------------------------------------------------------------------------------\n"); 

    fprintf(arquivo, "%d\n", code);
    fprintf(arquivo, "%s\n", descricao);   
    fprintf(arquivo, "%d\n", valor);       
    fprintf(arquivo, "%d\n", quant); 
    
    fprintf(arquivo, "------------------------------------------------------------------------------------------ \n"); 

    fclose(arquivo);

    printf("produto salvo com sucesso: %d - %s\n", code, valor, quant);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////

void limpar_buffer() {
    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF);
}

/////////////////////////////////////////////////////////////////////////////

