#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOME_ARQUIVO "produtos.txt"
#define NOME_TEMP "temp.txt"
#define TAMANHO_MAX_LINHA 256
// O produto tem 5 linhas de dados (----- CÓDIGO DESCRIÇÃO VALOR QUANTIDADE) + a linha de separação do final = 6 linhas no total
#define LINHAS_PRODUTO 6 

// --------------------------------------------------------------------------------------------------
// PROTÓTIPOS (Declaração de Funções para o compilador)
// --------------------------------------------------------------------------------------------------
void limpar_buffer();
int codigoExiste(int codigoAVerificar);
int novo_produto(int code, char descricao[], int valor, int quant);
int modulo_novo_produto();
int listar_produtos();
int apagar_produto();


// --------------------------------------------------------------------------------------------------
// IMPLEMENTAÇÃO DAS FUNÇÕES
// --------------------------------------------------------------------------------------------------

// ... código anterior

int apagar_produto() { 
    FILE *arquivo_original, *arquivo_temp;
    char linha[TAMANHO_MAX_LINHA];
    char codigo_a_apagar[20];
    int linhasParaPular = 0;
    int resposta;
    // Adiciona uma flag para saber se o código foi encontrado
    int codigo_encontrado = 0; 

    printf("\n--- APAGAR PRODUTO ---\n");
    printf("Digite o codigo do produto a ser apagado: ");
    if (scanf("%19s", codigo_a_apagar) != 1) {
        printf("Erro ao ler o codigo.\n");
        return 1;
    }
    limpar_buffer();

    arquivo_original = fopen(NOME_ARQUIVO, "r");
    if (arquivo_original == NULL) {
        printf("Arquivo de produtos nao encontrado. Nada para apagar.\n");
        return 0;
    }
    
    arquivo_temp = fopen(NOME_TEMP, "w");
    // ... (restante da abertura de arquivo)

    // Processa o arquivo linha por linha 
    while (fgets(linha, TAMANHO_MAX_LINHA, arquivo_original) != NULL) {
        
        // 1. Se estivermos pulando linhas (apagando o bloco), apenas decrementa e continua
        if (linhasParaPular > 0) {
            linhasParaPular--;
            continue; 
        }

        // ... (resto da lógica de busca)
        char linha_busca[TAMANHO_MAX_LINHA];
        sprintf(linha_busca, "%s\n", codigo_a_apagar);

        // Se encontrar o código na linha, começa a pular e ATIVA A FLAG
        if (strstr(linha, linha_busca) != NULL) { 
            codigo_encontrado = 1; // <--- ATIVA A FLAG AQUI!
            linhasParaPular = LINHAS_PRODUTO - 1; 
            continue; // Pula a linha atual (código)
        }

        // 3. Se não estiver no modo de pular, copia a linha para o arquivo temporário
        fprintf(arquivo_temp, "%s", linha);
    }
    
    fclose(arquivo_original);
    fclose(arquivo_temp);

    // ----------------------------------------------------------------------
    // NOVO BLOCO: VERIFICAÇÃO E MENSAGEM
    // ----------------------------------------------------------------------

    if (codigo_encontrado == 0) {
        // Se o código não foi encontrado, exclui o arquivo temporário e avisa.
        remove(NOME_TEMP);
        printf("\nO codigo %s nao foi encontrado no sistema. Nenhum produto foi apagado.\n", codigo_a_apagar);
    } else {
        // Se o código foi encontrado, faz a substituição dos arquivos
        if (remove(NOME_ARQUIVO) != 0) {
            perror("Erro ao deletar o arquivo original.");
            return 1;
        }

        if (rename(NOME_TEMP, NOME_ARQUIVO) != 0) {
            perror("Erro ao renomear o arquivo temporario.");
            return 1;
        }
        
        printf("\nProcesso concluido. O codigo %s e os dados associados foram apagados.\n", codigo_a_apagar);
    }
    
    // ... (restante do código de recursão)
    printf("Deseja apagar outro pedido? 0 sim e 1 nao: ");
    if (scanf("%d", &resposta) == 1 && resposta == 0) {
        limpar_buffer();
        apagar_produto(); 
    } else {
        printf("Retornando ao menu principal...\n");
    }
    
    return 0;
}


// MODULO DE CADASTRO DE PRODUTOS
int modulo_novo_produto () {

    int code, valor, quant, resposta;
    char descricao[100];
    int salvamento;

    printf("\n--- NOVO PRODUTO ---\n");
    printf("insira o codigo do produto: ");
    if (scanf("%d", &code) != 1) {
        printf("Erro ao ler o codigo.\n");
        limpar_buffer();
        return 1;
    }

    if (codigoExiste(code)) {
        printf("O codigo %d JA existe. Nao e possivel cadastrar.\n", code);
    } else {
        printf("O codigo %d nao existe, adicionando ao sistema.\n", code);

        limpar_buffer(); // Limpa apos o scanf do code

        printf("Digita a descricao do produto: ");
        fgets(descricao, sizeof(descricao), stdin);
        descricao[strcspn(descricao, "\n")] = 0; // Remove '\n'

        printf("Digita o valor do produto: ");
        scanf("%d", &valor);
        limpar_buffer();

        printf("Quantos produtos tem no estoque: ");
        scanf("%d", &quant);
        limpar_buffer();

        // --------------------------- confirmar cadastro ---------------

        printf("Salvar dados? 0 sim e 1 nao: ");
        scanf("%d", &resposta); // CORREÇÃO DE SINTAXE AQUI
        limpar_buffer();


        if (resposta == 0){
            
            salvamento = novo_produto(code, descricao, valor, quant);
            
            if (salvamento == 0) {
                printf("\nOs seguintes dados foram salvos:\n");
                printf("Codigo de produto : %d \n", code);
                printf("Descricao do produto : %s \n", descricao);
                printf("O valor do produto e: %d \n", valor);
                printf("A quantidade de produto e: %d \n", quant);
            }
        
        } else {
            printf("Os dados nao foram salvos.\n");
            
            printf("Deseja iniciar o cadastro novamente? 0 sim e 1 nao: ");
            scanf("%d", &resposta);
            limpar_buffer();

            if (resposta == 0){
                modulo_novo_produto();
            } else {
                printf("Encerrando o modulo de cadastro.\n");
            }
        }
    }

    return 0;
}


// Função para verificar se o código existe no arquivo
int codigoExiste(int codigoAVerificar) {
    FILE *arquivo;
    char linha[256];
    int codigoLido;
    int encontrado = 0; 

    arquivo = fopen(NOME_ARQUIVO, "r");
    if (arquivo == NULL) {
        return 0; 
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (sscanf(linha, "%d,", &codigoLido) == 1) {
            if (codigoLido == codigoAVerificar) {
                encontrado = 1;
                break; 
            }
        }
    }

    fclose(arquivo);
    return encontrado; 
}


// Função para salvar o novo produto
int novo_produto(int code, char descricao[], int valor, int quant){
    FILE *arquivo;

    arquivo = fopen(NOME_ARQUIVO, "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir ou criar o arquivo!\n");
        return 1; // Falha
    }

    fprintf(arquivo, "------------------------------------------------------------------------------------------\n"); 
    fprintf(arquivo, "%d\n", code);
    fprintf(arquivo, "%s\n", descricao);   
    fprintf(arquivo, "%d\n", valor);       
    fprintf(arquivo, "%d\n", quant); 
    fprintf(arquivo, "------------------------------------------------------------------------------------------\n"); 

    fclose(arquivo);

    printf("Produto salvo com sucesso.\n");
    return 0; // Sucesso
}


// Função para limpar o buffer
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// Função para listar/buscar produtos
int listar_produtos() {
    FILE *arquivo;
    char linha[256];
    int codigoLido;
    int codigoABuscar; 

    printf("\n--- BUSCAR PRODUTO ---\n");
    printf("Por favor digite o codigo do produto que deseja verificar se esta cadastrado: ");
    
    if (scanf("%d", &codigoABuscar) != 1) {
        printf("Erro na leitura do codigo.\n");
        return 0;
    }
    limpar_buffer(); 

    arquivo = fopen(NOME_ARQUIVO, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo 'produtos.txt' ou o arquivo nao existe.\n");
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (sscanf(linha, "%d", &codigoLido) == 1) {
            
            if (codigoLido == codigoABuscar) {
                
                char descricao[100];
                int valor, quant;

                printf("\n--- Produto Encontrado ---\n");
                printf("Codigo: %d\n", codigoLido);

                // Lê as próximas 4 linhas (Descricao, Valor, Quantidade, Separador)
                
                if (fgets(descricao, sizeof(descricao), arquivo) == NULL) goto erro_leitura;
                descricao[strcspn(descricao, "\n")] = 0;
                printf("Descricao: %s\n", descricao);

                if (fgets(linha, sizeof(linha), arquivo) == NULL || sscanf(linha, "%d", &valor) != 1) goto erro_leitura;
                printf("Valor: %d\n", valor);

                if (fgets(linha, sizeof(linha), arquivo) == NULL || sscanf(linha, "%d", &quant) != 1) goto erro_leitura;
                printf("Quantidade: %d\n", quant);
                
                // Lê o separador final do bloco
                fgets(linha, sizeof(linha), arquivo); 

                printf("--------------------------\n");
                fclose(arquivo);
                return 1; 
            }
        }
    }

    printf("O codigo %d nao foi encontrado no sistema.\n", codigoABuscar);
    fclose(arquivo);
    return 0; 

    erro_leitura:
    printf("Erro na leitura dos dados apos encontrar o codigo %d.\n", codigoABuscar);
    fclose(arquivo);
    return 0; 
}


// --------------------------------------------------------------------------------------------------
// FUNÇÃO PRINCIPAL
// --------------------------------------------------------------------------------------------------
int main() {
    int opcao;
    
    printf("\n=======================================\n");
    printf("Bem vindo ao sistema! O que deseja fazer?\n");
    printf("1 - Cadastrar novo produto\n");
    printf("2 - Listar/Buscar produto por codigo\n");
    printf("3 - Apagar um produto\n");
    printf("4 - Sair\n");
    printf("Digite a opcao: ");
    
    if (scanf("%d", &opcao) != 1) {
        printf("Opcao invalida. Encerrando.\n");
        limpar_buffer();
        return 1;
    }
    limpar_buffer();

    if (opcao == 1){
        // Usando a funcao correta: modulo_novo_produto()
        modulo_novo_produto(); 
    } else if (opcao == 2){
        listar_produtos();
    } else if (opcao == 3){
        apagar_produto();
    } else if (opcao == 4) {
        printf("Obrigado por usar o sistema. Adeus!\n");
        return 0;
    } else {
        printf("Opcao invalida. Tente novamente.\n");
    }
    
    return main(); 
}