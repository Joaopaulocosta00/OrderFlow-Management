#include<stdio.h>
struct Cliente {
    char nome[50];
    char endereco[100];
    char telefone[15];
    int codigo;
    int cpf;    
    int cnpj;
};
void main() {
    struct Cliente c;
    //Cadastro do cliente
    printf("Digite o nome do cliente: ");
    scanf("%49s", c.nome);

    printf("Digite o endereco do cliente: ");
    scanf("%99s", c.endereco);

    printf("Digite o telefone do cliente: ");
    scanf("%14s", c.telefone);
    printf("Pessoa fisica ou juridica? (F/J):" ); 
    if (==CPF||cpf){
        printf("Digite o codigo do cliente: ");
    scanf("%d", &c.codigo);

    printf("Digite o CPF do cliente: ");
    scanf("%d", &c.cpf);

    printf("Digite o CNPJ do cliente: ");
    scanf("%d", &c.cnpj);

    printf("\nCliente cadastrado:\n");
    printf("Nome: %s\n", c.nome);
    printf("Endereco: %s\n", c.endereco);
    printf("Telefone: %s\n", c.telefone);
    printf("Codigo: %d\n", c.codigo);
    printf("CPF: %d\n", c.cpf);
    printf("CNPJ: %d\n", c.cnpj);

    //Remover cliente
    c.codigo = 0;
    printf("Cliente removido. Codigo atual: %d\n", c.codigo);

    //consultar cliente
    printf("\nConsulta do cliente:\n");
    printf("Nome: %s\n", c.nome);
    printf("Endereco: %s\n", c.endereco);
    printf("Telefone: %s\n", c.telefone);
    printf("Codigo: %d\n", c.codigo);
    printf("CPF: %d\n", c.cpf);
    printf("CNPJ: %d\n", c.cnpj);

    //Listagem dos clientes cadastrados
    printf("\nListagem de clientes cadastrados:\n");
    printf("Nome: %s | Endereco: %s | Telefone: %s | Codigo: %d | CPF: %d | CNPJ: %d\n", c.nome, c.endereco, c.telefone, c.codigo, c.cpf, c.cnpj); 
}