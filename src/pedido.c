// ============================================================
// MÓDULO DE PEDIDOS - SISTEMA DE CONTROLE
// SOMENTE A PARTE DE PEDIDOS (Clientes e Produtos apenas validados)
// Compilar: gcc modulo_pedidos.c -lncurses -o pedidos
// ============================================================

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Arquivos usados pelo módulo (somente leitura de clientes e produtos)
#define ARQ_CLIENTES  "clientes.dat"
#define ARQ_PRODUTOS  "produtos.dat"

#define ARQ_PEDIDOS   "pedidos.dat"
#define ARQ_ITENS     "itenspedido.dat"

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


// =============================================================
// UTILITÁRIOS
// =============================================================

// Mostra mensagem no rodapé
void msg(WINDOW *w, const char *m) {
    int h; int l;
    getmaxyx(w, h, l);
    mvwprintw(w, h-2, 2, "%s", m);
    wrefresh(w);
}

void pausa(WINDOW *w) {
    msg(w, "Pressione qualquer tecla...");
    wgetch(w);
}

// Pega data atual
void data_atual(char buf[11]) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buf, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
}


// =============================================================
// FUNÇÕES DE VALIDAÇÃO
// =============================================================

int cliente_existe(int id) {
    FILE *f = fopen(ARQ_CLIENTES, "rb");
    if (!f) return 0;

    Cliente c;
    while (fread(&c, sizeof(c), 1, f)) {
        if (c.id == id) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int produto_existe(int id, Produto *out) {
    FILE *f = fopen(ARQ_PRODUTOS, "rb");
    if (!f) return 0;

    Produto p;
    while (fread(&p, sizeof(p), 1, f)) {
        if (p.id == id) {
            if (out) *out = p;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int pedido_existe(int id, Pedido *out) {
    FILE *f = fopen(ARQ_PEDIDOS, "rb");
    if (!f) return 0;

    Pedido p;
    while (fread(&p, sizeof(p), 1, f)) {
        if (p.id == id) {
            if (out) *out = p;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}


// =============================================================
// ATUALIZAÇÃO DE ESTOQUE
// =============================================================

// Função interna usada pelo módulo de pedidos
int atualizar_estoque(int prodId, int novoEstoque) {
    FILE *f = fopen(ARQ_PRODUTOS, "rb");
    if (!f) return 0;

    FILE *tmp = fopen("prod_tmp.dat", "wb");
    Produto p;
    int ok = 0;

    while (fread(&p, sizeof(p), 1, f)) {
        if (p.id == prodId) {
            p.estoque = novoEstoque;
            ok = 1;
        }
        fwrite(&p, sizeof(p), 1, tmp);
    }

    fclose(f);
    fclose(tmp);

    remove(ARQ_PRODUTOS);
    rename("prod_tmp.dat", ARQ_PRODUTOS);
    return ok;
}

int decrementar_estoque(int prodId, int qtd) {
    Produto p;
    if (!produto_existe(prodId, &p)) return 0;
    if (p.estoque < qtd) return 0;
    return atualizar_estoque(prodId, p.estoque - qtd);
}

int incrementar_estoque(int prodId, int qtd) {
    Produto p;
    if (!produto_existe(prodId, &p)) return 0;
    return atualizar_estoque(prodId, p.estoque + qtd);
}


// =============================================================
// CADASTRAR PEDIDO
// =============================================================

void cadastrar_pedido(WINDOW *w) {
    char entrada[20];
    Pedido ped;
    double total = 0;

    // 1 — Ler ID do pedido e validar unicidade
    while (1) {
        werase(w); box(w, 0, 0);
        mvwprintw(w, 2, 2, "=== CADASTRAR PEDIDO ===");
        mvwprintw(w, 4, 2, "ID do pedido: ");
        echo();
        mvwgetnstr(w, 4, 18, entrada, 10);
        noecho();

        ped.id = atoi(entrada);
        if (ped.id <= 0) { msg(w, "ID inválido."); pausa(w); continue; }
        if (pedido_existe(ped.id, NULL)) { msg(w, "Pedido já existe."); pausa(w); continue; }
        break;
    }

    // 2 — Ler cliente
    while (1) {
        mvwprintw(w, 6, 2, "ID do cliente:          ");
        echo();
        mvwgetnstr(w, 6, 18, entrada, 10);
        noecho();
        ped.clienteId = atoi(entrada);

        if (!cliente_existe(ped.clienteId)) { msg(w, "Cliente não existe."); pausa(w); continue; }
        break;
    }

    // 3 — Inserir itens
    FILE *fit = fopen(ARQ_ITENS, "ab");
    while (1) {
        mvwprintw(w, 8, 2, "ID produto (0 finalizar):      ");
        echo();
        mvwgetnstr(w, 8, 26, entrada, 10);
        noecho();

        int idProd = atoi(entrada);
        if (idProd == 0) break;

        Produto p;
        if (!produto_existe(idProd, &p)) {
            msg(w, "Produto não existe.");
            pausa(w);
            continue;
        }

        mvwprintw(w, 10, 2, "Qtd: ");
        echo();
        mvwgetnstr(w, 10, 8, entrada, 10);
        noecho();

        int qtd = atoi(entrada);
        if (qtd <= 0) { msg(w, "Qtd inválida."); pausa(w); continue; }

        if (p.estoque < qtd) {
            msg(w, "Estoque insuficiente.");
            pausa(w);
            continue;
        }
    }
    fclose(it);
    fclose(tmp2);

        // gravar item
        ItemPedido it;
        it.pedidoId = ped.id;
        it.produtoId = idProd;
        it.quantidade = qtd;
        it.subtotal = qtd * p.preco;

        fwrite(&it, sizeof(it), 1, fit);

        // baixa estoque
        decrementar_estoque(idProd, qtd);

        total += it.subtotal;
        msg(w, "Item adicionado!");
        pausa(w);
    }
    fclose(fit);

    // 4 — Registrar o pedido
    data_atual(ped.data);
    ped.total = total;

    FILE *fp = fopen(ARQ_PEDIDOS, "ab");
    fwrite(&ped, sizeof(ped), 1, fp);
    fclose(fp);

    werase(w); box(w, 0, 0);
    mvwprintw(w, 4, 2, "Pedido cadastrado!");
    mvwprintw(w, 6, 2, "Total: R$ %.2f", total);
    wrefresh(w);
    pausa(w);
}


// =============================================================
// LISTAR PEDIDOS
// =============================================================

void listar_pedidos(WINDOW *w) {
    FILE *f = fopen(ARQ_PEDIDOS, "rb");
    werase(w); box(w, 0, 0);
    mvwprintw(w, 2, 2, "=== LISTA DE PEDIDOS ===");

    if (!f) {
        mvwprintw(w, 4, 2, "Nenhum pedido encontrado.");
        wrefresh(w);
        pausa(w);
        return;
    }

    Pedido p;
    int linha = 4;
    while (fread(&p, sizeof(p), 1, f)) {
        mvwprintw(w, linha, 2, "ID:%d  Cliente:%d  Data:%s  Total: R$%.2f",
                  p.id, p.clienteId, p.data, p.total);
        linha++;

        if (linha >= 20) {
            pausa(w);
            werase(w); box(w, 0, 0);
            linha = 4;
        }
    }
    fclose(f);

    wrefresh(w);
    pausa(w);
}


// =============================================================
// CONSULTAR PEDIDO
// =============================================================

void listar_itens(WINDOW *w, int idPed) {
    FILE *f = fopen(ARQ_ITENS, "rb");
    if (!f) return;

    ItemPedido it;
    int linha = 8;

    mvwprintw(w, 6, 2, "Itens:");

    while (fread(&it, sizeof(it), 1, f)) {
        if (it.pedidoId == idPed) {
            Produto p;
            produto_existe(it.produtoId, &p);

            mvwprintw(w, linha, 2,
                "%s | Qt:%d | Sub: R$%.2f",
                p.descricao, it.quantidade, it.subtotal);

            linha++;
        }
    }

    fclose(f);
}

void consultar_pedido(WINDOW *w) {
    char entrada[20];
    int id;

    werase(w); box(w, 0, 0);
    mvwprintw(w, 2, 2, "=== CONSULTAR PEDIDO ===");
    mvwprintw(w, 4, 2, "ID: ");
    echo();
    mvwgetnstr(w, 4, 7, entrada, 10);
    noecho();
    id = atoi(entrada);

    Pedido p;
    if (!pedido_existe(id, &p)) {
        msg(w, "Pedido não encontrado.");
        pausa(w);
        return;
    }

    werase(w); box(w, 0, 0);
    mvwprintw(w, 2, 2, "Pedido %d", p.id);
    mvwprintw(w, 3, 2, "Cliente: %d", p.clienteId);
    mvwprintw(w, 4, 2, "Data: %s", p.data);
    mvwprintw(w, 5, 2, "Total: R$ %.2f", p.total);

    listar_itens(w, id);
    wrefresh(w);
    pausa(w);
}


// =============================================================
// REMOVER PEDIDO
// =============================================================

void remover_pedido(WINDOW *w) {
    char entrada[20];
    int id;

    werase(w); box(w, 0, 0);
    mvwprintw(w, 2, 2, "=== REMOVER PEDIDO ===");
    mvwprintw(w, 4, 2, "ID: ");
    echo();
    mvwgetnstr(w, 4, 7, entrada, 10);
    noecho();
    id = atoi(entrada);

    Pedido p;
    if (!pedido_existe(id, &p)) {
        msg(w, "Pedido não existe.");
        pausa(w);
        return;
    }

    // Restaura estoque
    FILE *fit = fopen(ARQ_ITENS, "rb");
    FILE *tmpItens = fopen("itens_tmp.dat", "wb");
    ItemPedido it;

    while (fread(&it, sizeof(it), 1, fit)) {
        if (it.pedidoId == id) {
            incrementar_estoque(it.produtoId, it.quantidade);
        } else {
            fwrite(&it, sizeof(it), 1, tmpItens);
        }
    }

    fclose(fit);
    fclose(tmpItens);
    remove(ARQ_ITENS);
    rename("itens_tmp.dat", ARQ_ITENS);

    // Remove pedido
    FILE *fp = fopen(ARQ_PEDIDOS, "rb");
    FILE *tmpPed = fopen("ped_tmp.dat", "wb");

    while (fread(&p, sizeof(p), 1, fp)) {
        if (p.id != id)
            fwrite(&p, sizeof(p), 1, tmpPed);
    }

    fclose(fp);
    fclose(tmpPed);
    remove(ARQ_PEDIDOS);
    rename("ped_tmp.dat", ARQ_PEDIDOS);

    msg(w, "Pedido removido com sucesso.");
    pausa(w);
}


// =============================================================
// MENU
// =============================================================

void menu_pedidos(WINDOW *w) {
    int op;

    do {
        werase(w); box(w, 0, 0);
        mvwprintw(w, 2, 2, "=== MÓDULO PEDIDOS ===");
        mvwprintw(w, 4, 2, "1 - Cadastrar Pedido");
        mvwprintw(w, 5, 2, "2 - Listar Pedidos");
        mvwprintw(w, 6, 2, "3 - Consultar Pedido");
        mvwprintw(w, 7, 2, "4 - Remover Pedido");
        mvwprintw(w, 8, 2, "5 - Sair");
        mvwprintw(w, 10,2, "Opção: ");
        wrefresh(w);

        echo();
        wscanw(w, "%d", &op);
        noecho();

        switch(op) {
            case 1: cadastrar_pedido(w); break;
            case 2: listar_pedidos(w); break;
            case 3: consultar_pedido(w); break;
            case 4: remover_pedido(w); break;
        }
    } while (op != 5);
}


// =============================================================
// MAIN
// =============================================================

int main() {
    initscr();
    cbreak();
    noecho();

    WINDOW *win = newwin(24, 80, 1, 2);

    menu_pedidos(win);

    delwin(win);
    endwin();

    return 0;
}
