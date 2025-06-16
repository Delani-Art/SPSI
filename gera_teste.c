#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

int tipo_por_string(const char* tipo_str) {
    if (strcmp(tipo_str, "CONJ_Z") == 0) return 0;      // Inteiro
    if (strcmp(tipo_str, "BINARIO") == 0) return 1;     // Booleano
    if (strcmp(tipo_str, "CONJ_Q") == 0) return 2;      // Float
    if (strcmp(tipo_str, "TEXTO") == 0) return 3;       // String
    return -1;                                          // inválido
}

void gerar_dados(FILE* f, char* sensor, int tipo, time_t inicio, time_t fim) {
    for (int i = 0; i < 2000; i++) {
        time_t ts = inicio + rand() % (fim - inicio + 1);
        char valor[64];

        switch (tipo) {
            case 0: sprintf(valor, "%d", rand() % 100); break;                      // Inteiro
            case 1: sprintf(valor, "%s", (rand() % 2) ? "true" : "false"); break;   // Booleano
            case 2: sprintf(valor, "%.2f", (rand() % 1000) / 10.0); break;          // Float
            case 3: sprintf(valor, "cod%04d", rand() % 10000); break;               // String
        }

        fprintf(f, "%ld %s %s\n", ts, sensor, valor);
    }
}

struct tm converter_data(char* data_str) {      // Converte data no formato DD/MM/AAAA para struct tm
    struct tm t = {0};
    int dia, mes, ano;
    sscanf(data_str, "%d/%d/%d", &dia, &mes, &ano);
    t.tm_mday = dia;
    t.tm_mon = mes - 1;
    t.tm_year = ano - 1900;
    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_isdst = -1;
    return t;
}

int main(int argc, char* argv[]) {     // Verifica número de argumentos
    if (argc < 5 || (argc - 3) % 2 != 0) {
        printf("Uso: %s DD/MM/AAAA DD/MM/AAAA sensor tipo [...]\n", argv[0]);
        printf("Tipos válidos: CONJ_Z, CONJ_Q, BINARIO, TEXTO\n");
        return 1;
    }

    srand(time(NULL));  // Inicializa aleatoriedade

    // Converte datas de início e fim
    struct tm inicio_tm = converter_data(argv[1]);
    struct tm fim_tm = converter_data(argv[2]);
    time_t inicio = mktime(&inicio_tm);
    time_t fim = mktime(&fim_tm);

    if (inicio == -1 || fim == -1 || inicio > fim) {
        printf("Erro na conversão das datas.\n");
        return 1;
    }
    
    FILE* f = fopen("teste.txt", "w");
    if (!f) {
        perror("Erro ao criar arquivo teste.txt");
        return 1;
    }

    for (int i = 3; i < argc; i += 2) {
        char* sensor = argv[i];
        char* tipo_str = argv[i + 1];
        int tipo = tipo_por_string(tipo_str);

        if (tipo == -1) {
            printf("Tipo inválido para o sensor %s: %s\n", sensor, tipo_str);
            continue;
        }

        printf("Gerando dados para sensor %s (%s)...\n", sensor, tipo_str);
        gerar_dados(f, sensor, tipo, inicio, fim);
    }

    fclose(f);
    printf("Arquivo teste.txt gerado com sucesso.\n");
    return 0;
}
