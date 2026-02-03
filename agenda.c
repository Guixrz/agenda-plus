#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include "agenda.h"

int strings_iguais_auxiliar(char *s1,char *s2) {
    while (*s1 || *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return 0;
        }
        s1++;
        s2++;
    }
    return 1;
}

int busca_contatos(contato *lista_contatos,int quantidade,char *nome_buscado) {
    for(int i=0;i<quantidade;i++) {
        if(strings_iguais_auxiliar(lista_contatos[i].nome,nome_buscado)==1) {
            return i;
        }
    }
    return -1;
}

int adicionar_contato(contato **lista_contatos,int *quantidade,contato novo_contato) {
    for(int i=0;i<*quantidade;i++) {
        if(strcmp((*lista_contatos)[i].email,novo_contato.email)==0) {
            return 0;
        }
    }
    (*quantidade)++;
    contato *temp = (contato *)realloc(*lista_contatos,*quantidade * sizeof(contato));
    if(temp == NULL) {
        (*quantidade)--;
        return -1;
    } else {
        *lista_contatos = temp;
        (*lista_contatos)[*quantidade - 1] = novo_contato;
        return 1;
    }
}
int deletar_contato(contato **lista_contatos,int *quantidade,char *nome) {
    if(*quantidade ==0) {
        return -1;
    }
    int indice = busca_contatos(*lista_contatos,*quantidade,nome);
    if(indice != -1) {
        for(int j=indice;j<*quantidade - 1;j++) {
            (*lista_contatos)[j] = (*lista_contatos)[j+1];
        }
        (*quantidade)--;
        if(*quantidade == 0) {
            free(*lista_contatos);
            *lista_contatos = NULL;
            return 1;
        } else {
            contato *temp = (contato *)realloc(*lista_contatos,*quantidade * sizeof(contato));
            if(temp!=NULL) {
                *lista_contatos = temp;
            }
            return 1;
        }
    }
    return 0;
}
int adicionar_atividade(atividade **lista_compromissos,int *quantidade) {
    (*quantidade)++;
    atividade *temp = (atividade *)realloc(*lista_compromissos,*quantidade * sizeof(atividade));
    if(temp == NULL) {
        (*quantidade)--;
        return -1;
    } else {
        *lista_compromissos = temp;
        (*lista_compromissos)[*quantidade - 1].participantes = NULL;
        (*lista_compromissos)[*quantidade - 1].quantidade_participantes = 0;
        return 1;
    }
}
int deletar_atividade(atividade **lista_compromissos,int *quantidade,int indice_deletar) {
    if(*quantidade == 0) {
        return -1;
    }
    if(indice_deletar < 0 || indice_deletar >= *quantidade) {
        return 0;
    }
    for(int i=0;i<(*lista_compromissos)[indice_deletar].quantidade_participantes;i++) {
        free((*lista_compromissos)[indice_deletar].participantes[i]);
    }
    free((*lista_compromissos)[indice_deletar].participantes);

    for(int j=indice_deletar;j<*quantidade - 1;j++) {
        (*lista_compromissos)[j] = (*lista_compromissos)[j + 1];
    }
    (*quantidade)--;

    if(*quantidade == 0) {
        free(*lista_compromissos);
        *lista_compromissos = NULL;
    } else {
        atividade *temp = (atividade *)realloc(*lista_compromissos,*quantidade * sizeof(atividade));
        if(temp!=NULL) {
            *lista_compromissos = temp;
        }
    }
    return 1;
}
int verificar_atividade(atividade compromisso,int opcao_filtro) {
    time_t tempo = time(NULL);
    struct tm *agora = localtime(&tempo);

    int dia_hoje = (*agora).tm_mday;
    int mes_hoje = (*agora).tm_mon + 1;
    int ano_hoje = (*agora).tm_year + 1900;
    Data d = compromisso.data;
    Hora h = compromisso.horario_inicial;

    switch (opcao_filtro) {
        case 1: {//hoje
            if(d.ano == ano_hoje && d.mes == mes_hoje && d.dia == dia_hoje) {
                return 1;
            }
            break;
        }
        case 2:{//restante do dia
            if(d.ano == ano_hoje && d.mes==mes_hoje && d.dia == dia_hoje) {
                if(h.hora > (*agora).tm_hour) {
                    return 1;
                }
                if(h.hora == (*agora).tm_hour && h.minuto >= (*agora).tm_min) {
                    return 1;
                }
            }
            break;
        }
        case 3:{//restante da semana
            struct tm data_atividade = {0};
            data_atividade.tm_year = d.ano - 1900;
            data_atividade.tm_mon = d.mes - 1;
            data_atividade.tm_mday = d.dia;
            time_t segundo_atividade = mktime(&data_atividade);

            int dias_sabado = 6 - (*agora).tm_wday;
            struct tm data_limite = *agora;
            data_limite.tm_mday += dias_sabado;
            time_t segundo_limite = mktime(&data_limite);

            struct tm hoje_zerado = *agora;
            hoje_zerado.tm_hour=0;
            hoje_zerado.tm_min=0;
            hoje_zerado.tm_sec=0;
            time_t segundo_hoje = mktime(&hoje_zerado);
            if(segundo_atividade >= segundo_hoje && segundo_atividade <= segundo_limite) {
                return 1;
            }
            break;
        }
        case 4:{//restante do mes
            if(d.ano == ano_hoje && d.mes == mes_hoje && d.dia>= dia_hoje) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

int salvar_arquivos(contato *lista_contato,int quantidade_contato,atividade *lista_atividade,int quantidade_atividade) {
    if (quantidade_contato == 0 && quantidade_atividade == 0) {
        // remove() deleta o arquivo do disco
        remove("dados_agenda.bin"); 
        return 1;
    }
    
    FILE *arquivo = fopen("dados_agenda.bin", "wb");
    if(arquivo==NULL) {
        return -1;
    }
    fwrite(&quantidade_contato,sizeof(int),1,arquivo);
    for(int i=0;i<quantidade_contato;i++) {
        fwrite(lista_contato[i].nome,sizeof(char),50,arquivo);
        fwrite(lista_contato[i].sobrenome,sizeof(char),100,arquivo);
        fwrite(lista_contato[i].telefone,sizeof(char),20,arquivo);
        fwrite(lista_contato[i].email,sizeof(char),100,arquivo);
        fwrite(lista_contato[i].local_trabalho,sizeof(char),100,arquivo);
    }

    fwrite(&quantidade_atividade,sizeof(int),1,arquivo);
    for(int i=0;i<quantidade_atividade;i++) {
        fwrite(lista_atividade[i].descricao,sizeof(char),200,arquivo);
        fwrite(&lista_atividade[i].data,sizeof(Data),1,arquivo);
        fwrite(&lista_atividade[i].horario_inicial,sizeof(Hora),1,arquivo);
        fwrite(&lista_atividade[i].tipo,sizeof(tipo_atividade),1,arquivo);
        fwrite(&lista_atividade[i].quantidade_participantes,sizeof(int),1,arquivo);
        //salva os nomes um por um
        for(int j=0;j<lista_atividade[i].quantidade_participantes;j++) {
            fwrite(lista_atividade[i].participantes[j],sizeof(char),50,arquivo);
        }
    }
    fclose(arquivo);
    return 1;
}
int carregar_arquivos(contato **lista_contato,int *quantidade_contato,atividade **lista_atividade,int *quantidade_atividade) {
    FILE *arquivo = fopen("dados_agenda.bin","rb");
    if(arquivo==NULL) {
        *quantidade_contato=0;
        *quantidade_atividade=0;
        *lista_contato=NULL;
        *lista_atividade=NULL;
        return 0;
    }
    fread(quantidade_contato,sizeof(int),1,arquivo);
    if(*quantidade_contato>0) {
        *lista_contato = (contato *)malloc((*quantidade_contato) * sizeof(contato));
        if(*lista_contato == NULL) {
            fclose(arquivo);
            return 0;
        }
        for(int i = 0;i < *quantidade_contato;i++) {
            fread((*lista_contato)[i].nome, sizeof(char), 50, arquivo);
            fread((*lista_contato)[i].sobrenome, sizeof(char), 100, arquivo);
            fread((*lista_contato)[i].telefone, sizeof(char), 20, arquivo);
            fread((*lista_contato)[i].email, sizeof(char), 100, arquivo);
            fread((*lista_contato)[i].local_trabalho, sizeof(char), 100, arquivo);
        }
    } else {
        *lista_contato = NULL;
    }

    fread(quantidade_atividade,sizeof(int),1,arquivo);
    if(*quantidade_atividade>0) {
        *lista_atividade = (atividade *)malloc((*quantidade_atividade) * sizeof(atividade));
        if(*lista_atividade == NULL) {
            *lista_atividade = NULL;
            *quantidade_atividade = 0;
            fclose(arquivo);
            return 0;
        }
        for(int i = 0; i < *quantidade_atividade; i++) {
            fread((*lista_atividade)[i].descricao, sizeof(char), 200, arquivo);
            fread(&(*lista_atividade)[i].data, sizeof(Data), 1, arquivo);
            fread(&(*lista_atividade)[i].horario_inicial, sizeof(Hora), 1, arquivo);
            fread(&(*lista_atividade)[i].tipo, sizeof(tipo_atividade), 1, arquivo);
        
            (*lista_atividade)[i].participantes = NULL;
            (*lista_atividade)[i].quantidade_participantes = 0;

            int qtd_part;
            fread(&qtd_part, sizeof(int), 1, arquivo);
            
            //ler cada participante
            for(int j = 0; j < qtd_part; j++) {
                char nome_temp[50];
                fread(nome_temp, sizeof(char), 50, arquivo);
                adicionar_participante(&(*lista_atividade)[i], nome_temp);
            }
        }
    } else {
        *lista_atividade = NULL;
    }
    fclose(arquivo);
    return 1;
}

int adicionar_participante(atividade *reuniao,char *nome_participante) {
    ((*reuniao).quantidade_participantes)++;
    char **temp = (char **)realloc((*reuniao).participantes,(*reuniao).quantidade_participantes * sizeof(char *));
    if(temp==NULL) {
        (*reuniao).quantidade_participantes--;
        return -1;
    }
    (*reuniao).participantes = temp;
    (*reuniao).participantes[(*reuniao).quantidade_participantes - 1] = (char *)malloc(50 * sizeof(char));
    if((*reuniao).participantes[(*reuniao).quantidade_participantes - 1] == NULL) {
        (*reuniao).quantidade_participantes--;
        return -1;
    }
    strcpy((*reuniao).participantes[(*reuniao).quantidade_participantes - 1],nome_participante);
    return 1;
}