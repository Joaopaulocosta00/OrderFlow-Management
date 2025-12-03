#include <ncurses.h>
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h>
#include <time.h>   

// ============================================================
// DEFINIÇÃO DOS ARQUIVOS (AGORA EM FORMATO .CSV)
// ============================================================
#define ARQ_CLIENTES  "clientes.csv"
#define ARQ_PRODUTOS  "produtos.csv"
#define ARQ_PEDIDOS   "pedidos.csv"
#define ARQ_ITENS     "itenspedido.csv"
#define ARQ_PRODUTOS_TXT "produtos.txt"       // Arquivo de importação original
#define ARQ_PEDIDO_TXT_SAIDA "pedido.txt"     // Recibo em texto

// ========================= ESTRUTURAS =========================
typedef struct {
    int id;
    char nome[100];
    char endereco[200];
    char telefone[20];
} Cliente;

typedef struct {
    int id;
    char descricao[100];
    double preco;
    int estoque;
} Produto;

typedef struct {
    int id;
    int clienteId;
    char data[11];
    double total;
} Pedido;

typedef struct {
    int pedidoId;
    int produtoId;
    int quantidade;
    double subtotal;
} ItemPedido;

// ============================================================
// PROTÓTIPOS
// ============================================================
void cadastrar_pedido(WINDOW *w);
void listar_pedidos(WINDOW *w);
void consultar_pedido(WINDOW *w);
void remover_pedido(WINDOW *w);
int produto_existe_txt(int id, Produto *out); // Mantido para leitura do TXT original

// =============================================================
// UTILITÁRIOS NCURSES
// =============================================================

void msg(WINDOW *w, const char *m) {
    int h, l;
    getmaxyx(w, h, l);
    // Limpa a linha de mensagem
    mvwprintw(w, h-2, 2, "                                                                  ");
    // Exibe mensagem
    wattron(w, A_BOLD);
    mvwprintw(w, h-2, 2, ">> %s", m);
    wattroff(w, A_BOLD);
    wrefresh(w);
}

void pausa(WINDOW *w) {
    msg(w, "Pressione qualquer tecla para continuar...");
    flushinp(); 
    wgetch(w);
    // Limpa a mensagem após a pausa
    int h, l;
    getmaxyx(w, h, l);
    mvwprintw(w, h-2, 2, "                                                                  ");
    wrefresh(w);
}

void data_atual(char buf[11]) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buf, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
}

int gerar_id_aleatorio() {
    srand(time(NULL));
    return (rand() % 9000) + 1000; 
}

// =============================================================
// LÓGICA DE ARQUIVOS CSV (LEITURA/ESCRITA)
// =============================================================

// Verifica se produto existe no "banco de dados" CSV interno
int produto_existe_csv(int id, Produto *out) {
    FILE *f = fopen(ARQ_PRODUTOS, "r"); 
    if (!f) return 0;

    Produto p;
    // Lê formato: ID;Descricao;Preco;Estoque
    while (fscanf(f, "%d;%[^;];%lf;%d\n", &p.id, p.descricao, &p.preco, &p.estoque) == 4) {
        if (p.id == id) {
            if (out) *out = p;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int pedido_existe_csv(int id, Pedido *out) {
    FILE *f = fopen(ARQ_PEDIDOS, "r");
    if (!f) return 0;

    Pedido p;
    // Lê formato: ID;ClienteID;Data;Total
    while (fscanf(f, "%d;%d;%[^;];%lf\n", &p.id, &p.clienteId, p.data, &p.total) == 4) {
        if (p.id == id) {
            if (out) *out = p;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

// Atualiza estoque reescrevendo o arquivo CSV
int atualizar_estoque_csv(int prodId, int novoEstoque) {
    FILE *f = fopen(ARQ_PRODUTOS, "r"); 
    if (!f) return 0;
    
    FILE *tmp = fopen("prod_tmp.csv", "w");
    if (!tmp) { fclose(f); return 0; }

    Produto p;
    int encontrou = 0;

    while (fscanf(f, "%d;%[^;];%lf;%d\n", &p.id, p.descricao, &p.preco, &p.estoque) == 4) {
        if (p.id == prodId) {
            p.estoque = novoEstoque;
            encontrou = 1;
        }
        // Reescreve no arquivo temporário
        fprintf(tmp, "%d;%s;%.2f;%d\n", p.id, p.descricao, p.preco, p.estoque);
    }

    fclose(f);
    fclose(tmp);

    remove(ARQ_PRODUTOS);
    rename("prod_tmp.csv", ARQ_PRODUTOS);
    return encontrou;
}

int incrementar_estoque(int prodId, int qtd) {
    Produto p;
    if (!produto_existe_csv(prodId, &p)) return 0;
    return atualizar_estoque_csv(prodId, p.estoque + qtd);
}

// Lógica do parser do arquivo TXT sujo (Mantida do seu código original)
int produto_existe_txt(int id, Produto *out) {
    const char *candidates[] = { ARQ_PRODUTOS_TXT, "produto.txt", NULL };
    char linha[512];
    char idStr[32];
    sprintf(idStr, "%d", id);

    FILE *f = NULL;
    for (int i = 0; candidates[i] != NULL; ++i) {
        f = fopen(candidates[i], "r");
        if (f) break;
    }
    if (!f) return 0;

    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\r\n")] = 0;
        char *start = linha;
        while (*start == ' ' || *start == '\t') start++;
        
        // Pula linhas separadoras
        int all_sep = 1;
        for (char *q = start; *q; ++q) {
            if (!(*q == '-' || *q == '_' || *q == '=' || *q == ' ' || *q == '\t')) { all_sep = 0; break; }
        }
        if (start[0] == '\0' || all_sep) continue;

        // Procura ID
        if (strstr(start, idStr) != NULL) {
            char desc[256] = {0}, precoLine[128] = {0}, estoqueLine[64] = {0};
            
            // Pega Descrição
            while (fgets(linha, sizeof(linha), f)) {
                char *s = linha; while(*s==' '||*s=='\t') s++;
                if(*s=='\0' || *s=='-') continue;
                strncpy(desc, s, 255); desc[strcspn(desc, "\r\n")] = 0; break;
            }
            // Pega Preço
            while (fgets(linha, sizeof(linha), f)) {
                char *s = linha; while(*s==' '||*s=='\t') s++;
                if(*s=='\0' || *s=='-') continue;
                strncpy(precoLine, s, 127); break;
            }
            // Pega Estoque
            while (fgets(linha, sizeof(linha), f)) {
                char *s = linha; while(*s==' '||*s=='\t') s++;
                if(*s=='\0' || *s=='-') continue;
                strncpy(estoqueLine, s, 63); break;
            }

            out->id = id;
            strncpy(out->descricao, desc, 99);
            out->preco = atof(precoLine);
            out->estoque = atoi(estoqueLine);
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}


// =============================================================
// FUNÇÕES VISUAIS (NCURSES)
// =============================================================

void cadastrar_pedido(WINDOW *w) {
    char entrada[20];
    Produto p; 
    int idProd;

    werase(w); box(w, 0, 0);
    mvwprintw(w, 1, 2, "=== CADASTRAR PEDIDO (Ler TXT -> Salvar CSV) ===");
    mvwprintw(w, 3, 2, "Digite o ID do produto (do arquivo txt): ");
    
    echo();
    mvwgetnstr(w, 3, 44, entrada, 10);
    noecho();

    idProd = atoi(entrada);
    if (idProd == 0) {
        msg(w, "Operacao cancelada.");
        pausa(w);
        return;
    }

    // 1. Busca no arquivo TXT de importação
    if (!produto_existe_txt(idProd, &p)) {
        msg(w, "Produto nao encontrado no arquivo TXT.");
        pausa(w);
        return; 
    }
    
    // Exibe dados na tela
    mvwprintw(w, 5, 2, "Produto Encontrado:");
    mvwprintw(w, 6, 4, "ID: %d", p.id);
    mvwprintw(w, 7, 4, "Desc: %s", p.descricao);
    mvwprintw(w, 8, 4, "Preco: R$ %.2f", p.preco);
    mvwprintw(w, 9, 4, "Estoque Disp: %d", p.estoque);
    
    mvwprintw(w, 11, 2, "Confirmar pedido? (S/N): ");
    echo(); mvwgetnstr(w, 11, 28, entrada, 1); noecho();
    
    if (entrada[0] != 's' && entrada[0] != 'S') return;
    
    // -- Lógica de salvar em CSV --
    int idPedido = gerar_id_aleatorio();
    char data[11];
    data_atual(data);
    double total = p.preco * p.estoque; // Exemplo: compra todo o estoque

    // Salva Cabeçalho do Pedido no CSV
    FILE *fp = fopen(ARQ_PEDIDOS, "a");
    if (fp) {
        // ID;ClienteID;Data;Total
        fprintf(fp, "%d;%d;%s;%.2f\n", idPedido, 1, data, total);
        fclose(fp);
    }

    // Salva Item do Pedido no CSV
    FILE *fi = fopen(ARQ_ITENS, "a");
    if (fi) {
        // PedidoID;ProdutoID;Qtd;Subtotal
        fprintf(fi, "%d;%d;%d;%.2f\n", idPedido, p.id, p.estoque, total);
        fclose(fi);
    }

    // Atualiza ou cria produto no "banco" CSV interno se não existir
    Produto p_check;
    if (!produto_existe_csv(p.id, &p_check)) {
        FILE *fprod = fopen(ARQ_PRODUTOS, "a");
        if(fprod) {
            fprintf(fprod, "%d;%s;%.2f;%d\n", p.id, p.descricao, p.preco, 0); // Estoque zerou
            fclose(fprod);
        }
    } else {
        atualizar_estoque_csv(p.id, 0); // Zera o estoque
    }

    msg(w, "Pedido salvo em CSV com sucesso!");
    pausa(w);
}

void listar_pedidos(WINDOW *w) {
    FILE *f = fopen(ARQ_PEDIDOS, "r");
    werase(w); box(w, 0, 0);
    mvwprintw(w, 1, 2, "=== LISTA DE PEDIDOS (Lendo CSV) ===");

    if (!f) {
        mvwprintw(w, 3, 2, "Nenhum pedido encontrado (arquivo vazio ou inexistente).");
        wrefresh(w); pausa(w); return;
    }

    Pedido p;
    int linha = 3;
    
    mvwprintw(w, linha++, 2, "ID      | DATA       | TOTAL");
    mvwprintw(w, linha++, 2, "--------------------------------");

    // Leitura formatada do CSV
    while (fscanf(f, "%d;%d;%[^;];%lf\n", &p.id, &p.clienteId, p.data, &p.total) == 4) {
        mvwprintw(w, linha, 2, "%-7d | %-10s | R$ %.2f", p.id, p.data, p.total);
        linha++;
        
        if (linha >= 20) {
            pausa(w);
            werase(w); box(w, 0, 0);
            mvwprintw(w, 1, 2, "=== LISTA DE PEDIDOS (Continua) ===");
            linha = 3;
        }
    }
    fclose(f);
    wrefresh(w);
    pausa(w);
}

void consultar_pedido(WINDOW *w) {
    char entrada[20];
    int idBusca;

    werase(w); box(w, 0, 0);
    mvwprintw(w, 1, 2, "=== CONSULTAR PEDIDO (CSV) ===");
    mvwprintw(w, 3, 2, "Digite o ID do pedido: ");
    echo(); mvwgetnstr(w, 3, 26, entrada, 10); noecho();
    idBusca = atoi(entrada);

    Pedido p;
    if (!pedido_existe_csv(idBusca, &p)) {
        msg(w, "Pedido nao encontrado.");
        pausa(w); return;
    }

    werase(w); box(w, 0, 0);
    mvwprintw(w, 1, 2, "DETALHES DO PEDIDO %d", p.id);
    mvwprintw(w, 3, 2, "Data: %s", p.data);
    mvwprintw(w, 4, 2, "Total: R$ %.2f", p.total);
    
    mvwprintw(w, 6, 2, "--- Itens do Pedido ---");
    int linha = 7;

    FILE *f = fopen(ARQ_ITENS, "r");
    if (f) {
        ItemPedido it;
        while (fscanf(f, "%d;%d;%d;%lf\n", &it.pedidoId, &it.produtoId, &it.quantidade, &it.subtotal) == 4) {
            if (it.pedidoId == idBusca) {
                mvwprintw(w, linha++, 4, "Prod ID: %d | Qtd: %d | Sub: %.2f", 
                          it.produtoId, it.quantidade, it.subtotal);
            }
        }
        fclose(f);
    }

    wrefresh(w);
    pausa(w);
}

void remover_pedido(WINDOW *w) {
    char entrada[20];
    int idRemover;

    werase(w); box(w, 0, 0);
    mvwprintw(w, 1, 2, "=== REMOVER PEDIDO (CSV) ===");
    mvwprintw(w, 3, 2, "ID a remover: ");
    echo(); mvwgetnstr(w, 3, 16, entrada, 10); noecho();
    idRemover = atoi(entrada);

    Pedido p;
    if (!pedido_existe_csv(idRemover, &p)) {
        msg(w, "Pedido nao existe."); pausa(w); return;
    }

    // 1. Estornar Estoque (Lendo Itens CSV)
    FILE *fit = fopen(ARQ_ITENS, "r");
    FILE *tmpItens = fopen("itens_tmp.csv", "w");
    ItemPedido it;

    if (fit && tmpItens) {
        while (fscanf(fit, "%d;%d;%d;%lf\n", &it.pedidoId, &it.produtoId, &it.quantidade, &it.subtotal) == 4) {
            if (it.pedidoId == idRemover) {
                // Estorna estoque no CSV de produtos
                incrementar_estoque(it.produtoId, it.quantidade);
            } else {
                // Mantém item no arquivo
                fprintf(tmpItens, "%d;%d;%d;%.2f\n", it.pedidoId, it.produtoId, it.quantidade, it.subtotal);
            }
        }
        fclose(fit);
        fclose(tmpItens);
        remove(ARQ_ITENS);
        rename("itens_tmp.csv", ARQ_ITENS);
    }

    // 2. Remover Cabeçalho do Pedido
    FILE *fp = fopen(ARQ_PEDIDOS, "r");
    FILE *tmpPed = fopen("ped_tmp.csv", "w");

    if (fp && tmpPed) {
        while (fscanf(fp, "%d;%d;%[^;];%lf\n", &p.id, &p.clienteId, p.data, &p.total) == 4) {
            if (p.id != idRemover) {
                fprintf(tmpPed, "%d;%d;%s;%.2f\n", p.id, p.clienteId, p.data, p.total);
            }
        }
        fclose(fp);
        fclose(tmpPed);
        remove(ARQ_PEDIDOS);
        rename("ped_tmp.csv", ARQ_PEDIDOS);
    }

    msg(w, "Pedido removido e estoque estornado (Arquivos CSV atualizados).");
    pausa(w);
}

// =============================================================
// MENU PRINCIPAL
// =============================================================

void menu_pedidos(WINDOW *w) {
    int op;
    do {
        werase(w); box(w, 0, 0);
        mvwprintw(w, 1, 2, "SISTEMA DE PEDIDOS - VERSAO CSV");
        mvwprintw(w, 2, 2, "===============================");
        mvwprintw(w, 4, 4, "1. Cadastrar Pedido (Ler TXT)");
        mvwprintw(w, 5, 4, "2. Listar Pedidos");
        mvwprintw(w, 6, 4, "3. Consultar Detalhes");
        mvwprintw(w, 7, 4, "4. Remover Pedido");
        mvwprintw(w, 9, 4, "5. Sair");
        
        mvwprintw(w, 11,2, "Escolha: ");
        wrefresh(w);

        echo();
        wscanw(w, "%d", &op);
        noecho();

        switch(op) {
            case 1: cadastrar_pedido(w); break;
            case 2: listar_pedidos(w); break;
            case 3: consultar_pedido(w); break;
            case 4: remover_pedido(w); break;
            case 5: break;
            default: msg(w, "Opcao invalida!"); pausa(w); break;
        }
    } while (op != 5);
}

int main() {
    // Inicialização do NCURSES
    initscr();
    cbreak();
    noecho();
    curs_set(1); // Mostra cursor

    // Cria uma janela centralizada
    int h = 24, w = 80;
    int starty = (LINES - h) / 2; 
    int startx = (COLS - w) / 2;
    
    WINDOW *win = newwin(h, w, starty, startx);
    keypad(win, TRUE);

    menu_pedidos(win);

    delwin(win);
    endwin(); // Finaliza NCURSES

    return 0;
}