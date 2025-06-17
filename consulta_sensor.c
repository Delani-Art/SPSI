#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include "utils.h"

#define MAX_LEITURAS 10000

typedef struct {
    time_t timestamp;
    char id_sensor[32];
    char valor[64];
} Leitura;

int busca_binaria_decrescente(Leitura* leituras, int n, time_t alvo) {
    int inicio = 0, fim = n - 1;
    int melhor = -1;
    long menor_dif = LONG_MAX;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        long diff = labs(leituras[meio].timestamp - alvo);

        if (diff < menor_dif) {
            menor_dif = diff;
            melhor = meio;
        }

        if (leituras[meio].timestamp > alvo)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }

    return melhor;
}

void exibir_data_legivel(time_t ts) {           // Converte um timestamp para data legível
    char data[64];
    struct tm* tm_info = localtime(&ts);
    strftime(data, sizeof(data), "%d/%m/%Y %H:%M:%S", tm_info);
    printf("Data legível: %s\n", data);
}

int eh_numero(const char* s) {                  // Verifica se a string é um número (simples validação para timestamp)
    for (int i = 0; s[i] != '\0'; i++) {
        if (!isdigit((unsigned char)s[i])) return 0;
    }
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {                            // Verifica se foram passados os argumentos (sensor e timestamp)
        printf("Uso: %s <nome_do_sensor> <timestamp>\n", argv[0]);
        return 1;
    }

    if (!eh_numero(argv[2])) {                   // Validação básica do timestamp
        printf("Erro: o timestamp deve ser um número inteiro positivo.\n");
        return 1;
    }

    char nome_arquivo[64];                        // Monta o nome do arquivo do sensor
    snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.dat", argv[1]);
    time_t alvo = atoll(argv[2]);

    FILE* arquivo = fopen(nome_arquivo, "r");    // Tenta abrir o arquivo .dat do sensor
    if (!arquivo) {
        perror("Erro ao abrir arquivo do sensor");
        return 1;
    }

    Leitura leituras[MAX_LEITURAS];        
    int total = 0;
    int linha = 1;

    while (fscanf(arquivo, "%ld %31s %63s",       // Leitura segura do arquivo
                  &leituras[total].timestamp, 
                  leituras[total].id_sensor, 
                  leituras[total].valor) == 3) {

        total++;
        if (total >= MAX_LEITURAS) {
            printf("Limite de %d leituras atingido.\n", MAX_LEITURAS);
            break;
        }
        linha++;
    }

    if (!feof(arquivo)) {
        printf("Erro na leitura do arquivo na linha %d. Verifique se o formato está correto.\n", linha);
        fclose(arquivo);
        return 1;
    }

    fclose(arquivo);

    int indice = busca_binaria_decrescente(leituras, total, alvo);    // Executa busca binária para encontrar leitura mais próxima
    if (indice >= 0) {
        Leitura l = leituras[indice];
        printf("Leitura mais próxima:\n");
        printf("Timestamp: %ld\n", l.timestamp);
        exibir_data_legivel(l.timestamp);
        printf("Sensor: %s\n", l.id_sensor);
        printf("Valor: %s\n", l.valor);
    } else {
        printf("Nenhuma leitura encontrada.\n");
    }

    return 0;
}
