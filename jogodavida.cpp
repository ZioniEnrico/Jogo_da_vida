#include<stdio.h>
#include<conio.h> // para getch
#include<stdlib.h>
#include<ctype.h> // para tolower e toupper
#include<string.h> // para strlwr e strupr
#include<locale.h>
#define ESC 27
FILE *arquivo; // Declaração do ponteiro de arquivo

typedef struct {
int id [10];
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

pessoa membros[10];


void miniGame() {
    system("pause");

}

void cadastrarMembro (int posicao) {
    do {
    char resposta;
    printf("===============================\n");
    printf("Cadastro de Membro da família\n");
    printf("===============================\n");
    printf("Você escolheu a posição %d.\n", posicao + 1);
    printf("Por favor, preencha as informações do membro:\n");
    fflush(stdin); // Limpa o buffer de entrada

    printf("\nNome: ");
    fgets(membros[posicao].nome, 50, stdin);
    membros[posicao].nome[strcspn(membros[posicao].nome, "\n")] = 0; // Remove newline
    fflush(stdin); // Limpa o buffer de entrada

    printf("\nCargo: ");
    fgets(membros[posicao].cargo, 50, stdin);
    membros[posicao].cargo[strcspn(membros[posicao].cargo, "\n")] = 0; // Remove newline

    printf("\nMembro cadastrado com sucesso!\n");
    printf("\nESC para sair...\n");
    } while (getche() != ESC);
}

void gerenciarDividasPoupanca(int posicao) {
    do {
        system("cls");
        printf("=============================\n");
        printf("---DIVIDAS E POUPANÇA---\n");
        printf("=============================\n");
    } while (getche() != ESC);
}

void registraTransacao (int posicao) {
    do {
    system("cls");

} while (getch() != ESC);
} 

void menuMembro () {
    do {
        int posicao;
        system("cls");
        printf("=================\n");
        printf("---MENU MEMBRO---\n");
        printf("=================\n");
        printf("\n");
        printf("Lista de membros cadastrados:\n");
        for (int i = 0; i < 10; i++) {
        if (strlen(membros[i].nome) > 0) {
            printf("Membro %d: %s\n", i + 1, membros[i].nome);
        } else {
            printf("Membro %d: Vazio\n", i + 1);
        }
        }
        printf("Selecione uma das posições:\n");
        scanf("%d", &posicao);
        posicao--;
        membros[posicao].id[posicao] = posicao; // Atribui o ID baseado na posição
        printf("Digite uma das seguintes opções:\n");
        printf("1 - Cadastrar novo membro\n");
        printf("2 - Registrar transações (Receitas e Despesas)\n");
        printf("3 - Gerenciar dívidas e poupança\n");
        printf("ESC - Voltar ao menu principal\n");
        switch (getch()) {
            case '1':
                system("cls");
                cadastrarMembro(posicao);
                return;
            case '2':
                system("cls");
                registraTransacao(posicao);
                return;
            case '3':
                system("cls");
                gerenciarDividasPoupanca(posicao);
                return;
            case ESC:
                printf("Voltando ao menu principal...\n");
                return;
            default:
                printf("Opção inválida! Tente novamente.\n");
        } 
    }  while (getch() != ESC);
}

void menu() {
    do {
        system("cls");
        printf("=====================\n");
        printf("---MENU INICIAL---\n");
        printf("=====================\n");
        printf("Escolha uma das seguintes opções:\n");
        printf("1 - Menu Membro\n");
        printf("2 - Menu Familia\n");
        printf("3 - Recomendações Coletivas\n");
        printf("ESC - Sair\n");
        switch (getch()) {
            case '1':
                system("cls");
                menuMembro();
                return;
            case '2':
                system("cls");
                return;
            case ESC:
                printf(".Saindo do sistema...\n");
                return;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (getch() != ESC);
}

int main () {
    setlocale(LC_ALL, "");
    system("cls");
    printf("========================================================\n");
    printf("BEM VINDO AO SISTEMA DE CONTROLE FINANCEIRO DA FAMILIA\n");
    printf("========================================================\n");
    printf("\n");
    printf("Este sistema foi desenvolvido para auxiliar no controle\n");
    printf("financeiro da familia, permitindo o cadastro de membros,\n");
    printf("receitas, despesas, saldo, poupanca e dividas.\n");
    printf("\n");
    printf("Para navegar pelo sistema, utilize as teclas numéricas\n");
    printf("\n");
    printf("========================================================\n");
    printf("Desenvolvido por: \nEnrico Castan Zioni\nGuilherme Ribeiro Gularte\nFelipe Dallé Fardo\n");
    printf("\nData: %s\n", __DATE__);
    printf("\nAperte ENTER para continuar...\n");
    getche();
    while (1) { // Loop infinito
        system("cls");
        printf("=====================\n");
        printf("---MENU INICIAL---\n");
        printf("=====================\n");
        printf("Escolha uma das seguintes opcoes:\n");
        printf("1 - Controle Financeiro da Familia\n");
        printf("2 - Minigame: Jogo da Vida\n");
        printf("ESC - Sair\n");
        switch (getche()) {
            case '1':
                system("cls");
                menu();
                break;
            case '2':
                system("cls");
                miniGame();
                break;
            case ESC:
                printf("Saindo do sistema...\n");
                return 0;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    }
    return 0;
}