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

pessoa membros[10];

void cadastrarMembro () {
    int posicao;
    printf("===============================\n");
    printf("Cadastro de Membro da família\n");
    printf("===============================\n");
    printf("Você poderá cadastrar até 10 membros.\n");
    printf("Segue a lista de posições disponíveis:\n");
    for (int i = 0; i < 10; i++) {
        if (strlen(membros[i].nome) == 0) {
            printf("Posição %d: Vazia\n", i + 1);
        } else {
            printf("Posição %d: %s\n", i + 1, membros[i].nome);
        }
    }
    do {
        printf("Digite a posição do membro\nque gostaria de cadastrar (1-10): ");
        scanf("%d", &posicao);
    if (posicao < 0 || posicao >= 10) {
        printf("Posição inválida! Deve ser entre 0 e 9.\n");
        return;
    }
    if (posicao > 0 && strlen(membros[posicao].nome) > 0) {
        printf("Já existe um membro cadastrado nessa posição!\n");
    }
 } while (posicao < 1 || posicao > 10);
    posicao--; // Ajusta para índice de array (0-9)
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
    fflush(stdin); // Limpa o buffer de entrada
    printf("\nReceita fixa: R$");
    scanf("%lf", &membros[posicao].receita[0]); // Lê um double
    fflush(stdin); // Limpa o buffer de entrada
    printf("\nDespesa fixa: R$");
    scanf("%lf", &membros[posicao].despesa[0]); // Lê um double
    fflush(stdin); // Limpa o buffer de entrada
    printf("\nSaldo atual: R$");
    membros[posicao].saldo[0] = membros[posicao].receita[0] - membros[posicao].despesa[0]; // Calcula o saldo
    fflush(stdin); // Limpa o buffer de entrada
    printf("\nTem algum valor em poupança (se não, digite 0): R$");
    scanf("%lf", &membros[posicao].poupanca[0]); // Lê um double
    fflush(stdin); // Limpa o buffer de entrada
    printf("\nPossui dividas: (se não, digite 0) R$");
    scanf("%lf", &membros[posicao].dividas[0]); // Lê um double
    fflush(stdin); // Limpa o buffer de entrada
    printf("\nDigite o nome da meta: ");
    fgets(membros[posicao].nMeta, 50, stdin);
    printf("\nDigite o valor de sua meta: R$");
    scanf("%lf", &membros[posicao].meta[0]); // Lê um double   
    fflush(stdin); // Limpa o buffer de entrada
    printf("\nMembro cadastrado com sucesso!\n");
    printf("\nAperte ENTER para continuar...\n");
    getch();
}

void relatoriosFamilia () {
    do {
        system("cls");
        printf("====================\n");
        printf("---MENU MEMBRO---\n");
        printf("====================\n");
        printf("\n");
        printf("Digite uma das seguintes opções:\n");
        printf("1 - Cadastrar receita\n");
        printf("2 - Cadastrar despesa\n");
        printf("3 - Listar receitas\n");
        printf("4 - Listar despesas\n");
        printf("5 - Listar saldo\n");
        printf("6 - Listar poupanca\n");
        printf("7 - Listar dividas\n");
        printf("ESC - Voltar ao menu principal\n");
        switch (getch()) {
            case '1':
                system("cls");
                printf("Cadastrar receita ainda não implementado.\n");
                return;
            case '2':
                system("cls");
                printf("Cadastrar despesa ainda não implementado.\n");
                return;
            case '3':
                system("cls");
                printf("Listar receitas ainda não implementado.\n");
                return;
            case '4':
                system("cls");
                printf("Listar despesas ainda não implementado.\n");
                return;
            case '5':
                system("cls");
                printf("Listar saldo ainda não implementado.\n");
                return;
            case '6':
                system("cls");
                printf("Listar poupanca ainda não implementado.\n");
                return;
            case '7':
                system("cls");
                printf("Listar dividas ainda não implementado.\n");
                return;
            case ESC:
                printf("Voltando ao menu principal...\n");
                return;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
} while (getch() != ESC);
}

void menuFamilia () {
    do {
        system("cls");
        printf("==========================================\n");
        printf("---MENU CONTROLE FINANCEIRO DA FAMILIA---\n");
        printf("==========================================\n");
        printf("\n");
        printf("Digite uma das seguintes opções:\n");
        printf("1 - Cadastrar membro\n");
        printf("2 - Relatório da família\n");
        printf("3 - Recomendações coletivas\n");
        printf("ESC - Voltar ao menu principal\n");
        switch (getch()) {
            case '1':
                system("cls");
                cadastrarMembro();
                return;
            case '2':
                system("cls");
                printf("Relatório da Família ainda não implementado.\n");
                return;
            case '3':
                system("cls");
                printf("Recomendações coletivas ainda não implementado.\n");
                return;
            case ESC:
                printf("Voltando ao menu principal...\n");
                return;
            default:
                printf("Opção inválida! Tente novamente.\n");
        } 
    }  while (getch() != ESC);
}


int main () {
    system("chcp 65001 > nul");
    setlocale(LC_ALL, "portuguese");
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
    getch();
    while (1) { // Loop infinito
        system("cls");
        printf("=====================\n");
        printf("---MENU INICIAL---\n");
        printf("=====================\n");
        printf("Escolha uma das seguintes opcoes:\n");
        printf("1 - Controle Financeiro da Familia\n");
        printf("2 - Minigame: Jogo da Vida\n");
        printf("ESC - Sair\n");
        switch (getch()) {
            case '1':
                system("cls");
                menuFamilia();
                break;
            case '2':
                system("cls");
                break;
            case '3':
                system("cls");
                printf("Jogo da Vida ainda não implementado.\n");
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