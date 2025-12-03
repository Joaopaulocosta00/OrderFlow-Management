#ifndef FEATURE_CLIENTE_H
#define FEATURE_CLIENTE_H

typedef struct {
    char codigo[20];
    char nome[50];
    char endereco[100];
    char telefone[15];
    char documento[20]; // CPF ou CNPJ
    char tipo; // 'F' = CPF, 'J' = CNPJ
    char email[50];
} Cliente;

// Funções principais
void carregarClientesCSV();
void salvarClientesCSV();
void mostrarMenuClientes();

#endif
