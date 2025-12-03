#include "feature_interface.h" // ncurses e ferramentas visuais
#include "feature_produto.h"
// #include "feature_pedido.h" // (modulo pedido.c - kauan )

#define ARQUIVO_PRODUTOS "data/Produtos.csv"
#define MAX_PRODUTOS 100

Produto listaProdutos[MAX_PRODUTOS];
int numProdutos = 0;

/
// funcoesdo produto 
// ==============================================================================

// aqui vai rpetorna o indice do produto no vetor ou -1 se não achar
int buscarProdutoPorId(int id) {
    for (int i = 0; i < numProdutos; i++) {
        if (listaProdutos[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Simulação da verificação de pedidos (Requisito do cenário de Remover)
// Como o módulo de pedidos ainda não está pronto, deixamos essa função preparada.
int produtoEstaEmAlgumPedido(int id) {
    // TODO: Quando criar o modulo de pedidos, implementar a busca real aqui.
    // Por enquanto, retorna 0 (falso), permitindo excluir.
    // Exemplo de lógica futura: if (buscarPedidoComProduto(id)) return 1;
    return 0; 
}

// ==============================================================================
// PERSISTÊNCIA (CSV)
// ==============================================================================

void carregarProdutosCSV() {
    FILE *f = fopen(ARQUIVO_PRODUTOS, "r");
    if (!f) return; // Se não existe, inicia vazio

    char linha[256];
    numProdutos = 0;
    while(fgets(linha, sizeof(linha), f) && numProdutos < MAX_PRODUTOS) {
        // Parse CSV: id;descricao;preco;estoque
        char *token = strtok(linha, ";");
        listaProdutos[numProdutos].id = atoi(token);
        
        token = strtok(NULL, ";");
        if(token) strcpy(listaProdutos[numProdutos].descricao, token);
        
        token = strtok(NULL, ";");
        if(token) listaProdutos[numProdutos].preco = atof(token);
        
        token = strtok(NULL, ";");
        if(token) listaProdutos[numProdutos].estoque = atoi(token);
        
        numProdutos++;
    }
    fclose(f);
}

void salvarProdutosCSV() {
    FILE *f = fopen(ARQUIVO_PRODUTOS, "w");
    if (!f) return;

    for(int i=0; i<numProdutos; i++) {
        fprintf(f, "%d;%s;%.2f;%d\n", 
            listaProdutos[i].id, 
            listaProdutos[i].descricao, 
            listaProdutos[i].preco, 
            listaProdutos[i].estoque);
    }
    fclose(f);
}



// cadastrar produto
void cadastrarProduto() {
    Produto p;
    int idValido = 0;

    mostrarCabecalho("CADASTRAR PRODUTO");

    if (numProdutos >= MAX_PRODUTOS) { //vasi verifica se a lista ja atingiu o limite maximo (100)
        printw("Erro: Memoria cheia.\n");
        pausa();
        return; // aborta a funcoaa por estar cheio
    }

    echo(); // habilita escrita

   
    do {
        printw("\nCodigo do Produto: ");
        scanw("%d", &p.id);

        if (buscarProdutoPorId(p.id) != -1) { // verifica se ja existe o codigo 
            //  Mensagem de erro e retorna para tela oara o usuario tentar outro codigo de produto 
            printw(" > Erro: Codigo %d ja existe. Tente outro.\n", p.id);
        } else {
            idValido = 1; ?// aqui vai criar um loop ate o usuario digitar o codigo valido pro sistema
        }
    } while (!idValido);

    // Informa descrição, preço e estoque 

    printw("Descricao: ");
    getnstr(p.descricao, 99); // getnstr parab o usuaruio escrever texto com espaço

    printw("Preco: "); 
    scanw("%lf", &p.preco);

    printw("Quantidade em Estoque: ");
    scanw("%d", &p.estoque);

    noecho(); // vai desabilitar o usuario digitar algo

    // vai confirmar o cadastro  
    printw("\ncadastrar este produto? (s/n): ");
    int confirm = getch();
    
    if (toupper(confirm) == 'S') {
        listaProdutos[numProdutos] = p; // (nao esquecer )a variavel p contem os dados que o usuario acabou de digitar
        numProdutos++;
        printw("\n\nProduto cadastrado com sucesso");
    } else {
        printw("\n\nOperacao de cadastro cancelada.");
    }
    pausa();
}



//----------------------------------------------------------------------------------------------------------

// Consultar Produto 


void consultarProduto() {
    int id, index;
    
    mostrarCabecalho("CONSULTAR PRODUTO");
    
    // Informa código 
    echo();
    printw("Digite o codigo do produto: ");
    scanw("%d", &id);
    noecho();



    //Analisa produto 
    index = buscarProdutoPorId(id);

    if (index != -1) {
        // Caso exista ele vai mostra os dados 
        printw("\n--- DADOS DO PRODUTO ---\n");
        printw("ID:        %d\n", listaProdutos[index].id);
        printw("Descricao: %s\n", listaProdutos[index].descricao);
        printw("Preco:     R$ %.2f\n", listaProdutos[index].preco);
        printw("Estoque:   %d unidades\n", listaProdutos[index].estoque);
    } else {
        // Caso não exista da a sauda de erro 
        printw("\nErro: Produto nao existe.\n");
    }
    pausa();
}

// funçao de remover o produto


void removerProdutoInterface() {
    int id, index;

    mostrarCabecalho("REMOVER PRODUTO");

    // pede o codigo do produto
    echo();
    printw("Digite o codigo do produto que vai  remover");
    scanw("%d", &id);
    noecho();

   
    index = buscarProdutoPorId(id); // vai pedir a funçao para verificar se o codigo existe ou nao

    if (index == -1) {
        // Caso nao exista retorna erro 
        printw("\nErro: Produto nao existe.\n");
    } else {
        //  verifica se existe pedido com esse item
        if (produtoEstaEmAlgumPedido(id)) {
            // Caso exista pedido, informa que não pode excluir [cite: 151]
            printw("\nErro: Produto nao pode ser excluido pois consta em pedidos de clientes.\n");
        } else {
            // Caso não exista pedidos, pede confirmação [cite: 152]
            printw("\nProduto '%s' encontrado.", listaProdutos[index].descricao);
            printw("\nTem certeza que deseja remover? (S/N): ");
            int conf = getch();

            if (toupper(conf) == 'S') {
                // Confirmou: remove do cadastro [cite: 153]
                for (int i = index; i < numProdutos - 1; i++) {
                    listaProdutos[i] = listaProdutos[i+1];
                }
                numProdutos--;
                printw("\nProduto removido com sucesso.\n");
            } else {
                // Não confirmou: produto mantido [cite: 153]
                printw("\nOperacao cancelada. Produto mantido.\n");
            }
        }
    }
    pausa();
}

// Cenario: Listar Produto [cite: 158]
void listarProdutos() {
    mostrarCabecalho("LISTA DE PRODUTOS");

    // Usuário solicita listagem [cite: 159]
    // (A solicitação ocorreu ao selecionar a opção no menu)

    if (numProdutos == 0) {
        printw("Nenhum produto cadastrado.\n");
    } else {
        // Mostra listagem com todos os dados
        printw("%-6s %-30s %-12s %-8s\n", "ID", "DESCRICAO", "PRECO", "QTD");
        printw("------------------------------------------------------------\n");
        
        for(int i=0; i<numProdutos; i++) {
            printw("%-6d %-30s R$ %-9.2f %-8d\n", 
                listaProdutos[i].id, 
                listaProdutos[i].descricao, 
                listaProdutos[i].preco, 
                listaProdutos[i].estoque);
        }
    }
    pausa();
}