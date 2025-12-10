#ifndef FEATURE_INTERFACE_H
#define FEATURE_INTERFACE_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void iniciarInterface();
void fecharInterface();
void limparTela();
void pausa();
void mostrarCabecalho(const char* titulo);

//Menu
int menuPrincipal();
void mostrarMenuClientes();
void mostrarMenuProdutos(); 

#endif
