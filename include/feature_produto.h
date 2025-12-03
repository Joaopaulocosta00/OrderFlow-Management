#ifndef FEATURE_PRODUTO_H
#define FEATURE_PRODUTO_H

// Definicao da struct de   Produto
typedef struct {
    int id;
    char descricao[100];
    double preco;
    int estoque;
} Produto;

//..................................................................

// Funções de Persistência
void carregarProdutosCSV();
void salvarProdutosCSV();

//...................................................................

// Funcoes de interface 

void cadastrarProduto();
void consultarProduto();
void removerProdutoInterface();
void listarProdutos();


//int buscarProdutoPorId(int id); (modulo - pedido - kauan)

#endif
