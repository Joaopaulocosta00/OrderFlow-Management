#include <stdio.h>
#include <string.h>

struct Cliente {
    int id;
    char nome[50];
    char endereco[100];
    char telefone[15];
    char documento[20]; // CPF ou CNPJ (apenas digitos)
    char tipo; // 'F' = CPF, 'J' = CNPJ
    char email[50];
};
//Funcao para cadastrar cliente
int cadastrarCliente(char clientes[][20], int numClientes, char codigo[]) {   // clientes[][]-Lista de clientes, numClientes-número de clientes cadastrados, codigo- Código do cliente
    int existe = 0;
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i], codigo) == 0) {
            existe = 1;
            break;
        }
    }
    return existe;
}
////////////////////////////////////////////////////////////////////
// Funcao para validar CPF
int validarCPF(const char *cpf) {
    int i, j, digito1 = 0, digito2 = 0;
    if (strlen(cpf) != 11) 
        return 0;
    for (i = 0; i < 11; i++) {
        if (cpf[i] < '0' || cpf[i] > '9') 
            return 0;
    }
    for (i = 0, j = 10; i < 9; i++, j--) {
        digito1 += (cpf[i] - '0') * j;
    }
    digito1 = (digito1 * 10) % 11;
    if (digito1 == 10) digito1 = 0;
    for (i = 0, j = 11; i < 10; i++, j--) {
        digito2 += (cpf[i] - '0') * j;
    }
    digito2 = (digito2 * 10) % 11;
    if (digito2 == 10) digito2 = 0;
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

// verifica o telefone
int verificarTelefone(const char *telefone) {
    int i;
    int len = strlen(telefone);
    if (len < 10 || len > 15) return 0; // tamanho invalido
    for (i = 0; i < len; i++) {
        if ((telefone[i] < '0' || telefone[i] > '9') && telefone[i] != '+' && telefone[i] != '-' && telefone[i] != ' ') {
            return 0; // caractere invalido
        }
    }
    return 1; // telefone valido
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

// Programa principal
int teste() {
    struct Cliente c;
    char cadastro[32];
    char tipo;
    char clientes[100][20];
    int numClientes = 0;
    char codigo;
    int existe=0;
    /*printf("Digite o codigo do cliente: ");
    if (scanf(" %c", &codigo) != 1) return 0;
    existe = cadastrarCliente(clientes, numClientes, &codigo);
    if (existe) {
        printf("Erro: O codigo ja existe.\n");
        return 0;
    } else {
        strcpy(clientes[numClientes], &codigo);
        numClientes++;
        printf("Cliente cadastrado com sucesso!\n");
    }*/
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Digite F para pessoa fisica (CPF) ou J para pessoa juridica (CNPJ): ");
    if (scanf(" %c", &tipo) != 1) return 0; // note o espaco antes de %c para pular whitespace

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
        //gravar documento e tipo
        strcpy(c.documento, cadastro);
        c.tipo = tipo;
        // Coleta outros dados do cliente
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
        
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
        printf("Digite o nome da empresa: ");
        scanf(" %[^\n]", c.nome);
        do {
        printf("Digite o numero de telefone: ");
        scanf(" %[^\n]", c.telefone);
        if(!verificarTelefone(c.telefone)) {
            printf("Telefone invalido! Tente novamente.\n");
        }
    } while(!verificarTelefone(c.telefone));
        printf("Telefone valido!\n");}
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
    printf("Tipo: %s\n", (c.tipo == 'F') ? "Pessoa fisica (CPF)" : "Pessoa juridica (CNPJ)");
    printf("Documento: %s\n", cadastro);
    printf("Nome: %s\n", c.nome);
    printf("Telefone: %s\n", c.telefone);
    printf("Endereco: %s\n", c.endereco);
    printf("Email: %s\n", c.email);
    return 0;
}
