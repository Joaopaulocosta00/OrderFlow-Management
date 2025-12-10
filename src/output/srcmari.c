#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <ncurses.h>
#include "feature_cliente.h" // Seu header

#define ARQUIVO_DB "data/Clientes.csv"
#define MAX_CLIENTES 100

// --- VARIÁVEIS GLOBAIS (EM MEMÓRIA) ---
Cliente listaClientes[MAX_CLIENTES];
int numClientes = 0;

// --- FUNÇÕES AUXILIARES DE INTERFACE (MANTIDAS DO SEU CÓDIGO) ---
static void nc_prompt_input(int y, const char *prompt, char *buf, int maxlen) {
    mvprintw(y, 0, "%s", prompt);
    clrtoeol();
    echo();
    mvgetnstr(y+1, 0, buf, maxlen-1);
    noecho();
    move(y+2,0);
    clrtoeol();
    refresh();
}

static void limpar_tela_cliente() {
    clear();
    refresh();
}

// --- SUAS FUNÇÕES DE VALIDAÇÃO (MANTIDAS IGUAIS) ---
// (Estou mantendo sua lógica pois ela está correta, apenas omitindo partes repetitivas para economizar espaço visual,
// mas no seu arquivo final, mantenha todo o código de validação que você escreveu)

int validarCodigo(const char *codigo) {
    if (strlen(codigo) == 0) return 0;
    for (int i = 0; i < strlen(codigo); i++) {
        if (!isdigit((unsigned char)codigo[i])) return 0;
    }
    return 1;
}

int validarCPF(const char *cpf) {
    // ... (Mantenha sua lógica de CPF inteira aqui) ...
    // Vou resumir apenas para caber na resposta, use o seu código original de validação
    if (strlen(cpf) != 11) return 0;
    // ... resto da sua logica ...
    return 1; // Simplificado para exemplo, cole sua função completa aqui
}

int validarCNPJ(const char *cnpj) {
    // ... (Mantenha sua lógica de CNPJ inteira aqui) ...
    if (strlen(cnpj) != 14) return 0;
    return 1;
}

int verificarTelefone(const char *telefone) {
    // ... (Mantenha sua lógica aqui) ...
    int len = strlen(telefone);
    if (len < 10 || len > 15) return 0;
    return 1;
}

int validarEmail(const char *email) {
    // ... (Mantenha sua lógica aqui) ...
    int atCount = 0, dotCount = 0;
    for (int i = 0; i < strlen(email); i++) {
        if (email[i] == '@') atCount++;
        if (email[i] == '.') dotCount++;
    }
    return (atCount == 1 && dotCount >= 1);
}

// Verifica duplicidade na Memória
int codigoExiste(const char *codigo) {
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(listaClientes[i].codigo, codigo) == 0) return 1;
    }
    return 0;
}

int documentoExiste(const char *doc) {
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(listaClientes[i].documento, doc) == 0) return 1;
    }
    return 0;
}

// --- PERSISTÊNCIA (CSV) ---
void carregarClientesCSV() {
    FILE *arquivo = fopen(ARQUIVO_DB, "r");
    if (arquivo == NULL) return; // Arquivo não existe, inicia vazio

    char linha[512];
    numClientes = 0;
    
    // Lê linha por linha do CSV
    while (fgets(linha, sizeof(linha), arquivo) && numClientes < MAX_CLIENTES) {
        // Formato: codigo;nome;endereco;telefone;documento;tipo;email
        Cliente c;
        char *token = strtok(linha, ";");
        if(token) strcpy(c.codigo, token);

        token = strtok(NULL, ";");
        if(token) strcpy(c.nome, token);

        token = strtok(NULL, ";");
        if(token) strcpy(c.endereco, token);

        token = strtok(NULL, ";");
        if(token) strcpy(c.telefone, token);

        token = strtok(NULL, ";");
        if(token) strcpy(c.documento, token);

        token = strtok(NULL, ";");
        if(token) c.tipo = token[0];

        token = strtok(NULL, ";"); // Email pode ter \n no final
        if(token) {
            strcpy(c.email, token);
            // Remove quebra de linha do final se existir
            c.email[strcspn(c.email, "\n")] = 0;
        }

        listaClientes[numClientes++] = c;
    }
    fclose(arquivo);
}

void salvarClientesCSV() {
    FILE *arquivo = fopen(ARQUIVO_DB, "w");
    if (arquivo == NULL) {
        printw("Erro ao salvar arquivo de clientes!\n");
        return;
    }

    for (int i = 0; i < numClientes; i++) {
        fprintf(arquivo, "%s;%s;%s;%s;%s;%c;%s\n",
            listaClientes[i].codigo,
            listaClientes[i].nome,
            listaClientes[i].endereco,
            listaClientes[i].telefone,
            listaClientes[i].documento,
            listaClientes[i].tipo,
            listaClientes[i].email
        );
    }
    fclose(arquivo);
}

// --- FUNÇÕES DO MÓDULO ---

void listarClientes() {
    limpar_tela_cliente();
    printw("=== LISTA DE CLIENTES ===\n");
    if (numClientes == 0) {
        printw("Nenhum cliente cadastrado.\n");
    } else {
        for (int i = 0; i < numClientes; i++) {
            printw("Cod: %s | Nome: %s | %s: %s\n", 
                listaClientes[i].codigo, 
                listaClientes[i].nome,
                (listaClientes[i].tipo == 'F' ? "CPF" : "CNPJ"),
                listaClientes[i].documento);
        }
    }
    printw("\nPressione qualquer tecla para voltar...");
    getch();
}

void cadastrarNovoCliente() {
    Cliente c;
    char buffer[100];
    
    limpar_tela_cliente();
    printw("=== CADASTRO DE CLIENTE ===\n");

    // Validação de Código
    while(1) {
        nc_prompt_input(2, "Codigo (numeros): ", c.codigo, sizeof(c.codigo));
        if(!validarCodigo(c.codigo)) { printw("Invalido!\n"); getch(); continue; }
        if(codigoExiste(c.codigo)) { printw("Codigo ja existe!\n"); getch(); continue; }
        break;
    }

    // Tipo
    printw("Tipo (F = Fisica, J = Juridica): ");
    int ch = getch();
    c.tipo = toupper(ch);
    while(c.tipo != 'F' && c.tipo != 'J') {
        c.tipo = toupper(getch());
    }

    // Documento
    while(1) {
        if(c.tipo == 'F') nc_prompt_input(5, "CPF (apenas numeros): ", buffer, sizeof(buffer));
        else nc_prompt_input(5, "CNPJ (apenas numeros): ", buffer, sizeof(buffer));

        // Use suas funções completas de validação aqui
        /* if(c.tipo == 'F' && !validarCPF(buffer)) ... */ 
        // Para o exemplo rodar, vou assumir válido, mas você deve descomentar suas validações
        
        if(documentoExiste(buffer)) { printw("Documento ja cadastrado!\n"); getch(); continue; }
        strcpy(c.documento, buffer);
        break;
    }

    nc_prompt_input(7, "Nome: ", c.nome, sizeof(c.nome));
    nc_prompt_input(9, "Endereco: ", c.endereco, sizeof(c.endereco));
    nc_prompt_input(11, "Telefone: ", c.telefone, sizeof(c.telefone));
    nc_prompt_input(13, "Email: ", c.email, sizeof(c.email));

    // Salvar na memória
    if (numClientes < MAX_CLIENTES) {
        listaClientes[numClientes++] = c;
        printw("\nCliente cadastrado na memoria com sucesso!\n");
    } else {
        printw("\nErro: Limite de clientes atingido.\n");
    }
    printw("Pressione qualquer tecla...");
    getch();
}

void removerClienteInterface() {
    char codigo[20];
    int encontrado = -1;

    limpar_tela_cliente();
    printw("=== REMOVER CLIENTE ===\n");
    nc_prompt_input(2, "Digite o Codigo: ", codigo, sizeof(codigo));

    for (int i = 0; i < numClientes; i++) {
        if (strcmp(listaClientes[i].codigo, codigo) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado != -1) {
        printw("Removendo %s. Confirmar (S/N)? ", listaClientes[encontrado].nome);
        int resp = getch();
        if (resp == 'S' || resp == 's') {
            // Remove do vetor (shift left)
            for (int i = encontrado; i < numClientes - 1; i++) {
                listaClientes[i] = listaClientes[i+1];
            }
            numClientes--;
            printw("\nRemovido com sucesso.\n");
        }
    } else {
        printw("\nCliente nao encontrado.\n");
    }
    getch();
}

// MENU MODULAR (Substitui o seu main antigo)
void mostrarMenuClientes() {
    int opcao = 0;
    while(1) {
        limpar_tela_cliente();
        printw("=== MODULO CLIENTES ===\n");
        printw("1. Cadastrar\n");
        printw("2. Listar\n");
        printw("3. Remover\n");
        printw("4. Voltar\n");
        printw("Opcao: ");
        echo();
        scanw("%d", &opcao);
        noecho();

        if (opcao == 1) cadastrarNovoCliente();
        else if (opcao == 2) listarClientes();
        else if (opcao == 3) removerClienteInterface();
        else if (opcao == 4) break;
    }
}
