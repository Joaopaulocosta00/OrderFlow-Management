#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

#define ARQUIVO "clientes.txt"

struct Cliente {
    char codigo[20];
    char nome[50];
    char endereco[100];
    char telefone[15];
    char documento[20];
    char tipo; // 'F' = CPF, 'J' = CNPJ
    char email[50];
};

struct Cliente clientes[100];
int numClientes = 0;

//Funcap para cadastrar cliente
int cadastrarCliente(struct Cliente clientes[], int *numClientes, struct Cliente novo) {
    if (*numClientes >= 100) return -11;
    for (int i = 0; i < *numClientes; i++) {
        if (strcmp(clientes[i].codigo, novo.codigo) == 0) return 1;
    }
    clientes[*numClientes] = novo;
    (*numClientes)++;
    return 0;
}
//Funcao para validar codigo com numeros
int validarCodigo(const char *codigo) {
    if (strlen(codigo) == 0) return 0;
    for (int i = 0; i< strlen(codigo); i++) {
        if (!isdigit((unsigned char)codigo[i])) return 0;
    }
    return 1;
}
//Funcao para validar CPF
int validarCPF(const char *cpf) {
    int i, j, digito1 = 0, digito2 = 0;
    // Verifica tamanho
    if (strlen(cpf) != 11) return 0;

    // Verifica se todos os caracteres são dígitos
    for (i = 0; i < 11; i++) {
        if (!isdigit((unsigned char)cpf[i])) return 0;
    }
    // Verifica se todos os dígitos são iguais (CPF inválido)
    int todosIguais = 1;
    for (i = 1; i < 11; i++) {
        if (cpf[i] != cpf[0]) {
            todosIguais = 0;
            break;
        }
    }
    if (todosIguais) return 0;

    // Calcula primeiro dígito verificador
    for (i = 0, j = 10; i < 9; i++, j--) {
        digito1 += (cpf[i] - '0') * j;
    }
    digito1 = (digito1 * 10) % 11;
    if (digito1 == 10) digito1 = 0;

    // Calcula segundo dígito verificador
    for (i = 0, j = 11; i < 10; i++, j--) {
        digito2 += (cpf[i] - '0') * j;
    }
    digito2 = (digito2 * 10) % 11;
    if (digito2 == 10) digito2 = 0;

    // Retorna 1 se válido, 0 se inválido
    return (digito1 == (cpf[9] - '0') && digito2 == (cpf[10] - '0'));
}
// Funcao para validar CNPJ
int validarCNPJ(const char *cnpj) { 
    int i, digito1 = 0, digito2 = 0;
    int pesos1[12] = {5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2};
    int pesos2[13] = {6, 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2};
    if (strlen(cnpj) != 14) return 0;
    for (i = 0; i < 14; i++) {
        if (cnpj[i] < '0' || cnpj[i] > '9') return 0;
    }
    for (i = 0; i < 12; i++) {
        digito1 += (cnpj[i] - '0') * pesos1[i];
    }
    digito1 = digito1 % 11;
    digito1 = (digito1 < 2) ? 0 : 11 - digito1;
    for (i = 0; i < 13; i++) {
        digito2 += (cnpj[i] - '0') * pesos2[i];
    }
    digito2 = digito2 % 11;
    digito2 = (digito2 < 2) ? 0 : 11 - digito2;
    return (digito1 == (cnpj[12] - '0') && digito2 == (cnpj[13] - '0'));
}
// Funcao para validar telefone
int verificarTelefone(const char *telefone) {
    int len = strlen(telefone);
    if (len < 10 || len > 15) return 0;
    for (int i = 0; i < len; i++)
        if (!(isdigit((unsigned char)telefone[i]) || telefone[i]=='+' || telefone[i]=='-' || telefone[i]==' ')) return 0;
    return 1;
}
// Funcao para validar email
int validarEmail(const char *email) {
    int atCount = 0; // Contador de ocorrências do caractere '@'
    int dotCount = 0; // Contador de ocorrências do caractere '.'
    int i;  // Variável de iteração
    int len = strlen(email); // Calcula o tamanho da string 'email'
    for (i = 0; i < len; i++) { // perorre por cada caractere da string
        if (email[i] == '@') atCount++;
        if (email[i] == '.') dotCount++;
    }
    return (atCount == 1 && dotCount >= 1); // deve ter exatamente um '@' e pelo menos um '.'
}
//Funcao para salvar cliente em arquivo
int salvarCliente(const char *codigo, const char *nome, const char *endereco, const char *telefone, const char *cpf, const char *cnpj, const char *tipo, const char *email) {
    FILE *arquivo = fopen(ARQUIVO, "a");
    if(arquivo == NULL) {
        printf("Erro ao abrir o arquivo '%s': %s\n", ARQUIVO, strerror(errno));
        return -6;
    }
    if (nome == NULL || nome[0] == '\0' || endereco == NULL || endereco[0] == '\0' || 
        telefone == NULL || telefone[0] == '\0' || email == NULL || email[0] == '\0' || 
        codigo == NULL || codigo[0] == '\0') {
        printf("Erro: dados incompletos.\n");
        fclose(arquivo);
        return -1;
    }
    fprintf(arquivo, "---------Dados do Cliente---------\n");
    fprintf(arquivo, "Codigo: %s\n", codigo);
    fprintf(arquivo, "Nome: %s\n", nome);
    fprintf(arquivo, "Tipo: %s\n", tipo);
    if (strcmp(tipo, "CPF") == 0) {
        fprintf(arquivo, "CPF: %s\n", cpf);
    } else {
        fprintf(arquivo, "CNPJ: %s\n", cnpj);
    }
    fprintf(arquivo, "Telefone: %s\n", telefone);
    fprintf(arquivo, "Endereco: %s\n", endereco);
    fprintf(arquivo, "Email: %s\n", email);
    fprintf(arquivo, "--------------------------\n\n");
    fclose(arquivo);
    printf("Dados salvos com sucesso.\n");
    return 0;
}
// Funcao para consultar cliente por codigo
int consultarCliente(struct Cliente clientes[], int numClientes, const char codigo[]) {
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].codigo, codigo) == 0) {
            printf("\n=== Dados do Cliente ===\n");
            printf("Codigo: %s\n", clientes[i].codigo);
            printf("Nome: %s\n", clientes[i].nome);
            printf("Tipo: %s\n", (clientes[i].tipo == 'F') ? "CPF" : "CNPJ");
            printf("Documento: %s\n", clientes[i].documento);
            printf("Telefone: %s\n", clientes[i].telefone);
            printf("Endereco: %s\n", clientes[i].endereco);
            printf("Email: %s\n", clientes[i].email);
            return i;
        }
    }
    return -7;
}
//Funcao para remover cliente por codigo
int removerCliente(struct Cliente clientes[], int *numClientes, const char codigo[]) {
    int indice = consultarCliente(clientes, *numClientes, codigo);
    if (indice == -7) return -8;
    for (int i = indice; i < *numClientes - 1; i++) {
        clientes[i] = clientes[i + 1];
    }
    (*numClientes)--;
    // Remove do arquivo - reescreve o arquivo sem o cliente deletado
    FILE *arq = fopen(ARQUIVO, "w");
    if (arq == NULL) {
        printf("Erro ao atualizar arquivo.\n");
        return -6;
    }
    
    for (int i = 0; i < *numClientes; i++) {
        fprintf(arq, "---------Dados do Cliente---------\n");
        fprintf(arq, "Codigo: %s\n", clientes[i].codigo);
        fprintf(arq, "Nome: %s\n", clientes[i].nome);
        fprintf(arq, "Tipo: %s\n", (clientes[i].tipo == 'F') ? "CPF" : "CNPJ");
        if (clientes[i].tipo == 'F') {
            fprintf(arq, "CPF: %s\n", clientes[i].documento);
        } else {
            fprintf(arq, "CNPJ: %s\n", clientes[i].documento);
        }
        fprintf(arq, "Telefone: %s\n", clientes[i].telefone);
        fprintf(arq, "Endereco: %s\n", clientes[i].endereco);
        fprintf(arq, "Email: %s\n", clientes[i].email);
        fprintf(arq, "--------------------------\n\n");
    }
    fclose(arq);
    return 0;
}
//Funcao para listar todos os clientes
void listarClientes(struct Cliente clientes[], int numClientes) {
    if (numClientes == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }
    printf("\n=== Lista de Clientes Cadastrados ===\n");
    for (int i = 0; i < numClientes; i++) {
        printf("%d) Codigo: %s | Nome: %s\n", i+1, clientes[i].codigo, clientes[i].nome);
    }
}
//Funcao para carregar clientes do arquivo na memoria
void carregarClientesDoArquivo() {
    FILE *arq = fopen(ARQUIVO, "r");
    if (arq == NULL) {
        printf("Arquivo nao existe ainda. Comecando com lista vazia.\n");
        return;
    }
    
    char linha[256];
    struct Cliente temp;
    memset(&temp, 0, sizeof(temp));
    int lendo = 0;
    
    while (fgets(linha, sizeof(linha), arq) != NULL) {
        if (strstr(linha, "Codigo:") != NULL) {
            sscanf(linha, "Codigo: %s", temp.codigo);
            lendo = 1;
        }
        else if (lendo && strstr(linha, "Nome:") != NULL) {
            sscanf(linha, "Nome: %[^\n]", temp.nome);
        }
        else if (lendo && strstr(linha, "Telefone:") != NULL) {
            sscanf(linha, "Telefone: %[^\n]", temp.telefone);
        }
        else if (lendo && strstr(linha, "Endereco:") != NULL) {
            sscanf(linha, "Endereco: %[^\n]", temp.endereco);
        }
        else if (lendo && strstr(linha, "Email:") != NULL) {
            sscanf(linha, "Email: %[^\n]", temp.email);
        }
        else if (lendo && strstr(linha, "CPF:") != NULL) {
            sscanf(linha, "CPF: %s", temp.documento);
            temp.tipo = 'F';
        }
        else if (lendo && strstr(linha, "CNPJ:") != NULL) {
            sscanf(linha, "CNPJ: %s", temp.documento);
            temp.tipo = 'J';
        }
        else if (lendo && strstr(linha, "--------------------------") != NULL) {
            if (numClientes < 100) {
                clientes[numClientes++] = temp;
            }
            memset(&temp, 0, sizeof(temp));
            lendo = 0;
        }
    }
    fclose(arq);
    printf("Clientes carregados do arquivo.\n");
}

int codigoPrincipal() {
    struct Cliente c;
    char cadastro[32];
    char tipo;
    char codigo[20];
    int resultado;
    char opcao;

    carregarClientesDoArquivo();  // ADICIONE ESTA LINHA no inicio do main
    
     while (1) {
        printf("\n=== Modulo Cliente ===\n");
        printf("C/c - Cadastrar | O/o - Consultar | R/r - Remover | L/l - Listar | 0 - Sair\n");
        printf("Escolha: ");
        scanf(" %c", &opcao);

        //Cadastrar cliente
        if(opcao == 'C' || opcao == 'c') {
           memset(&c, 0, sizeof(c)); // Zera a struct Cliente  // para evitar lixo de memória
            while (1) {
                printf("Codigo (somente numeros, 0 para cancelar): ");
                if (scanf("%19s", c.codigo) != 1) break;
        
                if (strcmp(c.codigo, "0") == 0) { 
                    printf("Operacao cancelada.\n"); break; } 
                if(!validarCodigo(c.codigo)){
                    printf("Codigo invalido. Use apenas numeros.\n");
                    continue;
                }
                resultado = cadastrarCliente(clientes, &numClientes, c);
              if(resultado == 1){
                printf("Codigo ja existe.\n"); 
                continue; 
              } 

            //Tipo de cliente F ou J
            do{
                    printf(" F=  pessoa fisica (CPF) ou J=pessoa juridica (CNPJ): ");
                    scanf(" %c", &tipo);
            } while (tipo != 'F' && tipo != 'f' && tipo != 'J' && tipo != 'j');
                if (tipo == 'F' || tipo == 'f') {
                do {
                    printf("CPF (11 digitos, somente numeros): ");
                    scanf("%31s", cadastro);
                } while(!validarCPF(cadastro));
                strcpy(c.documento, cadastro);
                c.tipo = 'F';
                printf("Nome: ");
                scanf(" %49[^\n]", c.nome);
                do {
                    printf("Telefone: ");
                    scanf(" %14[^\n]", c.telefone);
                } while(!verificarTelefone(c.telefone));
            } else {
                do {
                    printf("CNPJ (somente numero, 14 digitos): ");
                    scanf("%31s", cadastro);
                } while(!validarCNPJ(cadastro));
                strcpy(c.documento, cadastro);
                c.tipo = 'J';
                printf("Nome empresa: ");
                scanf(" %49[^\n]", c.nome);
                do {
                    printf("Telefone: ");
                    scanf(" %14[^\n]", c.telefone);
                } while(!verificarTelefone(c.telefone));
            }

            printf("Endereco: ");
            scanf(" %99[^\n]", c.endereco);
            do {
                printf("Email: ");
                scanf(" %49[^\n]", c.email);
            } while (!validarEmail(c.email));
//Cliente cadastrado
printf("\n--- Dados do Cliente ---\n");
    printf("Tipo: %s\n", (c.tipo == 'F'|| c.tipo== 'f') ? "Pessoa fisica (CPF)" : "Pessoa juridica (CNPJ)");
    printf("Documento: %s  %s\n ", (c.tipo == 'F'|| c.tipo== 'f') ? "CPF" : "CNPJ", cadastro);
    printf("Nome: %s\n", c.nome);
    printf("Telefone: %s\n", c.telefone);
    printf("Endereco: %s\n", c.endereco);
    printf("Email: %s\n", c.email);

     salvarCliente(c.codigo, c.nome, c.endereco, c.telefone,
                         (c.tipo == 'F') ? c.documento : "",
                         (c.tipo == 'J') ? c.documento : "",
                         (c.tipo == 'F') ? "CPF" : "CNPJ",
                         c.email);
    }   
}

        //Consultar cliente
        else if(opcao == 'O' || opcao == 'o') {
            while (1) {
                printf("Codigo (somente numeros, 0 para cancelar): ");
                if (scanf("%19s", codigo) != 1) break;
                if (strcmp(codigo, "0") == 0) { printf("Operacao cancelada.\n"); break; }
                if (!validarCodigo(codigo)) {
                    printf("Codigo invalido. Use apenas numeros.\n");
                    continue;
                }

                resultado = consultarCliente(clientes, numClientes, codigo);

                if (resultado != -7) {
                    printf("Cliente encontrado na memoria.\n");
                    break;
                }

                printf("Cliente nao encontrado na memoria.\nProcurando no arquivo...\n\n");

                // Buscar no arquivo
                FILE *arq = fopen(ARQUIVO, "r");
                if (arq == NULL) {
                    printf("Arquivo nao encontrado.\n");
                    break;
                } else {
                    char linha[256];
                    int encontrou = 0;

                    while (fgets(linha, sizeof(linha), arq) != NULL) {
                        if (strstr(linha, "Codigo:") != NULL && strstr(linha, codigo) != NULL) {
                            printf("\n=== Dados do Cliente (do arquivo) ===\n");
                            printf("%s", linha); // Imprime a linha "Codigo: XXXX"
                            encontrou = 1;

                            // Imprime as proximas linhas ate o separador
                            while (fgets(linha, sizeof(linha), arq) != NULL) {
                                if (strcmp(linha, "--------------------------\n") == 0) break;
                                printf("%s", linha);
                            }
                            break;
                        }
                    }
                    fclose(arq);

                    if (!encontrou) {
                        printf("Cliente nao encontrado no arquivo.\n");
                    }
                    break;
                }
            }
        }
//Remover
          else if(opcao == 'R' || opcao == 'r'){
            while (1) {
                printf("Codigo (0 para cancelar): ");
                if (scanf("%19s", codigo) != 1) break;
                if (strcmp(codigo, "0") == 0) { 
                    printf("Operacao cancelada.\n"); break; }
                if (!validarCodigo(codigo)) {
                    printf("Codigo invalido. Use apenas numeros.\n");
                    continue;
                }
                resultado = removerCliente(clientes, &numClientes, codigo);
                if (resultado == -8) {
                    printf("Nao encontrado.\n");
                    // pergunta se quer tentar novamente ou cancelar
                    printf("Deseja tentar outro codigo? (S/N): ");
                    char resp;
                    scanf(" %c", &resp);
                    if (resp == 'S' || resp == 's') continue;
                    else break;
                } else {
                    printf("Removido.\n");
                    break;
                }
            }
        }
//Listar clientes
        else if(opcao == 'L' || opcao == 'l'){
            listarClientes(clientes, numClientes);  
        }
// Sair
        else if(opcao == '0'){
            printf("Saindo.\n");
            break;
        }
        else printf("Invalido.\n");
    }
    return 0;


 }
