# Compilador
CC = gcc

# Opções de compilação
CFLAGS = -Wall -Wextra -g -Iinclude

# Lista de todos os arquivos .c do projeto
SRC = main.c persistencia.c produto_apa.c modulo_produto.c produto_cad.c pedido.c interface.c cliente.c

# Arquivo de saída
OUT = output/main

# Regra padrão
all: $(OUT)

# Como gerar o executável
$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

# Limpeza (remove o executável)
clean:
	rm -f $(OUT)
