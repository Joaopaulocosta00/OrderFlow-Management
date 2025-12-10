#ifndef FEATURE_CLIENTE_H
#define FEATURE_CLIENTE_H

typedef struct {
    char codigo[20];
    char nome[50];
    char endereco[100];
    char telefone[15];
    char documento[20];
    char tipo;
    char email[50];
} Cliente;

void carregarClientesCSV();
void salvarClientesCSV();

// Funcoes de Interface
void cadastrarCliente();
void listarClientes();
void removerClienteInterface();
void consultarCliente(); 

#endif