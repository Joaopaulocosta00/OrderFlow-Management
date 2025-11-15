#include "cliente.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct Cliente {
    char codigo[20];
    char nome[50];
    char endereco[100];
    char telefone[15];
    char documento[20]; // CPF ou CNPJ (apenas digitos)
    char tipo; // 'F' = CPF, 'J' = CNPJ
    char email[50];
};
//Funcao para cadastrar cliente
int cadastrarCliente(char clientes[][20], int *numClientes, const char codigo[]) {   // clientes[][]-Lista de clientes, numClientes-número de clientes cadastrados, codigo- Código do cliente
    //Limite de cadastro
    if (*numClientes >= 100) {
        printf("Limite de clientes cadastrados atingido.\n");
        return -11; // -11= limite atingido
    }
    //Verifica se o codigo ja existe
    for (int i = 0; i < *numClientes; i++) {
        if (strcmp(clientes[i], codigo) == 0) {
            return 1; // Ja existe
        }
    }
    //Abre arquivo para acrescentar cliente
    FILE *arquivo = fopen("clientes.txt", "a");
    if(arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return -1; // -1= erro ao abrir arquivo
    }
        //Grava no arquivo
        fprintf(arquivo, "---------Dados do Cliente---------\n");
        fprintf(arquivo, "Codigo do cliente: %s\n", codigo);
        fclose(arquivo);
        //Adiciona na lista de memoria
        strncpy(clientes[*numClientes], codigo, 19);
        clientes[*numClientes][19] = '\0'; // Garantir terminação nula
        (*numClientes)++;
        return 0; // 0= sucesso
}
////////////////////////////////////////////////////////////////////
//Fucao para validar CPF
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
//Função para validar telefone
int verificarTelefone(const char *telefone) {
    int i;
    int len = strlen(telefone);
    if (len < 10 || len > 15) return 0; // tamanho invalido
    for (i = 0; i < len; i++) {
        if ((telefone[i] < '0' || telefone[i] > '9') && telefone[i] != '+' && telefone[i] != '-' && telefone[i] != ' ') {
            return 0; // caractere invalido
        }
    }
    return 4; // telefone valido
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
//Arquivo para guardar nome, endereco, telefone, documento, tipo e email
int salvarCliente(const char *nome, const char *endereco, const char *telefone, const char *cpf, const char *cnpj, const char *tipo, const char *email) {
FILE *arquivo = fopen("clientes.txt", "a");
if(arquivo == NULL) {
    printf("Erro ao abrir o arquivo!\n");
    return -6; // -6= erro ao abrir arquivo
    }

    fprintf(arquivo,"Tipo: %s\n", tipo);
    fprintf(arquivo, "Documento: %s %s\n", (strcmp(tipo, "Pessoa fisica (CPF)") == 0) ? "CPF" : "CNPJ", (strcmp(tipo, "Pessoa fisica (CPF)") == 0) ? cpf : cnpj);
    fprintf(arquivo, "Nome: %s\n", nome);
    fprintf(arquivo, "Telefone: %s\n", telefone);
    fprintf(arquivo, "Endereco: %s\n", endereco);
    fprintf(arquivo, "Email: %s\n", email);
    fprintf(arquivo, "--------------------------\n");
    fclose(arquivo);
    return 0; // 0= sucesso 
    
}   
//Funcao consultar cliente
int consultarCliente(const char clientes[][20], int numClientes, const char codigo[]) {
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i], codigo) == 0) {
            return i; // Retorna o índice do cliente encontrado
        }
    }
    return -7; // Cliente não encontrado
}
//Funcao para remover cliente
int removerCliente(char clientes[][20], int *numClientes, const char codigo[]) {
    int indice = consultarCliente(clientes, *numClientes, codigo);
    if (indice == -7) {
        return -8; // Cliente não encontrado
    }
    // Move os clientes seguintes para preencher o espaço
    for (int i = indice; i < *numClientes - 1; i++) {
        strcpy(clientes[i], clientes[i + 1]);
    }
    (*numClientes)--;
    return 0; // Sucesso
}
//Funcao para listar todos os clientes
void listarClientes(const char clientes[][20], int numClientes) {
    for (int i = 0; i < numClientes; i++) {
        salvarCliente(clientes[i], "", "", "", "", "", ""); // Chama salvarCliente para cada cliente
    }
}
// Programa principal
int codigoPrincipal() {
    struct Cliente c;
    char cadastro[32];
    char tipo;
    char clientes[100][20];
    int numClientes = 0;
    char codigo[20];
    int resultado;
    char nomeempresa[50];
    char opcao;

    while (1) {
    printf("=== Modulo Cliente ===\n");
    printf("Digite C/c para cadastrar um cliente, \n");
    printf("Digite O/o para consultar um cliente, \n");
    printf("Digite R/r para remover um cliente, \n");
    printf("Digite L/l para listar todos os clientes cadastrados: \n");
    printf("0 para sair do programa.\n");
    printf("Escolha uma opcao: ");
    scanf(" %c", &opcao);
    
    //Cadastro do cliente id 
    if(opcao == 'C' || opcao == 'c') {
   do{
    printf("Digite o codigo do cliente: ");
    if (scanf(" %19s", codigo) != 1) 
    return 0;
    resultado = cadastrarCliente(clientes, &numClientes, codigo);
    if (resultado == 1) {
        printf("Erro: O codigo ja existe. Tente novamente.\n");
    } else if (resultado == -1) {
        printf("Erro ao abrir o arquivo. Tente novamente.\n");
     } else if (resultado == -11) {
        return 0; // Limite atingido, sai do programa
     }
    } while (resultado != 0);
        printf("Cliente cadastrado com sucesso!\n");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Tipo de cliente F ou J
   do{
    printf("Digite F para pessoa fisica (CPF) ou J para pessoa juridica (CNPJ): ");
    if (scanf(" %c", &tipo) != 1) return 0; // note o espaco antes de %c para pular whitespace
    if (tipo != 'F' && tipo != 'f' && tipo != 'J' && tipo != 'j') {
        printf("Tipo invalido! Tente novamente.\n");
    }
    } while (tipo != 'F' && tipo != 'f' && tipo != 'J' && tipo != 'j');
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Pessoa fisica (CPF)
    if (tipo == 'F' || tipo == 'f') {
        //Ler e validar CPF em loop
        do{
        printf("Digite o CPF (apenas numeros): ");
        if (scanf("%31s", cadastro) != 1) return 0; 
        if(!validarCPF(cadastro)) {
            printf("CPF invalido! Tente novamente.\n");
        }
    } while(!validarCPF(cadastro));
        printf("CPF valido!\n");
        //gravar documento e tipo-------------------------
        strcpy(c.documento, cadastro);
        c.tipo = tipo;

        // Coleta outros dados do cliente------------------
        printf("Digite o nome: ");
        scanf(" %[^\n]", c.nome);
        do {
        printf("Digite o numero de telefone: ");
        scanf(" %[^\n]", c.telefone);
        if(!verificarTelefone(c.telefone)) {
            printf("Telefone invalido! Tente novamente.\n");
        }
    } while(!verificarTelefone(c.telefone));
        printf("Telefone valido!\n");

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    } else if (tipo == 'J' || tipo == 'j') {
        do{
        printf("Digite o CNPJ (apenas numeros): ");
        if (scanf("%31s", cadastro) != 1) return 0;
         if(!validarCNPJ(cadastro)) {
            printf("CNPJ invalido! Tente novamente. \n");
        }
    } while(!validarCNPJ(cadastro));
        printf("CNPJ valido!\n");

        //Razão social e o numero de contato
    printf("Digite o nume da empresa: ");
        scanf(" %[^\n]", &nomeempresa);
        printf("Digite o nome da empresa: ");
        scanf(" %[^\n]", c.nome);
        do {
        printf("Digite o numero de telefone: ");
        scanf(" %[^\n]", c.telefone);
        if(!verificarTelefone(c.telefone)) {
            printf("Telefone invalido! Tente novamente.\n");
        }
    } while(!verificarTelefone(c.telefone));
        printf("Telefone valido!\n");
    }
//////////////////////////////////////////////////////////////////////
// Endereço do cliente - Rua , Setor , Cidade , Estado 
        printf("Digite o endereco: ");
        scanf(" %[^\n]", c.endereco);
// Email do cliente
  do{
    printf("Digite o email: ");
    scanf(" %[^\n]", c.email);
    if(!validarEmail(c.email)) {
        printf("Email invalido! Tente novamente.\n"); }
       } while (!validarEmail(c.email));
        printf("Email valido!\n");
        
  
///////////////////////////////////////////////////////////////////////////////////////////////
// Cliente cadastrado com sucesso
//////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("\n--- Dados do Cliente ---\n");
    printf("Tipo: %s\n", (c.tipo == 'F'|| c.tipo== 'f') ? "Pessoa fisica (CPF)" : "Pessoa juridica (CNPJ)");
    printf("Documento: %s  %s\n ", (c.tipo == 'F'|| c.tipo== 'f') ? "CPF" : "CNPJ", cadastro);
    printf("Nome: %s\n", c.nome);
    printf("Telefone: %s\n", c.telefone);
    printf("Endereco: %s\n", c.endereco);
    printf("Email: %s\n", c.email);
    
    salvarCliente(c.nome, c.endereco, c.telefone, (c.tipo == 'F'|| c.tipo== 'f') ? cadastro : "", (c.tipo == 'J'|| c.tipo== 'j') ? cadastro : "", (c.tipo == 'F'|| c.tipo== 'f') ? "Pessoa fisica (CPF)" : "Pessoa juridica (CNPJ)", c.email);
    }
    //---------------------------------------------------------------------------------------------------------------------------------
//Consular cliente
    if(opcao == 'O' || opcao == 'o') {
    do {
    printf("Digite o codigo do cliente para consultar: ");
    if (scanf(" %19s", codigo) != 1) return 0;
    resultado = consultarCliente(clientes, numClientes, codigo);
    if (resultado == -7) {
        printf("Cliente nao encontrado. Tente novamente.\n");
    } 
    }while(resultado != -7);
    printf("Cliente encontrado no indice: %d\n", resultado);
    }

//---------------------------------------------------------------------------------------------------------------------------------
//Remover cliente
if(opcao == 'R' || opcao == 'r'){
    do {
    printf("Digite o codigo do cliente para remover: ");
    if (scanf(" %c", codigo) != 1) return 0;
    resultado = removerCliente(clientes, &numClientes, codigo);
    if (resultado == -8) {
        printf("Cliente nao encontrado. Tente novamente.\n");
    }
 } while(resultado != -8);
    printf("Cliente removido com sucesso!\n");
}
   
//---------------------------------------------------------------------------------------------------------------------------------
//Listar clientes
if(opcao == 'L' || opcao == 'l'){
printf("Listando todos os clientes cadastrados:\n");
listarClientes(clientes, numClientes);
printf("Total de clientes cadastrados: %d\n", numClientes);
printf("%d", resultado);
}
if(opcao == '0'){
    printf("Saindo do programa.\n");
    break;
}

return 0;
}
}
