
#include <stdio.h>
#include "biblioteca.h"



int main() {
    int opcao;

    do {
        printf("Escolha uma opcao:\n");
        printf("1 - Cadastro de pessoas\n");
        printf("2 - Consultar pessoas\n");
        printf("3 - Listar pessoas por cidade\n");
        printf("4 - Gerar relatorio\n");
        printf("5 - Excluir Pessoa\n");
        printf("6 - Encerrar programa\n");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
               cadastroPessoa();
                // código para cadastro de pessoas
                break;
            case 2:
              consultarPessoa();
                // código para consultar pessoas
                break;
            case 3:
                listarPessoas();
                // código para listar pessoas por cidade
                break;
            case 4:
            gerarRelatorio();
                // código para gerar relatório
                break;
            case 5:
                excluirPessoa();
                // código para excluir pessoa
                break;
            case 6:
                printf("Programa encerrado.\n");
                break;
            default:
                printf("Opcao invalida, escolha novamente.\n");
                break;
        }
    } while (opcao != 6);
}
