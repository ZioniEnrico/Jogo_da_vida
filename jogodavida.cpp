#include <windows.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

// ESTRUTURAS DE DADOS
typedef struct {
    int codigo;
    char nome[100];
    int idade;
} MembroFamilia;

typedef struct {
    int codigoMembro;
    char descricao[100];
    float valor;
    char categoria; // 'P' para Pessoal, 'D' para Dividida
} Transacao;

// Estrutura auxiliar para facilitar a busca de nomes nos relatórios
typedef struct {
    int codigo;
    char nome[100];
} NomeMembro;


// PROTÓTIPOS DAS FUNÇÕES
void exibirMenu();
void cadastrarMembro();
void listarMembros();
void iniciarRemocaoMembro();
void adicionarReceita();
void adicionarDespesa();
void gerarRelatorioIndividual();
void gerarRelatorioFamiliar();
void jogarMinigame();
int obterProximoCodigo(const char* nomeArquivo);
bool verificarMembroExiste(int codigoMembro);
void removerMembro(int codigoParaRemover);
void removerTransacoesPorMembro(const char* nomeArquivo, int codigoMembro);
void salvarReceita(Transacao transacao);
void salvarDespesa(Transacao transacao);
void limparTela();
void esperarEnter();
void exibirStatus(int riqueza, int saude, int reputacao, int inovacao);
int contarMembros();
int carregarNomesMembros(NomeMembro nomes[]);
const char* obterNomePorCodigo(int codigo, const NomeMembro nomes[], int numNomes);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "C.UTF-8");
    int opcao;
    do {
        exibirMenu();
        scanf("%d", &opcao);
        getchar();
        switch (opcao) {
            case 1: cadastrarMembro(); break;
            case 2: listarMembros(); break;
            case 3: iniciarRemocaoMembro(); break;
            case 4: adicionarReceita(); break;
            case 5: adicionarDespesa(); break;
            case 6: gerarRelatorioIndividual(); break;
            case 7: gerarRelatorioFamiliar(); break;
            case 8: jogarMinigame(); break;
            case 0: printf("Encerrando o programa. Até mais!\n"); break;
            default: printf("Opção inválida! Tente novamente.\n");
        }
        if (opcao != 0) esperarEnter();
    } while (opcao != 0);
    return 0;
}

void exibirMenu() {
    limparTela();
    printf("====================================================\n");
    printf("   CONTROLE FINANCEIRO E JOGO DA VIDA SUSTENTÁVEL   \n");
    printf("====================================================\n");
    printf("1 - Cadastrar Novo Membro da Família\n");
    printf("2 - Listar Membros da Família\n");
    printf("3 - Remover Membro da Família\n");
    printf("4 - Adicionar Receita\n");
    printf("5 - Adicionar Despesa (Pessoal ou Dividida)\n");
    printf("6 - Gerar Relatório Financeiro Individual\n");
    printf("7 - Gerar Relatório Financeiro da Família\n");
    printf("8 - Jogar o Minigame 'Jogo da Vida'\n");
    printf("0 - Sair\n");
    printf("----------------------------------------------------\n");
    printf("Escolha uma opção: ");
}

void limparTela() {
    system("clear || cls");
}

void esperarEnter() {
    printf("\nPressione Enter para continuar...");
    getchar();
}

void removerMembro(int codigoParaRemover) {
    if (!verificarMembroExiste(codigoParaRemover)) {
        printf("\nERRO: Membro com o código %d não encontrado!\n", codigoParaRemover);
        return;
    }

    FILE *arquivoOriginal = fopen("familiares.txt", "r");
    FILE *arquivoTemporario = fopen("temp_familiares.txt", "w");

    if (arquivoOriginal == NULL || arquivoTemporario == NULL) {
        printf("Erro ao abrir os arquivos para remoção de membro.\n");
        return;
    }

    MembroFamilia membro;
    while (fscanf(arquivoOriginal, "%d;%[^;];%d\n", &membro.codigo, membro.nome, &membro.idade) == 3) {
        if (membro.codigo != codigoParaRemover) {
            fprintf(arquivoTemporario, "%d;%s;%d\n", membro.codigo, membro.nome, membro.idade);
        }
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemporario);

    remove("familiares.txt");
    rename("temp_familiares.txt", "familiares.txt");

    removerTransacoesPorMembro("receitas.txt", codigoParaRemover);
    removerTransacoesPorMembro("despesas.txt", codigoParaRemover);

    printf("\nMembro e todas as suas transações foram removidos com sucesso.\n");
}

void removerTransacoesPorMembro(const char* nomeArquivo, int codigoMembro) {
    FILE *arquivoOriginal, *arquivoTemporario;
    char tempFilename[256];

    if (strcmp(nomeArquivo, "receitas.txt") == 0) {
        arquivoOriginal = fopen(nomeArquivo, "r");
        if (arquivoOriginal == NULL) return;
        strcpy(tempFilename, "temp_receitas.txt");
        arquivoTemporario = fopen(tempFilename, "w");
        if (arquivoTemporario == NULL) { fclose(arquivoOriginal); return; }
        
        Transacao transacao;
        while (fscanf(arquivoOriginal, "%d;%[^;];%f\n", &transacao.codigoMembro, transacao.descricao, &transacao.valor) == 3) {
            if (transacao.codigoMembro != codigoMembro) {
                fprintf(arquivoTemporario, "%d;%s;%.2f\n", transacao.codigoMembro, transacao.descricao, transacao.valor);
            }
        }
    } else if (strcmp(nomeArquivo, "despesas.txt") == 0) {
        arquivoOriginal = fopen(nomeArquivo, "r");
        if (arquivoOriginal == NULL) return;
        strcpy(tempFilename, "temp_despesas.txt");
        arquivoTemporario = fopen(tempFilename, "w");
        if (arquivoTemporario == NULL) { fclose(arquivoOriginal); return; }

        Transacao transacao;
        while (fscanf(arquivoOriginal, "%d;%[^;];%f;%c\n", &transacao.codigoMembro, transacao.descricao, &transacao.valor, &transacao.categoria) == 4) {
            if (transacao.codigoMembro != codigoMembro) {
                 fprintf(arquivoTemporario, "%d;%s;%.2f;%c\n", transacao.codigoMembro, transacao.descricao, transacao.valor, transacao.categoria);
            }
        }
    } else {
        return;
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemporario);

    remove(nomeArquivo);
    rename(tempFilename, nomeArquivo);
}

void listarMembros() {
    limparTela();
    FILE *arquivo = fopen("familiares.txt", "r");
    bool temMembros = false;

    printf("\n--- Lista de Membros da Família ---\n");

    if (arquivo != NULL) {
        if (fgetc(arquivo) != EOF) {
            rewind(arquivo);
            temMembros = true;
            printf("-----------------------------------\n");
            printf("Código | Nome                  | Idade\n");
            printf("-----------------------------------\n");
            MembroFamilia membro;
            while (fscanf(arquivo, "%d;%[^;];%d\n", &membro.codigo, membro.nome, &membro.idade) == 3) {
                printf("%-6d | %-21s | %d\n", membro.codigo, membro.nome, membro.idade);
            }
            printf("-----------------------------------\n");
        }
        fclose(arquivo);
    }

    if (!temMembros) {
        printf("\nNenhum membro cadastrado ainda.\n");
    }
}

void iniciarRemocaoMembro() {
    limparTela();
    printf("--- Remover Membro da Família ---\n");
    listarMembros();
    if (contarMembros() == 0) {
        return;
    }
    int codigoParaRemover;
    printf("\nDigite o código do membro que deseja remover (ou 0 para cancelar): ");
    scanf("%d", &codigoParaRemover);
    getchar();
    if (codigoParaRemover != 0) {
        removerMembro(codigoParaRemover);
    } else {
        printf("\nOperação de remoção cancelada.\n");
    }
}

int obterProximoCodigo(const char* nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) return 1;
    int ultimoCodigo = 0;
    MembroFamilia membro;
    while (fscanf(arquivo, "%d;%[^;];%d\n", &membro.codigo, membro.nome, &membro.idade) == 3) {
        if(membro.codigo > ultimoCodigo) {
            ultimoCodigo = membro.codigo;
        }
    }
    fclose(arquivo);
    return ultimoCodigo + 1;
}

bool verificarMembroExiste(int codigoMembro) {
    FILE *arquivo = fopen("familiares.txt", "r");
    if (arquivo == NULL) return false;
    MembroFamilia membro;
    bool encontrado = false;
    while (fscanf(arquivo, "%d;%[^;];%d\n", &membro.codigo, membro.nome, &membro.idade) == 3) {
        if (membro.codigo == codigoMembro) {
            encontrado = true;
            break;
        }
    }
    fclose(arquivo);
    return encontrado;
}

void salvarMembro(MembroFamilia membro) {
    FILE *arquivo = fopen("familiares.txt", "a");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de familiares");
        return;
    }
    fprintf(arquivo, "%d;%s;%d\n", membro.codigo, membro.nome, membro.idade);
    fclose(arquivo);
}

void salvarReceita(Transacao transacao) {
    FILE *arquivo = fopen("receitas.txt", "a");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de receitas");
        return;
    }
    fprintf(arquivo, "%d;%s;%.2f\n", transacao.codigoMembro, transacao.descricao, transacao.valor);
    fclose(arquivo);
}

void salvarDespesa(Transacao transacao) {
    FILE *arquivo = fopen("despesas.txt", "a");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de despesas");
        return;
    }
    fprintf(arquivo, "%d;%s;%.2f;%c\n", transacao.codigoMembro, transacao.descricao, transacao.valor, transacao.categoria);
    fclose(arquivo);
}

int contarMembros() {
    FILE *arquivo = fopen("familiares.txt", "r");
    if (arquivo == NULL) {
        return 0;
    }
    int contador = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        contador++;
    }
    fclose(arquivo);
    return contador;
}

int carregarNomesMembros(NomeMembro nomes[]) {
    FILE *arquivo = fopen("familiares.txt", "r");
    if (arquivo == NULL) {
        return 0;
    }
    int count = 0;
    while (fscanf(arquivo, "%d;%[^;];%*d\n", &nomes[count].codigo, nomes[count].nome) == 2) {
        count++;
    }
    fclose(arquivo);
    return count;
}

const char* obterNomePorCodigo(int codigo, const NomeMembro nomes[], int numNomes) {
    for (int i = 0; i < numNomes; i++) {
        if (nomes[i].codigo == codigo) {
            return nomes[i].nome;
        }
    }
    return "Desconhecido";
}

void cadastrarMembro() {
    limparTela();
    MembroFamilia novoMembro;
    novoMembro.codigo = obterProximoCodigo("familiares.txt");
    printf("\n--- Cadastro de Novo Membro ---\n");
    printf("Nome: ");
    fgets(novoMembro.nome, 100, stdin);
    novoMembro.nome[strcspn(novoMembro.nome, "\n")] = 0;
    printf("Idade: ");
    scanf("%d", &novoMembro.idade);
    getchar();
    salvarMembro(novoMembro);
    printf("\nMembro '%s' cadastrado com sucesso com o código %d!\n", novoMembro.nome, novoMembro.codigo);
}

void adicionarReceita() {
    limparTela();
    Transacao novaReceita;
    int codigoMembro;
    printf("\n--- Adicionar Nova Receita ---\n");
    listarMembros();
    if (contarMembros() == 0) return;
    printf("Informe o código do membro para adicionar a receita: ");
    scanf("%d", &codigoMembro);
    getchar();
    if (!verificarMembroExiste(codigoMembro)) {
        printf("\nERRO: Membro com o código %d não encontrado!\n", codigoMembro);
        return;
    }
    novaReceita.codigoMembro = codigoMembro;
    printf("Descrição da receita (ex: Salário): ");
    fgets(novaReceita.descricao, 100, stdin);
    novaReceita.descricao[strcspn(novaReceita.descricao, "\n")] = 0;
    printf("Valor da receita: R$ ");
    scanf("%f", &novaReceita.valor);
    getchar();
    salvarReceita(novaReceita);
    printf("\nReceita adicionada com sucesso!\n");
}

void adicionarDespesa() {
    limparTela();
    Transacao novaDespesa;
    char tipoDespesa;
    int codigoMembro;

    printf("\n--- Adicionar Nova Despesa ---\n");

    if (contarMembros() == 0) {
        printf("\nÉ necessário cadastrar pelo menos um membro antes de adicionar uma despesa.\n");
        return;
    }

    printf("A despesa é (P)essoal ou (D)ividida entre todos os membros? ");
    scanf(" %c", &tipoDespesa);
    getchar();
    tipoDespesa = toupper(tipoDespesa);

    if (tipoDespesa != 'P' && tipoDespesa != 'D') {
        printf("\nOpção inválida! A despesa deve ser 'P' ou 'D'.\n");
        return;
    }

    novaDespesa.categoria = tipoDespesa;

    listarMembros();
    if(tipoDespesa == 'P') {
        printf("Informe o código do membro responsável pela despesa: ");
    } else {
        printf("Informe o código do membro que está registrando a despesa dividida: ");
    }
    scanf("%d", &codigoMembro);
    getchar();

    if (!verificarMembroExiste(codigoMembro)) {
        printf("\nERRO: Membro com o código %d não encontrado!\n", codigoMembro);
        return;
    }
    novaDespesa.codigoMembro = codigoMembro;

    printf("Descrição da despesa (ex: Aluguel, Supermercado): ");
    fgets(novaDespesa.descricao, 100, stdin);
    novaDespesa.descricao[strcspn(novaDespesa.descricao, "\n")] = 0;

    printf("Valor da despesa: R$ ");
    scanf("%f", &novaDespesa.valor);
    getchar();

    salvarDespesa(novaDespesa);
    printf("\nDespesa adicionada com sucesso!\n");
}

void gerarRelatorioIndividual() {
    limparTela();
    int codigoMembro;
    float totalReceitas = 0.0, despesasPessoais = 0.0, despesasDivididasTotais = 0.0, totalDespesas = 0.0;
    char nomeMembro[100] = "Não encontrado";

    printf("\n--- Relatório Financeiro Individual ---\n");
    listarMembros();
    
    int numeroMembros = contarMembros();
    if (numeroMembros == 0) {
        return;
    }
    
    printf("Informe o código do membro para gerar o relatório: ");
    scanf("%d", &codigoMembro);
    getchar();

    if (!verificarMembroExiste(codigoMembro)) {
        printf("\nERRO: Membro com o código %d não encontrado!\n", codigoMembro);
        return;
    }

    FILE *arqMembros = fopen("familiares.txt", "r");
    if (arqMembros != NULL) {
        MembroFamilia membro;
        while (fscanf(arqMembros, "%d;%[^;];%d\n", &membro.codigo, membro.nome, &membro.idade) == 3) {
            if (membro.codigo == codigoMembro) {
                strcpy(nomeMembro, membro.nome);
                break;
            }
        }
        fclose(arqMembros);
    }
    
    FILE *arqReceitas = fopen("receitas.txt", "r");
    if (arqReceitas != NULL) {
        Transacao receita;
        while (fscanf(arqReceitas, "%d;%[^;];%f\n", &receita.codigoMembro, receita.descricao, &receita.valor) == 3) {
            if (receita.codigoMembro == codigoMembro) totalReceitas += receita.valor;
        }
        fclose(arqReceitas);
    }

    FILE *arqDespesas = fopen("despesas.txt", "r");
    if (arqDespesas != NULL) {
        Transacao despesa;
        while (fscanf(arqDespesas, "%d;%[^;];%f;%c\n", &despesa.codigoMembro, despesa.descricao, &despesa.valor, &despesa.categoria) == 4) {
            if (despesa.categoria == 'P' && despesa.codigoMembro == codigoMembro) {
                despesasPessoais += despesa.valor;
            } else if (despesa.categoria == 'D') {
                despesasDivididasTotais += despesa.valor;
            }
        }
        fclose(arqDespesas);
    }
    
    float parcelaDividida = (numeroMembros > 0) ? (despesasDivididasTotais / numeroMembros) : 0;
    totalDespesas = despesasPessoais + parcelaDividida;
    float saldo = totalReceitas - totalDespesas;

    printf("\n--- Relatório para: %s ---\n", nomeMembro);
    printf("Total de Receitas................: + R$ %.2f\n", totalReceitas);
    printf("Total de Despesas Pessoais.......: - R$ %.2f\n", despesasPessoais);
    printf("Sua parcela das Despesas Divididas: - R$ %.2f\n", parcelaDividida);
    printf("--------------------------------------------------\n");
    printf("Saldo Final Individual...........:   R$ %.2f\n", saldo);
    printf("--------------------------------------------------\n\n");

    printf("--- Detalhes das Receitas ---\n");
    arqReceitas = fopen("receitas.txt", "r");
    if (arqReceitas != NULL) {
        Transacao receita;
        bool encontrouReceita = false;
        while (fscanf(arqReceitas, "%d;%[^;];%f\n", &receita.codigoMembro, receita.descricao, &receita.valor) == 3) {
            if (receita.codigoMembro == codigoMembro) {
                printf("  [+] R$ %-8.2f | %s\n", receita.valor, receita.descricao);
                encontrouReceita = true;
            }
        }
        if (!encontrouReceita) {
            printf("  Nenhuma receita pessoal encontrada.\n");
        }
        fclose(arqReceitas);
    }
    printf("\n");

    printf("--- Detalhes das Despesas ---\n");
    arqDespesas = fopen("despesas.txt", "r");
    if (arqDespesas != NULL) {
        Transacao despesa;
        bool encontrouDespesa = false;
        while (fscanf(arqDespesas, "%d;%[^;];%f;%c\n", &despesa.codigoMembro, despesa.descricao, &despesa.valor, &despesa.categoria) == 4) {
            if (despesa.categoria == 'P' && despesa.codigoMembro == codigoMembro) {
                printf("  [-] R$ %-8.2f | %s (Pessoal)\n", despesa.valor, despesa.descricao);
                encontrouDespesa = true;
            } else if (despesa.categoria == 'D') {
                 printf("  [-] R$ %-8.2f | %s (Dividida - Parcela: R$ %.2f)\n", despesa.valor, despesa.descricao, despesa.valor / numeroMembros);
                 encontrouDespesa = true;
            }
        }
        if(!encontrouDespesa) {
             printf("  Nenhuma despesa encontrada.\n");
        }
        fclose(arqDespesas);
    }
    printf("--------------------------------------------------\n\n");

    printf("RECOMENDAÇÃO:\n");
    if (saldo > 0) {
        float proporcaoGasto = (totalReceitas > 0) ? (totalDespesas / totalReceitas) * 100 : 0;
        if (saldo < 100) {
             printf("Parabéns pelo saldo positivo! Cada real economizado é uma vitória.\n");
             printf("Sugestão: Guarde este valor de R$ %.2f. Ele pode ser o primeiro passo para construir sua reserva de emergência. A consistência é a chave!\n", saldo);
        } else {
            printf("Excelente! Você demonstrou um ótimo controle sobre suas finanças.\n");
            printf("Sugestão: Com um saldo de R$ %.2f, você pode acelerar suas metas. Considere usar uma parte para investimentos ou para adiantar o pagamento de alguma dívida, se houver.\n", saldo);
        }
        if(proporcaoGasto > 85 && proporcaoGasto < 100) {
            printf("Alerta: Apesar do saldo positivo, suas despesas estão consumindo %.0f%% de suas receitas. Cuidado com gastos inesperados!\n", proporcaoGasto);
        }
    } else if (saldo < 0) {
        printf("Atenção! Seu saldo está negativo. É um sinal claro de que as despesas foram maiores que as receitas.\n");
        printf("Ação Imediata: Revise suas despesas pessoais (R$ %.2f) e identifique gastos não essenciais que podem ser cortados. Converse com a família sobre as despesas divididas para otimizar os custos em conjunto.\n", despesasPessoais);
    } else {
        printf("Seu orçamento está no limite. Não há espaço para imprevistos.\n");
        printf("Ação Imediata: O objetivo agora é criar uma 'folga' no orçamento. Defina a meta de economizar pelo menos 5%% de sua receita no próximo mês. Isso pode vir de pequenos cortes em várias áreas.\n");
    }
}

void gerarRelatorioFamiliar() {
    limparTela();
    float totalReceitasFamilia = 0.0, totalDespesasFamilia = 0.0;
    
    NomeMembro nomes[100];
    int numNomes = carregarNomesMembros(nomes);

    printf("\n--- Relatório Financeiro Consolidado da Família ---\n");

    FILE *arqReceitas = fopen("receitas.txt", "r");
    if (arqReceitas != NULL) {
        Transacao receita;
        while (fscanf(arqReceitas, "%d;%[^;];%f\n", &receita.codigoMembro, receita.descricao, &receita.valor) == 3) {
            totalReceitasFamilia += receita.valor;
        }
        fclose(arqReceitas);
    }

    FILE *arqDespesas = fopen("despesas.txt", "r");
    if (arqDespesas != NULL) {
        Transacao despesa;
        while (fscanf(arqDespesas, "%d;%[^;];%f;%c\n", &despesa.codigoMembro, despesa.descricao, &despesa.valor, &despesa.categoria) == 4) {
            if (despesa.categoria == 'D') {
                totalDespesasFamilia += despesa.valor;
            }
        }
        fclose(arqDespesas);
    }
    
    float saldoFamiliar = totalReceitasFamilia - totalDespesasFamilia;

    printf("Total de Receitas da Família: R$ %.2f\n", totalReceitasFamilia);
    printf("Total de Despesas (apenas contas divididas): R$ %.2f\n", totalDespesasFamilia);
    printf("---------------------------------------------------\n");
    printf("Saldo Final da Família:                       R$ %.2f\n", saldoFamiliar);
    printf("---------------------------------------------------\n\n");

    printf("--- Detalhes das Receitas da Família ---\n");
    arqReceitas = fopen("receitas.txt", "r");
    if (arqReceitas != NULL) {
        Transacao receita;
        bool encontrouReceita = false;
        while (fscanf(arqReceitas, "%d;%[^;];%f\n", &receita.codigoMembro, receita.descricao, &receita.valor) == 3) {
            const char* nome = obterNomePorCodigo(receita.codigoMembro, nomes, numNomes);
            printf("  [+] R$ %-8.2f | %s (de: %s)\n", receita.valor, receita.descricao, nome);
            encontrouReceita = true;
        }
        if (!encontrouReceita) {
            printf("  Nenhuma receita encontrada na família.\n");
        }
        fclose(arqReceitas);
    }
    printf("\n");

    printf("--- Detalhes das Despesas da Família (Divididas) ---\n");
    arqDespesas = fopen("despesas.txt", "r");
    if (arqDespesas != NULL) {
        Transacao despesa;
        bool encontrouDespesa = false;
        while (fscanf(arqDespesas, "%d;%[^;];%f;%c\n", &despesa.codigoMembro, despesa.descricao, &despesa.valor, &despesa.categoria) == 4) {
            if (despesa.categoria == 'D') {
                const char* nome = obterNomePorCodigo(despesa.codigoMembro, nomes, numNomes);
                printf("  [-] R$ %-8.2f | %s (registrado por: %s)\n", despesa.valor, despesa.descricao, nome);
                encontrouDespesa = true;
            }
        }
        if(!encontrouDespesa) {
             printf("  Nenhuma despesa dividida encontrada.\n");
        }
        fclose(arqDespesas);
    }
    printf("---------------------------------------------------\n\n");

    printf("RECOMENDAÇÃO FAMILIAR:\n");
    if (saldoFamiliar > 1000) {
        printf("Parabéns! A saúde financeira da família está excelente, com um saldo positivo considerável sobre as contas conjuntas.\n");
        printf("Sugestão: Este é um ótimo momento para uma reunião familiar e definir metas de longo prazo, como a compra de um bem ou um investimento familiar.\n");
    } else if (saldoFamiliar > 0) {
        printf("Muito bem! A família conseguiu cobrir todas as despesas divididas e ainda ter um saldo positivo.\n");
        printf("Sugestão: Usem este saldo para fortalecer a reserva de emergência da família. Ter o equivalente a 3-6 meses de despesas guardado traz muita segurança.\n");
    } else if (saldoFamiliar < 0) {
        printf("Alerta! As receitas da família não foram suficientes para cobrir as despesas divididas. É crucial uma ação conjunta.\n");
        printf("Ação Imediata: Marquem uma reunião de orçamento familiar URGENTE. É preciso rever as contas conjuntas ou planejar como aumentar a receita total da família.\n");
    } else {
        printf("O orçamento familiar cobriu exatamente as contas divididas, mas sem folgas.\n");
        printf("Ação Imediata: O diálogo é o primeiro passo. Busquem em conjunto pequenas economias nas contas da casa para criar uma reserva de segurança.\n");
    }
}

void exibirStatus(int riqueza, int saude, int reputacao, int inovacao) {
    printf("---------------------------------------------------\n");
    printf("STATUS: Riqueza: $%d | Saúde: %d ♥ | Reputação: %d ★ | Inovação: %d ⚡\n", riqueza, saude, reputacao, inovacao);
    printf("---------------------------------------------------\n\n");
}

void jogarMinigame() {
    int riqueza = 50;
    int saude = 10;
    int reputacao = 10;
    int inovacao = 10;

    int escolha;
    int caminho = 0;
    char nomeJogador[50];

    srand(time(NULL));

    limparTela();
    printf("\n=====================================\n");
    printf("   BEM-VINDO AO JOGO DA VIDA: DESTINOS\n");
    printf("=====================================\n");
    printf("Sua jornada financeira começa agora. Suas escolhas afetarão sua Riqueza, Saúde, Reputação e capacidade de Inovação.\n");
    printf("Digite seu nome para começar: ");
    fgets(nomeJogador, 50, stdin);
    nomeJogador[strcspn(nomeJogador, "\n")] = 0;
    
    esperarEnter();

    limparTela();
    printf("\n--- CAPÍTULO 1: A ENCRUZILHADA ---\n\n");
    exibirStatus(riqueza, saude, reputacao, inovacao);
    printf("%s, após anos de estudo, você está diante da primeira grande decisão profissional.\nQual caminho você seguirá?\n\n", nomeJogador);
    printf("1. Empreendedorismo: Abrir uma startup de tecnologia. (+Inovação, -Saúde)\n");
    printf("2. Carreira Corporativa: Vaga de trainee em uma multinacional. (+Riqueza)\n");
    printf("3. Serviço Público: Cargo estável no governo. (+Reputação, +Saúde)\n");
    printf("4. Vida Artística: Tentar a vida como músico(a) independente. (-Riqueza, +Reputação)\n");
    printf("5. Aventura: Usar suas economias para viajar pelo mundo. (-Riqueza)\n");
    printf("\nSua escolha: ");
    scanf("%d", &escolha);
    getchar();

    switch (escolha) {
        case 1:
            printf("\n> Você escolheu o caminho do Empreendedor! A jornada será estressante, mas com grande potencial criativo.");
            caminho = 1; inovacao += 5; saude -= 2; riqueza -= 5;
            break;
        case 2:
            printf("\n> Você escolheu a Carreira Corporativa! Um mundo de metas, promoções e desafios te aguarda.");
            caminho = 2; riqueza += 5; reputacao -= 1;
            break;
        case 3:
            printf("\n> Você escolheu o Serviço Público! Estabilidade e a chance de servir à comunidade guiarão seus passos.");
            caminho = 3; reputacao += 3; saude += 2; riqueza -= 5;
            break;
        case 4:
            printf("\n> Você segue sua paixão! A instabilidade é grande, mas a admiração da sua pequena base de fãs é real.");
            caminho = 0; riqueza = 35; reputacao = 15; saude = 12; inovacao = 12;
            break;
        case 5:
            printf("\n> A viagem foi incrível! Você volta com muitas histórias, mas com o bolso vazio.");
            caminho = 0; riqueza = 20; saude = 15; reputacao = 12; inovacao = 8;
            break;
    }

    if (caminho != 0) {
        esperarEnter();
        limparTela();
        printf("\n--- CAPÍTULO 2: A PROVA DE FOGO ---\n\n");
        exibirStatus(riqueza, saude, reputacao, inovacao);
        if (caminho == 1) printf("Sua startup recebeu o primeiro aporte! É hora de decidir como usar o capital para crescer.\n");
        else if (caminho == 2) printf("Você recebeu uma promoção e um bônus generoso. É sua chance de fazer o dinheiro trabalhar para você.\n");
        else if (caminho == 3) printf("Com a estabilidade do cargo, você acumulou economias. É o momento de pensar no futuro financeiro.\n");
        printf("O que você faz com este dinheiro significativo?\n\n");
        printf("1. Aposto em criptomoedas de alto risco. (Alta chance de Riqueza, alto risco)\n");
        printf("2. Invisto em minha própria educação, fazendo um MBA ou uma especialização cara. (+Inovação, +Riqueza futura)\n");
        printf("3. Financio a construção de um centro comunitário no bairro. (++Reputação)\n");
        printf("4. Compro uma casa confortável para minha família. (+Saúde, +Reputação, -Riqueza)\n");
        printf("5. Compro um carro esportivo e artigos de luxo. (-Riqueza, --Reputação)\n");
        printf("\nSua escolha: ");
        scanf("%d", &escolha);
        getchar();
        switch(escolha) {
            case 1: riqueza += 35; saude -= 3; reputacao -= 2; printf("\n> A aposta deu certo, por enquanto. Você está rico, mas o estresse foi enorme.\n"); break;
            case 2: inovacao += 7; riqueza += 10; saude -= 2; printf("\n> Investimento inteligente. Você está mais qualificado e seu potencial de ganho aumentou.\n"); break;
            case 3: reputacao += 8; riqueza -= 10; printf("\n> Você se torna uma figura amada e respeitada, um verdadeiro pilar para muitos.\n"); break;
            case 4: saude += 4; reputacao += 2; riqueza -= 5; printf("\n> Sua família está segura e feliz. A paz de espírito não tem preço.\n"); break;
            case 5: riqueza -= 20; reputacao -= 5; printf("\n> Você ostenta, mas as pessoas o veem como alguém fútil. A satisfação é passageira.\n"); break;
        }
        esperarEnter();
        limparTela();
        printf("\n--- CAPÍTULO 3: UMA REVIRAVOLTA DO DESTINO ---\n\n");
        exibirStatus(riqueza, saude, reputacao, inovacao);
        int evento = rand() % 3;
        if (evento == 0) {
            printf("Crise de Saúde Inesperada! Você é diagnosticado com uma condição médica que exige tratamento.\nComo proceder?\n\n");
            printf("1. Uso o melhor tratamento particular disponível, caríssimo. (-Riqueza, ++Saúde)\n");
            printf("2. Vendo bens para pagar um tratamento decente, mas não o de ponta. (-Riqueza, +Saúde)\n");
            printf("3. Dependo apenas do sistema público, que é lento e incerto. (--Saúde)\n");
            printf("4. Busco tratamentos alternativos e não comprovados. (-Riqueza, -Saúde)\n");
            printf("5. Ignoro o problema e trabalho mais para esquecer. (---Saúde)\n");
            printf("\nSua escolha: ");
            scanf("%d", &escolha); getchar();
            if (escolha == 1) { riqueza -= 25; saude += 6; }
            else if (escolha == 2) { riqueza -= 15; saude += 4; }
            else if (escolha == 3) { saude -= 5; }
            else if (escolha == 4) { riqueza -= 5; saude -= 3; }
            else if (escolha == 5) { saude -= 8; }
        } else if (evento == 1) {
            printf("Dilema Ético! Surge uma oportunidade de ganhar muito dinheiro de forma questionável.\nO que você faz?\n\n");
            printf("1. Aceito sem pensar duas vezes. Dinheiro é dinheiro. (++Riqueza, ---Reputação)\n");
            printf("2. Aceito, mas doo parte dos lucros para caridade para limpar a consciência. (+Riqueza, --Reputação)\n");
            printf("3. Recuso a oferta e denuncio o esquema para as autoridades. (---Riqueza, +++Reputação)\n");
            printf("4. Recuso a oferta, mas não me envolvo para não ter problemas. (-Riqueza, +Reputação)\n");
            printf("5. Peço mais dinheiro para participar do esquema. (+++Riqueza, ----Reputação)\n");
            printf("\nSua escolha: ");
            scanf("%d", &escolha); getchar();
            if (escolha == 1) { riqueza += 30; reputacao -= 8; }
            else if (escolha == 2) { riqueza += 20; reputacao -= 5; }
            else if (escolha == 3) { riqueza -= 10; reputacao += 9; }
            else if (escolha == 4) { riqueza -= 5; reputacao += 3; }
            else if (escolha == 5) { riqueza += 40; reputacao -= 12; }
        } else {
            printf("Sorte Inesperada! Você recebe uma pequena herança de um parente distante.\nComo usar este presente do destino?\n\n");
            printf("1. Invisto em um projeto de alto risco, mas com alto potencial. (+Riqueza ou ---Riqueza)\n");
            printf("2. Faço uma viagem dos sonhos com a família. (+Saúde, +Reputação familiar)\n");
            printf("3. Pago dívidas e crio uma reserva de emergência. (+Riqueza, +Saúde)\n");
            printf("4. Dou uma festa lendária para todos os amigos. (+Reputação social, -Riqueza)\n");
            printf("5. Guardo o dinheiro no banco sem um plano definido. (+Riqueza)\n");
            printf("\nSua escolha: ");
            scanf("%d", &escolha); getchar();
            if (escolha == 1) { if(rand() % 2 == 0) riqueza += 25; else riqueza -= 15; }
            else if (escolha == 2) { saude += 4; reputacao += 2; }
            else if (escolha == 3) { riqueza += 10; saude += 3; }
            else if (escolha == 4) { reputacao += 4; riqueza -= 5; }
            else if (escolha == 5) { riqueza += 5; }
        }
        
        esperarEnter();
        
        limparTela();
        printf("\n--- CAPÍTULO 4: A CONSTRUÇÃO DO LEGADO ---\n\n");
        exibirStatus(riqueza, saude, reputacao, inovacao);
        printf("Você está no auge de sua carreira. Como você usará sua influência e recursos para moldar o futuro?\n\n");
        printf("1. Foco total em maximizar lucros, custe o que custar. (++Riqueza, --Saúde, --Reputação)\n");
        printf("2. Crio uma fundação filantrópica para apoiar causas sociais. (++Reputação, -Riqueza)\n");
        printf("3. Invisto em pesquisa e desenvolvimento para criar uma tecnologia que mudará o mundo. (++Inovação, +Riqueza)\n");
        printf("4. Reduzo meu ritmo de trabalho para passar mais tempo com a família e cuidar da saúde. (++Saúde, -Riqueza)\n");
        printf("5. Escrevo um livro compartilhando minhas experiências para inspirar outros. (+Reputação, +Inovação)\n");
        printf("\nSua escolha: ");
        scanf("%d", &escolha); getchar();
        switch(escolha) {
            case 1: riqueza += 20; saude -= 5; reputacao -= 5; break;
            case 2: reputacao += 10; riqueza -= 15; break;
            case 3: inovacao += 10; riqueza += 10; break;
            case 4: saude += 8; riqueza -= 10; break;
            case 5: reputacao += 5; inovacao += 3; break;
        }
    }
    
    esperarEnter();
    limparTela();
    printf("\n--- O FIM DA JORNADA: SEU ARQUÉTIPO ---\n\n");
    printf("Os anos se passaram, e o resultado de suas escolhas se materializou.\n");
    exibirStatus(riqueza, saude, reputacao, inovacao);

    if (riqueza > 90 && saude < 8 && reputacao < 8) {
        printf("**ARQUÉTIPO: O MAGNATA SOLITÁRIO**\n");
        printf("Você acumulou uma fortuna colossal, %s. Seu nome é sinônimo de poder e dinheiro. No entanto, o custo foi alto. Sua saúde é frágil e você é visto mais com temor do que com admiração. Você tem tudo que o dinheiro pode comprar, mas talvez tenha perdido o que ele não pode.\n", nomeJogador);
    } else if (reputacao > 20 && riqueza < 60) {
        printf("**ARQUÉTIPO: O PILAR DA COMUNIDADE**\n");
        printf("Você pode não ser a pessoa mais rica, %s, mas seu valor é incalculável. Sua generosidade e liderança o tornaram uma figura amada e essencial. Você construiu um legado de bondade e apoio mútuo que o dinheiro jamais poderia comprar. Sua riqueza está nas vidas que você tocou.\n", nomeJogador);
    } else if (inovacao > 20) {
        printf("**ARQUÉTIPO: A LENDA INOVADORA**\n");
        printf("%s, sua mente brilhante e sua coragem para criar mudaram o jogo. Você não apenas teve sucesso, mas também abriu novos caminhos e deixou uma marca indelével em seu campo. Sua verdadeira herança não é seu saldo bancário, mas o futuro que você ajudou a construir.\n", nomeJogador);
    } else if (saude < 5 && riqueza > 60) {
        printf("**ARQUÉTIPO: O BURNOUT**\n");
        printf("A busca incessante pelo sucesso cobrou seu preço, %s. Você alcançou a segurança financeira, mas sua saúde está em ruínas. A jornada o esgotou de tal forma que agora é difícil aproveitar os frutos do seu trabalho. Um lembrete sombrio de que a saúde é o bem mais precioso.\n", nomeJogador);
    } else if (riqueza < 35) {
        printf("**ARQUÉTIPO: O LUTADOR**\n");
        printf("A vida não foi fácil, %s. Apesar de seus esforços, a estabilidade financeira sempre pareceu fora de alcance. Decisões difíceis e infortúnios o deixaram em uma posição vulnerável. Sua jornada foi de constante luta, um testemunho de resiliência, mesmo diante da adversidade.\n", nomeJogador);
    } else {
        printf("**ARQUÉTIPO: A VIDA EQUILIBRADA**\n");
        printf("Parabéns, %s. Você encontrou o que muitos buscam: o equilíbrio. Você construiu uma vida confortável sem sacrificar sua saúde ou seus valores. Sua jornada não foi de extremos, mas de escolhas conscientes que lhe permitiram ter sucesso financeiro, bem-estar e o respeito daqueles ao seu redor.\n", nomeJogador);
    }
}