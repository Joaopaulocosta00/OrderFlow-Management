#include<stdio.h>
#include<string.h>

void cadastrarCliente(char clientes[][20], int *numClientes) {
    char codigo[20];
    int existe = 0;
    printf("Digite o c�digo do cliente: "); 
    scanf("%s", codigo);
    for(int i = 0; i < *numClientes; i++) {
        if(strcmp(clientes[i], codigo) == 0) {
            existe = 1;
            break;
        }
    }
    // Verifica se o c�digo j� existe
    if(existe) {
        printf("Erro: O c�digo j� existe. Tente novamente.\n");
    } /*else { // Se o c�digo n�o existe, cadastra o cliente
        strcpy(clientes[*numClientes], codigo);
        (*numClientes)++;
        printf("Cliente cadastrado com sucesso!\n");
    }*/
}
int main() {
    char clientes[100][20];
    int numClientes = 0;
    cadastrarCliente(clientes, &numClientes);
    return 0;
}
// Fun��o para validar CPF
int validarCPF(const char *cpf) {
    int i, j, digito1 = 0, digito2 = 0;

    // Verifica se o CPF tem 11 d�gitos
    if (strlen(cpf) != 11) 
    return 0;

    //Verifica se contem apenas n�meros
    for(i = 0; i < 11; i++) {
        if(cpf[i] < '0' || cpf[i] > '9') 
        return 0;
    }

    // Calcula o primeiro d�gito verificador
    for (i = 0, j = 10; i < 9; i++, j--) {
        digito1 += (cpf[i] - '0') * j;
    }
    digito1 = (digito1 * 10) % 11;
    if (digito1 == 10) digito1 = 0;

    // Calcula o segundo d�gito verificador
    for (i = 0, j = 11; i < 10; i++, j--) {
        digito2 += (cpf[i] - '0') * j;
    }
    digito2 = (digito2 * 10) % 11;
    if (digito2 == 10) digito2 = 0;

    // Verifica se os d�gitos calculados s�o iguais aos informados
    return (digito1 == (cpf[9] - '0') && digito2 == (cpf[10] - '0'));
}
// Fun��o para validar CNPJ
int validarCNPJ(const char *cnpj) { 
    int i, j, digito1 = 0, digito2 = 0;
    int pesos1[12] = {5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2};
    int pesos2[13] = {6, 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2};

    // Verifica se o CNPJ tem 14 d�gitos
    if (strlen(cnpj) != 14) return 0;

    //Verifica se contem apenas n�meros
    for(i = 0; i < 14; i++) {
        if(cnpj[i] < '0' || cnpj[i] > '9') return 0;
    }

    // Calcula o primeiro d�gito verificador
    for (i = 0; i < 12; i++) {
        digito1 += (cnpj[i] - '0') * pesos1[i];
    }
    digito1 = digito1 % 11;
    digito1 = (digito1 < 2) ? 0 : 11 - digito1;

    // Calcula o segundo d�gito verificador
    for (i = 0; i < 13; i++) {
        digito2 += (cnpj[i] - '0') * pesos2[i];
    }
    digito2 = digito2 % 11;
    digito2 = (digito2 < 2) ? 0 : 11 - digito2;

    // Verifica se os d�gitos calculados s�o iguais aos informados
    return (digito1 == (cnpj[12] - '0') && digito2 == (cnpj[13] - '0'));
}
int main() {
    char cadastro[20];
    int tipo;
    printf("Digite 1 para validar CPF ou 2 para validar CNPJ: ");
    scanf("%d", &tipo); 
        //Numero de CPF menos ou mais  de 11 caracteres
    //if  (cadastro<11 || cadastro>11) {
            //printf("CPF inv�lido! Deve conter 11 d�gitos.\n");}
        
    if(tipo == 1) {
        printf("Digite o CPF (apenas n�meros): ");
        scanf("%s", cadastro);
        if (validarCPF(cadastro)) {
            printf("CPF v�lido!\n");
        } else {
            printf("CPF inv�lido!\n");
        }


    } else if(tipo == 2) {
        printf("Digite o CNPJ (apenas n�meros): ");
        scanf("%s", cadastro);
        if (validarCNPJ(cadastro)) {
            printf("CNPJ v�lido!\n");
        } else {
            printf("CNPJ inv�lido!\n");
        }
    } else {
        printf("Op��o inv�lida!\n");
    }
    return 0;
}
