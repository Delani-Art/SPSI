#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "utils.h"

typedef struct {
    time_t timestamp;    // Tempo da leitura 
    char id_sensor[32];  // sensor
    char valor[64];      // Valor lido
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

int main(int argc, char* argv[]) {
    if (argc != 3) {    // Verifica se foram passados os argumentos (sensor e timestamp)
        printf("Uso: %s sensor timestamp\n", argv[0]);
        return 1;
    }

    char nome_arquivo[64];
    sprintf(nome_arquivo, "%s.dat", argv[1]);
    time_t alvo = atoll(argv[2]);    // Converte o argumento timestamp para time_t

    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo");
        return 1;
    }
    
    Leitura leituras[10000];    // Lê todas as leituras do sensor para a memória
    int total = 0;

    while (fscanf(arquivo, "%ld %s %s", &leituras[total].timestamp, leituras[total].id_sensor, leituras[total].valor) == 3) {
        total++;    // Conta quantas leituras foram carregadas
    }
    fclose(arquivo);

    int indice = busca_binaria_decrescente(leituras, total, alvo);
    if (indice >= 0) {
        printf("Leitura mais próxima:\n%ld %s %s\n", leituras[indice].timestamp, leituras[indice].id_sensor, leituras[indice].valor);
    } else {
        printf("Nenhuma leitura encontrada.\n");
    }

    return 0;
}
