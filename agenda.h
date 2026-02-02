#ifndef AGENDA_H
#define AGENDA_H
#include <stdio.h>

typedef enum {
    reuniao = 1,
    trabalho_individual = 2,
    relaxamento = 3
} tipo_atividade;

typedef struct {
    int dia,mes,ano;
} Data;

typedef struct {
    int hora,minuto;
} Hora;

typedef struct {
    char nome[50];
    char sobrenome[100];
    char telefone[20];
    char email[100];
    char local_trabalho[100];
} contato;

typedef struct {
    char descricao[200];
    Data data;
    Hora horario_inicial;
    tipo_atividade tipo;
    char **participantes;
    int quantidade_participantes;
} atividade;



int busca_contatos(contato *lista_contatos,int quantidade,char *nome_buscado);
int adicionar_contato(contato **lista_contatos,int *quantidade,contato novo_contato);
int deletar_contato(contato **lista_contatos,int *quantidade,char *nome);
int adicionar_atividade(atividade **lista_compromissos,int *quantidade);
int deletar_atividade(atividade **lista_compromissos,int *quantidade,int indice_deletar);
int verificar_atividade(atividade compromisso,int opcao_filtro);
int salvar_arquivos(contato *lista_contato,int quantidade_contato,atividade *lista_atividade,int quantidade_atividade);
int carregar_arquivos(contato **lista_contato,int *quantidade_contato,atividade **lista_atividade,int *quantidade_atividade);
int adicionar_participante(atividade *reuniao,char *nome_participante);



#endif