#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

#define MAX_SENSORES 100
#define MAX_LINHA 256
#define MAX_LEITURAS 10000

typedef enum { INT, BOOL, FLOAT, STRING } TipoSensor;    // Tipos possíveis de sensores

typedef struct {
    time_t timestamp;
    char id_sensor[32];
    char valor[64];
} Leitura;

typedef struct {
    char id_sensor[32];
    TipoSensor tipo;
} SensorTipo;

SensorTipo tipos_sensores[MAX_SENSORES] = {    // "Banco de dados" com tipos de sensores pré-definidos
    {"temp01", FLOAT},
    {"press01", INT},
    {"hum01", FLOAT},
    {"door01", BOOL},
    {"cod01", STRING},
};

int detectar_tipo(const char* id_sensor) {               // Busca o tipo de sensor pelo nome
    for (int i = 0; i < MAX_SENSORES; i++) {
        if (strcmp(tipos_sensores[i].id_sensor, id_sensor) == 0) {
            return tipos_sensores[i].tipo;
        }
    }
    return -1;
}

int comparar_leitura(const void* a, const void* b) {    // Função usada pelo qsort para ordenar por timestamp descrescente
    time_t ta = ((Leitura*)a)->timestamp;
    time_t tb = ((Leitura*)b)->timestamp;
    return (tb > ta) - (tb < ta);                         // evita problemas de overflow
}

void processar_arquivo(const char* nome_arquivo) {        // Função principal que lê o arquivo, separa por sensor e ordena
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo");
        return;
    }

    Leitura leituras[MAX_LEITURAS];
    int total = 0;
    char linha[MAX_LINHA];
    int num_linha = 1;

    // Lê linha por linha com validação
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (total >= MAX_LEITURAS) {
            printf("Limite máximo de leituras (%d) atingido.\n", MAX_LEITURAS);
            break;
        }

        Leitura temp;
        int lidos = sscanf(linha, "%ld %31s %63s", &temp.timestamp, temp.id_sensor, temp.valor);
        if (lidos != 3) {
            printf("Linha %d inválida (esperado: <timestamp> <sensor> <valor>): %s", num_linha, linha);
        } else {
            leituras[total++] = temp;
        }

        num_linha++;
    }

    fclose(arquivo);

    for (int i = 0; i < MAX_SENSORES; i++) {
        int count = 0;
        Leitura filtradas[MAX_LEITURAS];

        for (int j = 0; j < total; j++) {
            if (strcmp(tipos_sensores[i].id_sensor, leituras[j].id_sensor) == 0) {
                filtradas[count++] = leituras[j];
            }
        }

        if (count == 0) continue;

        qsort(filtradas, count, sizeof(Leitura), comparar_leitura);        // Ordena as leituras por timestamp

        char nome_saida[64];
        snprintf(nome_saida, sizeof(nome_saida), "%s.dat", tipos_sensores[i].id_sensor);

        FILE* saida = fopen(nome_saida, "w");
        if (!saida) {
            printf("Erro ao criar arquivo %s\n", nome_saida);
            continue;
        }

        for (int k = 0; k < count; k++) {
            fprintf(saida, "%ld %s %s\n",
                    filtradas[k].timestamp,
                    filtradas[k].id_sensor,
                    filtradas[k].valor);
        }

        fclose(saida);
    }
}

// Programa principal
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }

    processar_arquivo(argv[1]);
    return 0;
}
