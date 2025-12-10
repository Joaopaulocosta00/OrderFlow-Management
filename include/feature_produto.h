#ifndef FEATURE_PRODUTO_H
#define FEATURE_PRODUTO_H

// Estrutura do Produto
typedef struct {
    int id;
    char descricao[100];
    double preco;
    int estoque;
} Produto;

// Funcoes de arquivo
void carregarProdutosCSV();
void salvarProdutosCSV();

// Funcoes que a interface precisa v o prototipos
void cadastrarProduto();
void consultarProduto();
void removerProdutoInterface();
void listarProdutos();

// Auxiliares
int buscarProdutoPorId(int id);

#endif
