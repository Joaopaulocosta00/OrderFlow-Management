
#include <stdio.h>
#include <string.h>

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



int novo_produto(int code, const char *nome, int valor, int quant){
    FILE *arquivo;

    arquivo = fopen("produtos.txt", "a");

    if (arquivo == NULL) {
        printf("🚨 Erro ao abrir ou criar o arquivo!\n");
        return; // Sai da função se houver erro
    }

    fprintf(arquivo, "%d,%s\n", code,nome,valor,quant);

    fclose(arquivo);

    printf("✅ Produto salvo com sucesso: %d - %s\n", code, valor, quant);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////




int main() {
    int code,valor,quant;
    int salvamento;
    char descricao[100];


    printf("insira o codigo que deseja verificar se existe");
    scanf("%d", &code);

    if (codigoExiste(code)) {
        printf("O código %d JÁ existe.\n", code);
    } else {
        printf("O código %d não existe adicionando ao sistema.\n", code);
    }
    if (codigoExiste(code)) {
        printf("");
    } else {

        printf("Digita a descrição do produto");
        fgets(descricao, sizeof(descricao), stdin);

        printf("Digita o valor do produto");
        scanf("%d", &valor);

        printf("Quantos produtos tem no estoque");
        scanf("%d", &quant);}

        int salvar (novo_produto);

        printf("Os seguintes dados foram salvados /n");
        printf("codigo de produto : %d \n", code);
        printf("Descrição do produto : %s \n", descricao);
        printf("O valor do produto e: %d \n", valor);
        printf("A quantidade de produto e: %d \n", quant);

        salvamento = novo_produto(code, descricao, valor, quant);
        



        
///////////////////////////////////////////////

    return 0;
}
