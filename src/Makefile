CC = gcc
CFLAGS = -Wall -Wextra -g -I../include
LDFLAGS = -lncurses

SRC = main.c persistencia.c produto_apa.c modulo_produto.c produto_cad.c pedido.c interface.c cliente.c
OBJ = $(SRC:.c=.o)

OUT = output/main

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(OUT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(OUT)
