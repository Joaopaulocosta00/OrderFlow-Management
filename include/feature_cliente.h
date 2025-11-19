#ifndef CLIENTE_H
#define CLIENTE_H

int cadastrarCliente(struct Cliente clientes[], int *numClientes, struct Cliente novo);

int validarCodigo(const char *codigo);

int validarCPF(const char *cpf);

int validarCNPJ(const char *cnpj);

int verificarTelefone(const char *telefone) ;

int validarEmail(const char *email);

int salvarCliente(const char *codigo, const char *nome, const char *endereco, const char *telefone, const char *cpf, const char *cnpj, const char *tipo, const char *email);

int consultarCliente(struct Cliente clientes[], int numClientes, const char codigo[]);

int removerCliente(struct Cliente clientes[], int *numClientes, const char codigo[]);

void listarClientes(struct Cliente clientes[], int numClientes);

void carregarClientesDoArquivo();

int codigoPrincipal();

#endif // CLIENTE_H
