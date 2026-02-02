#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "agenda.h"

int main(void) {
    contato *contatos = NULL;
    int quantidade_contatos=0;
    atividade *atividades = NULL;
    int quantidade_atividades=0;
    if(carregar_arquivos(&contatos,&quantidade_contatos,&atividades,&quantidade_atividades)) {
        printf("arquivo salvo encontrado!\n");
        if(quantidade_atividades>0) {
            int alerta=0;
            printf("\n===================================================\n");
            printf("                    ALERTAS DO DIA              \n");
            printf("======================================================\n");
            for(int i=0;i<quantidade_atividades;i++) {
                if(verificar_atividade(atividades[i],2)==1) {
                    printf("! [%02dh%02d]: %s\n",
                    atividades[i].horario_inicial.hora,
                    atividades[i].horario_inicial.minuto,
                    atividades[i].descricao);
                    alerta = 1;
                }
            }
            if(!alerta) {
                printf("nenhuma atividade para o restante do dia!\n");
            }
        }
    } else {
        printf("iniciando uma nova agenda!\n");
    }
    int opcao,sair=0;
    while(!sair) {
        printf("=========================Agenda Plus=======================\n");
        printf("[1] adicionar contato\n");
        printf("[2] deletar contato\n");
        printf("[3] adicionar atividade\n");
        printf("[4] deletar atividade\n");
        printf("[5] consulta rapida\n");
        printf("[6] sair\n");
        scanf("%d",&opcao);
        getchar();
        switch (opcao) {
            case 1: {
                contato novo;

                printf("Digite o nome do contato: ");
                fgets(novo.nome, 50, stdin);
                novo.nome[strcspn(novo.nome, "\n")] = '\0';
                printf("Digite o sobrenome do contato: ");
                fgets(novo.sobrenome, 100, stdin);
                novo.sobrenome[strcspn(novo.sobrenome, "\n")] = '\0';
                printf("Digite o telefone do contato: ");
                fgets(novo.telefone, 20, stdin);
                novo.telefone[strcspn(novo.telefone, "\n")] = '\0';
                printf("Digite o email do contato: ");
                fgets(novo.email, 100, stdin);
                novo.email[strcspn(novo.email, "\n")] = '\0';
                printf("Digite o local de trabalho do contato: ");
                fgets(novo.local_trabalho, 100, stdin);
                novo.local_trabalho[strcspn(novo.local_trabalho, "\n")] = '\0';
                //tenta adicionar
                int resultado = adicionar_contato(&contatos, &quantidade_contatos, novo);
                if(resultado == 1) {
                    printf("contato adicionado com sucesso!\n");
                } else if(resultado == 0) {
                    printf("ja existe um contato com este email!\n");
                } else {
                    printf("erro de realocação de memória!\n");
                }
                break;
            }  
            case 2: {
                char nome_deletar[50];
                printf("digite o nome do contato que voce quer deletar: ");
                fgets(nome_deletar,50,stdin);
                nome_deletar[strcspn(nome_deletar,"\n")] = '\0';
                int resultado = deletar_contato(&contatos,&quantidade_contatos,nome_deletar);
                if(resultado == -1) {
                    printf("a agenda esta vazia!\n");
                } else if (resultado==1) {
                    printf("contato deletado!\n");
                } else {
                    printf("%s nao achado em contatos da agenda!\n",nome_deletar);
                }
                break;
            }
            case 3: {
                if(adicionar_atividade(&atividades, &quantidade_atividades)==1) {
                    int indice = quantidade_atividades - 1;
                    printf("[1] reuniao \n[2] trabalho individual \n[3] atividade de relaxamento \n");
                    int tipo_temp;
                    scanf("%d", &tipo_temp);
                    atividades[indice].tipo = tipo_temp;
                    getchar();

                    printf("fale sobre a atividade a ser feita: \n");
                    fgets(atividades[indice].descricao, 200, stdin);
                    atividades[indice].descricao[strcspn(atividades[indice].descricao, "\n")] = '\0';
                    
                    printf("digite o dia: ");
                    scanf("%d", &atividades[indice].data.dia);
                    getchar();
                    
                    printf("digite o mes: ");
                    scanf("%d", &atividades[indice].data.mes);
                    getchar();
                    
                    printf("digite o ano: ");
                    scanf("%d", &atividades[indice].data.ano);
                    getchar();
                    
                    printf("digite a hora da atividade: ");
                    scanf("%d", &atividades[indice].horario_inicial.hora);
                    getchar();
                    
                    printf("digite os minutos da atividade: ");
                    scanf("%d", &atividades[indice].horario_inicial.minuto);
                    getchar();
                    
                    if(atividades[indice].tipo == 1) {
                        char continuar = 's';
                        char nome_temp[50];
                        
                        while(continuar == 's') {
                            printf("digite o nome do participante: ");
                            fgets(nome_temp, 50, stdin);
                            nome_temp[strcspn(nome_temp, "\n")] = '\0';
                            
                            if(busca_contatos(contatos, quantidade_contatos, nome_temp) != -1) {
                                if(adicionar_participante(&atividades[indice], nome_temp)) {
                                    printf("participante adicionado!\n");
                                } else {
                                    printf("erro de realocacao em adicionar participantes!\n");
                                }
                            } else {
                                char adicionar = 'n';
                                printf("o contato nao existe em sua agenda!\n");
                                printf("adicionar? [s/n]\n");
                                scanf(" %c", &adicionar);
                                getchar();
                                
                                if(adicionar == 's') {
                                    contato novo;
                                    strcpy(novo.nome, nome_temp);
                                    printf("digite o sobrenome do contato: ");
                                    fgets(novo.sobrenome, 100, stdin);
                                    novo.sobrenome[strcspn(novo.sobrenome, "\n")] = '\0';
                                    printf("digite o telefone do contato: ");
                                    fgets(novo.telefone, 20, stdin);
                                    novo.telefone[strcspn(novo.telefone, "\n")] = '\0';
                                    printf("digite o email do contato: ");
                                    fgets(novo.email, 100, stdin);
                                    novo.email[strcspn(novo.email, "\n")] = '\0';
                                    printf("digite o local de trabalho do contato: ");
                                    fgets(novo.local_trabalho, 100, stdin);
                                    novo.local_trabalho[strcspn(novo.local_trabalho, "\n")] = '\0';
                                    
                                    if(adicionar_contato(&contatos, &quantidade_contatos, novo) == 1) {
                                        printf("contato adicionado com sucesso!\n");
                                        if(adicionar_participante(&atividades[indice], nome_temp)) {
                                            printf("participante adicionado a reuniao!\n");
                                        }
                                    } else {
                                        printf("erro ao adicionar contato!\n");
                                    }
                                }
                            }
                            
                            printf("adicionar mais participantes? [s/n]\n");
                            scanf(" %c", &continuar);
                            getchar();
                        }
                    }
                    if (atividades[indice].tipo == 1 && atividades[indice].quantidade_participantes == 0) {
                        printf("\numa Reuniao precisa ter pelo menos 1 participante!\n");
                        printf("a atividade foi descartada\n");
                        deletar_atividade(&atividades, &quantidade_atividades, indice);
                    } else {
                        printf("atividade adicionada com sucesso!\n");
                    }
                } else {
                    printf("falha ao alocar memoria para atividade!\n");
                }
                break;
            }
            case 4: {
                if(quantidade_atividades == 0) {
                    printf("nenhuma atividade cadastrada!\n");
                } else {
                    printf("\n====================atividades na agenda===============\n");
                    for(int i=0;i<quantidade_atividades;i++) {
                        printf("[%d] %s - %02d/%02d/%d as %02d:%02d\n",
                        i+1,
                        atividades[i].descricao,
                        atividades[i].data.dia,
                        atividades[i].data.mes,
                        atividades[i].data.ano,
                        atividades[i].horario_inicial.hora,
                        atividades[i].horario_inicial.minuto);
                    }
                    int escolha;
                    printf("digite o numero da atividade que oce quer deletar[0 para sair]: ");
                    scanf("%d",&escolha);
                    getchar();
                    if(escolha==0) {
                        printf("operacao cancelada!\n");
                    } else if(escolha < 1 || escolha > quantidade_atividades) {
                        printf("opcao invalida!\n");
                    } else {
                        int resultado = deletar_atividade(&atividades,&quantidade_atividades,escolha-1);
                        if(resultado==1) {
                            printf("atividade deletada com sucesso!\n");
                        } else {
                            printf("erro ao deletar atividade!\n");
                        }
                    }
                }
                break;
            }
        case 5: {
            if(quantidade_atividades == 0) {
                printf("nenhuma atividade cadastrada!\n");
            } else {
                printf("\n======consulta rapida=======\n");
                printf("[1] atividade de hoje\n");
                printf("[2] atividades restantes do dia\n");
                printf("[3] atividades da semana\n");
                printf("[4] atividades do mes\n");
                int filtro;
                scanf("%d",&filtro);
                getchar();
                printf("\n========resultados da busca=========\n");
                int encontrou_agum = 0;
                for(int i=0;i<quantidade_atividades;i++) {
                    if(verificar_atividade(atividades[i],filtro)==1) {
                        encontrou_agum = 1;
                        printf("[%d] %s (Tipo: %d)\n", i+1, atividades[i].descricao, atividades[i].tipo);
                        printf("data: %02d/%02d/%d as %02d:%02d\n", 
                               atividades[i].data.dia, 
                               atividades[i].data.mes, 
                               atividades[i].data.ano,
                               atividades[i].horario_inicial.hora, 
                               atividades[i].horario_inicial.minuto);
                        if(atividades[i].quantidade_participantes > 0) {
                            printf("participantes: ");
                            for(int j = 0;j<atividades[i].quantidade_participantes;j++) {
                                printf("%s",atividades[i].participantes[j]);
                                if(j<atividades[i].quantidade_participantes - 1) {
                                    printf(", ");
                                }
                            }
                            printf("\n");
                        }
                    }
                }
                if(encontrou_agum == 0) {
                    printf("nenhum compromisso encontrado!\n");
                }
            }
            break;
        }
        case 6:{
            printf("salvando....\n");
            if(salvar_arquivos(contatos,quantidade_contatos,atividades,quantidade_atividades)==1) {
                printf("dados salvos!\n");
            } else {
                printf("erro ao salvar os dados!\n");
            }
            if(contatos != NULL) {
                free(contatos);
            }
            if(atividades != NULL) {
                for(int i=0;i<quantidade_atividades;i++) {
                    if(atividades[i].participantes != NULL) {
                        for(int j=0;j<atividades[i].quantidade_participantes;j++) {
                            free(atividades[i].participantes[j]);
                        }
                        free(atividades[i].participantes);
                    }
                }
                free(atividades);
            }
            printf("te vejo em breve!\n");
            sair = 1;
            break;
        }
        default:
            printf("operacao invalida!\n");
        }
    }
    return 0;
}