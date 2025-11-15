#ifndef CLIENTE_H
#define CLIENTE_H
#include <stdio.h>

int cadastrarCliente(char clientes[][20], int *numClientes, const char codigo[]);

int validarCPF(const char *cpf);

int validarCNPJ(const char *cnpj);

int validarTelefone(const char *telefone);

int validarEmail(const char *email);

int salvarCliente(const char *nome, const char *endereco, const char *telefone, const char *cpf, const char *cnpj, const char *tipo, const char *email);

int consultarCliente(const char clientes[][20], int numClientes, const char codigo[]);

int removerCliente(char clientes[][20], int *numClientes, const char codigo[]);

void listarClientes(const char clientes[][20], int numClientes);

#endif // CLIENTE_H
