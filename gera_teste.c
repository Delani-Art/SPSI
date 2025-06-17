#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "utils.h"

                                                        // Converte string para código de tipo:
int tipo_por_string(const char* tipo_str) {
    if (strcmp(tipo_str, "CONJ_Z") == 0) return 0;      // Inteiro
    if (strcmp(tipo_str, "BINARIO") == 0) return 1;     // Booleano
    if (strcmp(tipo_str, "CONJ_Q") == 0) return 2;      // Float
    if (strcmp(tipo_str, "TEXTO") == 0) return 3;       // String
    return -1;
}


void gerar_dados(FILE* f, const char* sensor, int tipo, time_t inicio, time_t fim) {    // Gera os dados aleatórios
    for (int i = 0; i < 2000; i++) {
        time_t ts = inicio + rand() % (fim - inicio + 1);
        char valor[64];

        switch (tipo) {
            case 0: sprintf(valor, "%d", rand() % 100); break;
            case 1: sprintf(valor, "%s", (rand() % 2) ? "true" : "false"); break;
            case 2: sprintf(valor, "%.2f", (rand() % 1000) / 10.0); break;
            case 3: sprintf(valor, "cod%04d", rand() % 10000); break;
            default:
                fprintf(stderr, "Tipo inválido na geração de %s\n", sensor);
                continue;
        }

        fprintf(f, "%ld %s %s\n", ts, sensor, valor);
    }
}

int converter_data(char* data_str, struct tm* t) {        // Converte data no formato DD/MM/AAAA para struct tm
    int dia, mes, ano;
    if (sscanf(data_str, "%d/%d/%d", &dia, &mes, &ano) != 3) {
        return 0;
    }
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || ano < 1970) {
        return 0;
    }
    memset(t, 0, sizeof(struct tm));
    t->tm_mday = dia;
    t->tm_mon = mes - 1;
    t->tm_year = ano - 1900;
    t->tm_hour = 0;
    t->tm_min = 0;
    t->tm_sec = 0;
    t->tm_isdst = -1;
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc < 5 || (argc - 3) % 2 != 0) {
        printf("Uso: %s DD/MM/AAAA DD/MM/AAAA sensor tipo [...]\n", argv[0]);
        printf("Tipos válidos: CONJ_Z, CONJ_Q, BINARIO, TEXTO\n");
        return 1;
    }

    srand((unsigned int) time(NULL));

    struct tm inicio_tm, fim_tm;            // Valida e converte datas
    if (!converter_data(argv[1], &inicio_tm) || !converter_data(argv[2], &fim_tm)) {
        printf("Erro: formato de data inválido. Use DD/MM/AAAA.\n");
        return 1;
    }

    time_t inicio = mktime(&inicio_tm);
    time_t fim = mktime(&fim_tm);
    if (inicio == -1 || fim == -1 || inicio >= fim) {
        printf("Erro na conversão das datas: intervalo inválido.\n");
        return 1;
    }

    FILE* f = fopen("teste.txt", "w");        // Abre arquivo de saída
    if (!f) {
        perror("Erro ao criar o arquivo teste.txt");
        return 1;
    }

    int sensores_validos = 0;                    // Processa pares sensor/tipo
    for (int i = 3; i < argc; i += 2) {
        const char* sensor = argv[i];
        const char* tipo_str = argv[i + 1];
        int tipo = tipo_por_string(tipo_str);

        if (tipo == -1) {
            printf("Erro: tipo inválido para o sensor %s: %s\n", sensor, tipo_str);
            continue;
        }

        printf("Gerando dados para sensor %s (%s)...\n", sensor, tipo_str);
        gerar_dados(f, sensor, tipo, inicio, fim);
        sensores_validos++;
    }

    fclose(f);

    if (sensores_validos == 0) {
        printf("Nenhum dado foi gerado. Nenhum sensor com tipo válido informado.\n");
        remove("teste.txt");
        return 1;
    }

    printf("Arquivo teste.txt gerado com sucesso.\n");
    return 0;
}
