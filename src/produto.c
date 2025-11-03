
#include <stdio.h>
#include <string.h>

// Função para verificar se o código existe no arquivo
int codigoExiste(int codigoAVerificar) {
    FILE *arquivo;
    char linha[256]; // Buffer para ler a linha
    int codigoLido;
    int encontrado = 0; // Flag para indicar se achou

    // 1. Abre o arquivo para leitura
    arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        // Se o arquivo não existir, significa que o código não está lá
        // E não é um erro se for a primeira vez
        return 0; // Não existe
    }

    // 2. Loop para ler o arquivo linha por linha
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // 3. Tenta ler o primeiro inteiro (código) da linha
        // O formato esperado é: inteiro, texto...
        if (sscanf(linha, "%d,", &codigoLido) == 1) {
            // 4. Compara o código lido com o código procurado
            if (codigoLido == codigoAVerificar) {
                encontrado = 1;
                break; // Encontrou, pode sair do loop
            }
        }
    }

    // 5. Fecha o arquivo
    fclose(arquivo);

    return encontrado; // Retorna 1 se achou, 0 se não
}


////////////////////////////////////      ////////////////////////////////////////////



int novo_produto(int code, const char *nome, int qant){
    FILE *arquivo;

    // 1. Abrir o arquivo no modo "a" (append)
    // Se não existir, ele cria. Se existir, ele adiciona no final.
    arquivo = fopen("produtos.txt", "a");

    if (arquivo == NULL) {
        printf("🚨 Erro ao abrir ou criar o arquivo!\n");
        return; // Sai da função se houver erro
    }

    // 2. Escrever a nova linha no arquivo
    // Usamos fprintf para escrever no arquivo de forma formatada (como o printf)
    // Garantimos quebra de linha (\n) para o próximo registro
    fprintf(arquivo, "%d,%s\n", code, nome);

    // 3. Fechar o arquivo (MUITO IMPORTANTE!)
    fclose(arquivo);

    printf("✅ Produto salvo com sucesso: %d - %s\n", code, valor, quant);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////




int main() {
    int code,valor,quant;
    char descricao[100];


    printf("insira o codigo que deseja verificar se existe");
    scanf("%d", &code);

    // A. Verificando um código que provavelmente existe
    if (codigoExiste(code)) {
        printf("O código %d JÁ existe.\n", code);
    } else {
        printf("O código %d não existe adicionando ao sistema.\n", code);
        // *Se for inserir, você deve abrir o arquivo no modo "a" (append) aqui*
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
        


    // B. Verificando um código que provavelmente não existe



    return 0;
}