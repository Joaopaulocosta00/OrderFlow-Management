#include "feature_interface.h" // Importa ncurses e ferramentas visuais
#include "feature_cliente.h"   // Importa a Struct Cliente
#include <ctype.h>             // Para isdigit, toupper

#define ARQUIVO_CLIENTES "data/Clientes.csv"
#define MAX_CLIENTES 100

// --- BANCO DE DADOS EM MEMÓRIA ---
Cliente listaClientes[MAX_CLIENTES];
int numClientes = 0;

// ==============================================================================
// 1. FUNÇÕES DE VALIDAÇÃO (Lógica Matemática e de Negócio)
// ==============================================================================

// Verifica se tem apenas números
int validarDigitos(const char *str) {
    if (strlen(str) == 0) return 0;
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit((unsigned char)str[i])) return 0;
    }
    return 1;
}

// Verifica se o Código já existe
int codigoExiste(const char *codigo) {
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(listaClientes[i].codigo, codigo) == 0) return 1;
    }
    return 0;
}

// Verifica se o Documento (CPF/CNPJ) já existe
int documentoExiste(const char *doc) {
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(listaClientes[i].documento, doc) == 0) return 1;
    }
    return 0;
}

// Validar CPF
int validarCPF(const char *cpf) {
    int i, j, digito1 = 0, digito2 = 0;
    if (strlen(cpf) != 11) return 0;
    if (!validarDigitos(cpf)) return 0;

    int todosIguais = 1;
    for (i = 1; i < 11; i++) {
        if (cpf[i] != cpf[0]) { todosIguais = 0; break; }
    }
    if (todosIguais) return 0;

    for (i = 0, j = 10; i < 9; i++, j--) digito1 += (cpf[i] - '0') * j;
    digito1 = (digito1 * 10) % 11;
    if (digito1 == 10) digito1 = 0;

    for (i = 0, j = 11; i < 10; i++, j--) digito2 += (cpf[i] - '0') * j;
    digito2 = (digito2 * 10) % 11;
    if (digito2 == 10) digito2 = 0;

    return (digito1 == (cpf[9] - '0') && digito2 == (cpf[10] - '0'));
}

// Validar CNPJ
int validarCNPJ(const char *cnpj) {
    int i, digito1 = 0, digito2 = 0;
    int pesos1[12] = {5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2};
    int pesos2[13] = {6, 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2};
    
    if (strlen(cnpj) != 14) return 0;
    if (!validarDigitos(cnpj)) return 0;

    for (i = 0; i < 12; i++) digito1 += (cnpj[i] - '0') * pesos1[i];
    digito1 = digito1 % 11;
    digito1 = (digito1 < 2) ? 0 : 11 - digito1;

    for (i = 0; i < 13; i++) digito2 += (cnpj[i] - '0') * pesos2[i];
    digito2 = digito2 % 11;
    digito2 = (digito2 < 2) ? 0 : 11 - digito2;

    return (digito1 == (cnpj[12] - '0') && digito2 == (cnpj[13] - '0'));
}

// Validação de Email
int validarEmail(const char *email) {
    int atCount = 0, dotCount = 0;
    for (int i = 0; i < strlen(email); i++) {
        if (email[i] == '@') atCount++;
        if (email[i] == '.') dotCount++;
    }
    return (atCount == 1 && dotCount >= 1);
}

// ==============================================================================
// 2. FUNÇÕES DE PERSISTÊNCIA (Carregar e Salvar CSV)
// ==============================================================================

void carregarClientesCSV() {
    FILE *f = fopen(ARQUIVO_CLIENTES, "r");
    if (!f) return;
    
    char linha[512];
    numClientes = 0;
    while(fgets(linha, sizeof(linha), f) && numClientes < MAX_CLIENTES) {
        Cliente c;
        // Parse manual do CSV: codigo;nome;endereco;telefone;documento;tipo;email
        char *token = strtok(linha, ";"); if(token) strcpy(c.codigo, token);
        token = strtok(NULL, ";"); if(token) strcpy(c.nome, token);
        token = strtok(NULL, ";"); if(token) strcpy(c.endereco, token);
        token = strtok(NULL, ";"); if(token) strcpy(c.telefone, token);
        token = strtok(NULL, ";"); if(token) strcpy(c.documento, token);
        token = strtok(NULL, ";"); if(token) c.tipo = token[0];
        token = strtok(NULL, ";"); 
        if(token) {
            strcpy(c.email, token);
            c.email[strcspn(c.email, "\n")] = 0; // Remove \n
        }
        listaClientes[numClientes++] = c;
    }
    fclose(f);
}

void salvarClientesCSV() {
    FILE *f = fopen(ARQUIVO_CLIENTES, "w");
    if (!f) return;
    
    for(int i=0; i<numClientes; i++) {
        fprintf(f, "%s;%s;%s;%s;%s;%c;%s\n", 
            listaClientes[i].codigo, 
            listaClientes[i].nome, 
            listaClientes[i].endereco, 
            listaClientes[i].telefone, 
            listaClientes[i].documento, 
            listaClientes[i].tipo,
            listaClientes[i].email);
    }
    fclose(f);
}

/* ==============================================================================
 3. FUNÇÕES DE FORMULÁRIO (Telas Específicas do Cliente)
 ==============================================================================*/

void cadastrarCliente() {
    Cliente c;
    int valido = 0;
    
    mostrarCabecalho("CADASTRAR CLIENTE");
    if (numClientes >= MAX_CLIENTES) {
        printw("Erro: Limite de clientes atingido.\n");
        pausa();
        return;
    }

    echo(); // Habilita o usuário ver o que digita

    // -- Validação do Código --
    do {
        printw("\nCodigo (apenas numeros): ");
        getnstr(c.codigo, 19);
        
        if (!validarDigitos(c.codigo)) {
            printw(" > Erro: Apenas numeros sao aceitos.\n");
        } else if (codigoExiste(c.codigo)) {
            printw(" > Erro: O codigo '%s' ja existe!\n", c.codigo);
        } else {
            valido = 1;
        }
    } while (!valido);

    // -- Validação do Tipo --
    valido = 0;
    do {
        printw("Tipo (F - Fisica / J - Juridica): ");
        int ch = getch();
        c.tipo = toupper(ch);
        printw("%c\n", c.tipo); // Mostra o que foi digitado
        
        if (c.tipo == 'F' || c.tipo == 'J') valido = 1;
        else printw(" > Erro: Use F ou J.\n");
    } while(!valido);

    // -- Validação do Documento (CPF/CNPJ) --
    valido = 0;
    do {
        if (c.tipo == 'F') printw("CPF (11 digitos): ");
        else printw("CNPJ (14 digitos): ");
        
        getnstr(c.documento, 19);

        if (c.tipo == 'F') {
            if (!validarCPF(c.documento)) printw(" > Erro: CPF invalido!\n");
            else if (documentoExiste(c.documento)) printw(" > Erro: CPF ja cadastrado!\n");
            else valido = 1;
        } else {
            if (!validarCNPJ(c.documento)) printw(" > Erro: CNPJ invalido!\n");
            else if (documentoExiste(c.documento)) printw(" > Erro: CNPJ ja cadastrado!\n");
            else valido = 1;
        }
    } while(!valido);

    // -- Outros Dados --
    printw("Nome Completo: ");
    getnstr(c.nome, 49);

    printw("Telefone: ");
    getnstr(c.telefone, 14);

    printw("Endereco: ");
    getnstr(c.endereco, 99);

    do {
        printw("Email: ");
        getnstr(c.email, 49);
        if(!validarEmail(c.email)) printw(" > Erro: Email invalido.\n");
    } while(!validarEmail(c.email));

    noecho(); // Desabilita o echo

    // -- Confirmação --
    printw("\nSalvar cadastro? (S/N): ");
    int confirm = getch();
    if (toupper(confirm) == 'S') {
        listaClientes[numClientes] = c;
        numClientes++;
        printw("\n\nCliente cadastrado com sucesso!");
    } else {
        printw("\n\nCadastro cancelado.");
    }
    
    pausa();
}

void listarClientes() {
    mostrarCabecalho("LISTA DE CLIENTES");
    
    if (numClientes == 0) {
        printw("Nenhum cliente cadastrado.\n");
    } else {
        // Cabeçalho da Tabela
        printw("%-8s %-25s %-16s %-5s\n", "COD", "NOME", "DOC", "TIPO");
        printw("--------------------------------------------------------\n");
        
        for (int i = 0; i < numClientes; i++) {
            printw("%-8s %-25s %-16s %-5c\n", 
                listaClientes[i].codigo, 
                listaClientes[i].nome, 
                listaClientes[i].documento, 
                listaClientes[i].tipo);
        }
    }
    pausa();
}

void removerClienteInterface() {
    char codigo[20];
    int index = -1;

    mostrarCabecalho("REMOVER CLIENTE");
    echo();
    printw("Digite o codigo do cliente a remover: ");
    getnstr(codigo, 19);
    noecho();

    // Busca o índice no vetor
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(listaClientes[i].codigo, codigo) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printw("\nErro: Cliente nao encontrado.\n");
    } else {
        printw("\nTem certeza que deseja remover '%s'? (S/N): ", listaClientes[index].nome);
        int conf = getch();
        if (toupper(conf) == 'S') {
            // Remove deslocando elementos para a esquerda
            for (int i = index; i < numClientes - 1; i++) {
                listaClientes[i] = listaClientes[i+1];
            }
            numClientes--;
            printw("\nCliente removido com sucesso.\n");
        } else {
            printw("\nOperacao cancelada.\n");
        }
    }
    pausa();

    
}

void consultarCliente() {
    char codigo[20];
    int index = -1;

    mostrarCabecalho("CONSULTAR CLIENTE");
    
    echo();
    printw("Digite o codigo do cliente: ");
    getnstr(codigo, 19);
    noecho();

    // Busca no vetor
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(listaClientes[i].codigo, codigo) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printw("\nErro: Cliente nao encontrado.\n");
    } else {
        printw("\n--- DADOS DO CLIENTE ---\n");
        printw("Codigo:    %s\n", listaClientes[index].codigo);
        printw("Nome:      %s\n", listaClientes[index].nome);
        printw("Documento: %s (%c)\n", listaClientes[index].documento, listaClientes[index].tipo);
        printw("Telefone:  %s\n", listaClientes[index].telefone);
        printw("Endereco:  %s\n", listaClientes[index].endereco);
        printw("Email:     %s\n", listaClientes[index].email);
    }
    pausa();
}
