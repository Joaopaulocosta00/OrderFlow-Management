#include<stdio.h>
struct Produto {
    char nome[50];
    float preco;
    int quantidade;
    int codigo;
};
void main() {
    struct Produto p;
    //Cadastro do produto
    printf("Digite o nome do produto: ");
    scanf("%49s", p.nome);

    printf("Digite o preco do produto: ");
    scanf("%f", &p.preco);

    printf("Digite a quantidade do produto: ");
    scanf("%d", &p.quantidade);

    printf("Digite o codigo do produto: ");
    scanf("%d", &p.codigo);

    printf("\nProduto cadastrado:\n");
    printf("Nome: %s\n", p.nome);
    printf("Preco: %.2f\n", p.preco);
    printf("Quantidade: %d\n", p.quantidade);

    //Remover produto
    p.quantidade = 0;
    printf("Produto removido. Quantidade atual: %d\n", p.quantidade);

    //consultar produto
    printf("\nConsulta do produto:\n");
    printf("Nome: %s\n", p.nome);
    printf("Preco: %.2f\n", p.preco);
    printf("Quantidade: %d\n", p.quantidade);
    printf("Codigo: %d\n", p.codigo);  

    //Listagem dos produtros cadastrados
    printf("\nListagem de produtos cadastrados:\n");
    printf("Nome: %s | Preco: %.2f | Quantidade: %d | Codigo: %d\n", p.nome, p.preco, p.quantidade, p.codigo); 
}     
//Codigo do produto exeste ou nao
   int codigoConsulta;{
    printf("\nDigite o codigo do produto para consulta: ");
    scanf("%d", &codigoConsulta);
    if(codigoConsulta == p.codigo) {
        printf("Produto encontrado:\n");
        printf("Nome: %s\n", p.nome);
        printf("Preco: %.2f\n", p.preco);
        printf("Quantidade: %d\n", p.quantidade);
        printf("Codigo: %d\n", p.codigo);
    } else {
        printf("Produto nao encontrado com o codigo %d\n", codigoConsulta);
    }
}https://github.com/Joaopaulocosta00/OrderFlow-Management.git