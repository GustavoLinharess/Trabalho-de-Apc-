#define TAM_E 4     // definindo o tamanho da string que vai ler o estado
#define TAM_C 100   // Tamanho  do nome da cidade
#define TAM_MAX 100 // tamanho do nome da pessoa
#define TAM_DATA 20 // tamanho da data de nascimento
#define MAX_CIDADE 50
#define MAX_LINHA 100
#define MAX_PESSOAS 100
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>



typedef struct Pessoa
{
    char cpf[14];
    char nome[100];
    int dia;
    int mes;
    int ano;
    char sexo;
    char cidade[4096];
    char uf[3];
} PESSOA;

bool valida_cpf();
bool validadorDeData();
bool validarSexo(char sex);
int busca_dados();
int busca_uf();
void consultarPessoa();
void listarPessoas();
bool verificarCpf();
void excluirPessoa();
void gerarRelatorio();
int idadePc();

void limpa_buffer()
{
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

void cadastroPessoa()
{
    struct Pessoa p;
    FILE *fp;

    printf("Insira o CPF (sem o uso de pontos e hifens): ");
    scanf("%s", p.cpf);
    printf(valida_cpf(p.cpf) ? "\n" : "Invalido\n");
    limpa_buffer();

    printf("Insira o nome completo: ");
    fgets(p.nome, 4096, stdin);
    p.nome[strlen(p.nome) - 1] = NULL;
    limpa_buffer();

    printf("Insira a data de nascimento (dd mm aaaa): ");
    scanf("%d %d %d", &p.dia, &p.mes, &p.ano);
    validadorDeData(p.dia, p.mes, p.ano);
    limpa_buffer();

    printf("Insira o sexo (M ou F): ");
    scanf(" %c", &p.sexo);
    validarSexo(p.sexo);
    limpa_buffer();

    printf("Insira a cidade: ");
    fgets(p.cidade, 4096, stdin);
    p.cidade[strlen(p.cidade) - 1] = NULL;
    printf("%s\n", p.cidade);
    busca_dados(p.cidade);

    printf("Insira o UF: ");
    scanf("%s", p.uf);
    busca_uf(p.uf);

    if (validadorDeData(p.dia, p.mes, p.ano) && validarSexo(p.sexo) && valida_cpf(p.cpf) && busca_dados(p.cidade) && busca_uf(p.uf) && verificarCpf(p.cpf))
    {
        fp = fopen("pessoas.txt", "a"); // abre o arquivo para escrita no final do arquivo
        fprintf(fp, "%s\n%s\n%d %d %d\n%c\n", p.cpf, p.nome, p.dia, p.mes, p.ano, p.sexo);
        fprintf(fp, "%s\n%s\n\n", p.cidade, p.uf);
        fclose(fp);
        printf("Cadastro Valido ! ");
    }
    else
    {
        printf("Cadastro Invalido.");
    }
}

void consultarPessoa()
{
    FILE *file = fopen("pessoas.txt", "r");
    char line[1024];
    int found = 0;
    char cpf[4096];

    printf("Informe o Cpf cadastrado: ");
    scanf("%s", cpf);

    int count = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, cpf))
        {
            printf("%s", line);
            found = 1;
            count = 6;
        }
        else if (found && count > 0)
        {
            printf("%s", line);
            count--;
        }
        else if (found && count == 0)
        {
            break;
        }
    }
    if (found == 0)
    {
        printf("Usario não localizado.");
    }
    fclose(file);
}

bool valida_cpf(char *cpf)
{
    int i, j, digito1 = 0, digito2 = 0, digito_verificador1, digito_verificador2;

    // Verifica se o tamanho do CPF é válido
    if (strlen(cpf) != 11)
    {
        return false;
    }

    // Verifica se todos os dígitos são iguais
    for (i = 0; i < 10; i++)
    {
        if (cpf[i] != cpf[i + 1])
        {
            break;
        }
    }
    if (i == 10)
    {
        return false;
    }

    // Calcula o primeiro dígito verificador
    for (i = 0, j = 10; i < 9; i++, j--)
    {
        digito1 += (cpf[i] - '0') * j;
    }
    digito_verificador1 = 11 - (digito1 % 11);
    if (digito_verificador1 > 9)
    {
        digito_verificador1 = 0;
    }

    // Calcula o segundo dígito verificador
    for (i = 0, j = 11; i < 10; i++, j--)
    {
        digito2 += (cpf[i] - '0') * j;
    }
    digito_verificador2 = 11 - (digito2 % 11);
    if (digito_verificador2 > 9)
    {
        digito_verificador2 = 0;
    }

    // Verifica se os dígitos verificadores calculados são iguais aos dígitos verificadores informados
    if ((digito_verificador1 == (cpf[9] - '0')) && (digito_verificador2 == (cpf[10] - '0')))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool verificarCpf(char *cpf)
{
    FILE *arqv;
    PESSOA pessoa;
    int a = 0;

    // abrir o arquivo
    arqv = fopen("pessoas.txt", "r");
    // verificando se o arquivo existe
    if (arqv != NULL)
    {
        while (!feof(arqv))
        {

            fscanf(arqv, "%[^\n]\n", pessoa.cpf);
            fscanf(arqv, "%[^\n]\n", pessoa.nome);
            fscanf(arqv, "%d %d %d\n", &pessoa.dia, &pessoa.mes, &pessoa.ano);
            fscanf(arqv, "%c\n", &pessoa.sexo);
            fscanf(arqv, "%[^\n]\n", pessoa.cidade);
            fscanf(arqv, "%[^\n]\n", pessoa.uf);

            if (strcmp(cpf, pessoa.cpf) == 0)
            {
                // se retorna 0 tem cpf cadastrado
                a = 0;
                break;
            }
            else
            {
                // se ta retornando 1 nao tem cpf igual cadastrado
                a = a + 1;
            }
        }
        if(a == 0){
            return false;
        }else{
            return true;
        }

        fclose(arqv);
    }
}



bool validarSexo(char sex)
{
    if (sex == 'M' || sex == 'm')
    {
        return true;
    }
    else if (sex == 'F' || sex == 'f')
    {
        return true;
    }
    else
    {
        printf("Sexo invalido\n");
        return false;
    }
}

bool validadorDeData(int dia, int mes, int ano)
{

    // Verificando se o mês é válido
    if (mes < 1 || mes > 12)
    {
        printf("Data de nascimento inválida\n");
        return false;
    }

    // Verificando se o dia é válido
    if (dia < 1)
    {
        printf("Data de nascimento inválida\n");
        return false;
    }

    int dias_mes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0))
    {
        dias_mes[1] = 29;
    }
    if (dia > dias_mes[mes - 1])
    {
        printf("Data de nascimento inválida\n");
        return false;
    }

    // Verificando se o ano é válido
    if (ano < 0)
    {
        printf("Data de nascimento inválida\n");
        return false;
    }
    return true;
}

int busca_dados(char *nome_municipio)
{
    char linha[4096];
    int contador = 0;
    FILE *arquivo = fopen("municipios-brasil.txt", "r");
    int temp = 0;

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        contador = 2;
    }
    else
    {

        while (fgets(linha, 4096, arquivo) && temp != 22280)
        {
            ++temp;
            if (strstr(linha, nome_municipio))
            {
                contador = 1;
                fclose(arquivo);
            }
        }
    }
    return contador == 1 ? 1 : 0;
}

int busca_uf(char *nome_uf)
{
    char linha[4096];
    int contador = 0;
    FILE *arquivo = fopen("municipios-brasil.txt", "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        contador = 2;
    }
    else
    {

        while (fgets(linha, 4096, arquivo))
        {
            if (strstr(linha, nome_uf))
            {
                contador = 1;
                fclose(arquivo);
            }
        }
    }
    return contador == 1 ? 1 : 0;
}

void listarPessoas()
{
    FILE *arrquivo;
    PESSOA humano;
    char linhas[MAX_PESSOAS][MAX_LINHA];
    char nomes[MAX_PESSOAS][MAX_LINHA];
    int nomes_encontrados = 0;
    int cidade_encontrada = 0;
    char cidade[MAX_CIDADE];
    char uf[3];
    int contador = 0, i = 0, j = 0;
    printf("Insira o nome da cidade desejada: \n");
    scanf("%s", cidade);
    printf("Digite a UF da cidade desejada: \n");
    scanf("%s", uf);

    if (busca_dados(cidade) == 0 && busca_uf(uf) == 0)
    {
        printf("Essa cidade nao existe no nosso banco de dados\n");
    }
    else
    {
        arrquivo = fopen("pessoas.txt", "r");
        if (arrquivo)
        {
            while (fscanf(arrquivo, "%[^\n]\n%[^\n]\n%d %d %d\n%c\n%[^\n]\n%[^\n]\n\n",
                          humano.cpf, humano.nome, &humano.dia, &humano.mes, &humano.ano,
                          &humano.sexo, humano.cidade, humano.uf) != EOF)
            {
                if ((strcmp(cidade, humano.cidade) == 0) && (strcmp(uf, humano.uf) == 0))
                {
                    strcpy(nomes[contador], humano.nome);
                    contador++;
                }
            }
            qsort(nomes, contador, sizeof(char) * MAX_LINHA, (int (*)(const void *, const void *))strcmp);
            for (int i = 0; i < contador; i++)
            {
                printf("%s\n", nomes[i]);
            }
            fclose(arrquivo);
        }
        else
        {
            printf("Não foi possível abrir o banco de dados");
        }
    }
}

void gerarRelatorio(){


    PESSOA pessoa_arquivada;
    FILE *relatorio;
    float idade;
    float total15 =0 , total29 = 0, total49 = 0, total60 = 0, total60mais = 0;
    float per15 = 0, per29 = 0, per49 = 0, per60 = 0, per60mais = 0;
    float total_masc = 0, total_fem = 0, per_masc = 0, per_fem = 0;
    float total_pessoas = 0;
    relatorio = fopen("pessoas.txt", "r");
    if(relatorio != NULL){
        while(!feof(relatorio)){

            while (fscanf(relatorio, "%[^\n]\n%[^\n]\n%d %d %d\n%c\n%[^\n]\n%[^\n]\n\n",
            pessoa_arquivada.cpf, pessoa_arquivada.nome, &pessoa_arquivada.dia, &pessoa_arquivada.mes, &pessoa_arquivada.ano,
            &pessoa_arquivada.sexo, pessoa_arquivada.cidade, pessoa_arquivada.uf) != EOF)
            {
            
            total_pessoas = total_pessoas+1;
            idade = idadePc(pessoa_arquivada.dia, pessoa_arquivada.mes, pessoa_arquivada.ano);
            if(idade<16){
                total15 = total15+1;
            }else if(idade>15 && idade<30){
                total29 = total29+1;
            }else if(idade>29 && idade<50){
                total49 = total49+1;
            }else if(idade>49 && idade<61){
                total60 = total60+1;
            }else if(idade>60){
                total60mais = total60mais+1;
            }
            
                if(pessoa_arquivada.sexo == 'm' || pessoa_arquivada.sexo == 'M'){
                    total_masc = total_masc+1;
                }else if(pessoa_arquivada.sexo == 'f' || pessoa_arquivada.sexo == 'F'){
                    total_fem = total_fem+1;
                }
            }

        }
        per15=(total15/total_pessoas)*100;
        per29=(total29/total_pessoas)*100;
        per49=(total49/total_pessoas)*100;
        per60=(total60/total_pessoas)*100;
        per60mais=(total60mais/total_pessoas)*100;

        printf("Porcentagem de pessoas por faixa etaria: \n");
        printf("0 a 15 anos: \n");
        printf("%0.2f %%\n", per15);
        printf("16 a 29 anos: \n");
        printf("%0.2f %%\n", per29);
        printf("30 a 49 anos: \n");
        printf("%0.2f %%\n", per49);
        printf("50 a 60 anos: \n");
        printf("%0.2f %%\n", per60);
        printf("acima de 60 anos: \n");
        printf("%0.2f %%\n", per60mais);
        per_masc = (total_masc/total_pessoas)*100;
        per_fem = (total_fem/total_pessoas)*100;

        
        printf("Porcentagem de pessoas por sexo: \n");
        printf("Masculino: \n");
        printf("%0.2f %%\n", per_masc);
        printf("Feminino: \n");
        printf("%0.2f %%\n", per_fem);
    }

}


void excluirPessoa() {
    
    char cpf[14];
    struct Pessoa pessoa;
    FILE *arquivo = fopen("pessoas.txt", "r");
    FILE *temp = fopen("pessoastemp.txt", "w");

    printf("Digite o CPF da pessoa a ser excluida\n");
    limpa_buffer();
    scanf("%s", cpf);

    while (fscanf(arquivo, "%[^\n]\n%[^\n]\n%d %d %d\n%c\n%[^\n]\n%[^\n]\n\n",
    pessoa.cpf, pessoa.nome, &pessoa.dia, &pessoa.mes, &pessoa.ano,
    &pessoa.sexo, pessoa.cidade, pessoa.uf) != EOF)    
    {
        if (strcmp(cpf, pessoa.cpf) != 0){
            fprintf(temp, "%s\n %s\n %d %d %d %c\n %s\n %s\n", pessoa.cpf, pessoa.nome, pessoa.dia, pessoa.mes, pessoa.ano, pessoa.sexo, pessoa.cidade, pessoa.uf);
        }
    }

    fclose(arquivo);
    fclose(temp);
    remove("pessoas.txt");
    rename("pessoastemp.txt", "pessoas.txt");
    printf("Usuario excluido com sucesso\n");
}


int idadePc(int dia, int mes, int ano){
    struct tm *calculo;
    time_t data;
    int idade;
    int diaAtual = 0, mesAtual = 0, anoAtual = 0;

time(&data);
calculo = localtime(&data);
diaAtual = calculo->tm_mday;
mesAtual = calculo->tm_mon+1;
anoAtual = calculo->tm_year + 1900;

if(mes < mesAtual){
idade = anoAtual - ano;} 
else if(mes > mesAtual){idade = (anoAtual-1) - ano;}
else if(mes == mesAtual-1){
    if(dia <= diaAtual){
        idade = anoAtual - ano;
    }
else if(dia > diaAtual){
    idade = (anoAtual-1) - ano;
}
}



return idade;


}