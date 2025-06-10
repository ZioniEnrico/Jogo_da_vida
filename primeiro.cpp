#include<stdio.h>
#include<conio.h> // para getch
#include<stdlib.h>
#include<ctype.h> // para tolower e toupper
#include<string.h> // para strlwr e strupr
#include<locale.h>
#define ESC 27

typedef struct {
char nome[50];
char cargo[50];
char nMeta[50];
double receita[12];
double despesa[12];
double saldo[12];
double poupanca[12];
double dividas[12];
double meta[12];
} pessoa;

typedef struct {
pessoa membro[10];
} familia;

void menuFamilia () {
    printf("Digite uma das seguintes opções:\n");
    printf("1 - Cadastrar membro da familia\n");
    //Modulo cadastrar
    printf("2 - Listar membros da familia\n");
    //Modulo listar
    printf("3 - Excluir membro da familia\n");
    //Modulo excluir
    printf("4 - Editar membro da familia\n");
    //Modulo editar
}

void menuMembros () {
    printf("Digite uma das seguintes opções:\n");
    printf("1 - Cadastrar receita\n");
    //Modulo cadastrar receita
    printf("2 - Cadastrar despesa\n");
    //Modulo cadastrar despesa
    printf("3 - Listar receitas\n");
    //Modulo listar receitas
    printf("4 - Listar despesas\n");
    //Modulo listar despesas
    printf("5 - Listar saldo\n");
    //Modulo listar saldo
    printf("6 - Listar poupanca\n");
    //Modulo listar poupanca
    printf("7 - Listar dividas\n");
    //Modulo listar dividas
}

int main () {
    system("chcp 65001 > nul");
    setlocale(LC_ALL, "portuguese");
    printf("teste");
    do {
    system("cls");
    printf("========================================================\n");
    printf("Bem vindo ao sistema de controle financeiro da familia\n");
    printf("========================================================\n");
    printf("\n");
    printf("Digite uma das seguintes opções:\n");
    printf("1 - Menu Família\n");
    printf("2 - Menu Membros\n");
    printf("ESC - Sair\n");

    } while (getch() != ESC);
    return 0;
}