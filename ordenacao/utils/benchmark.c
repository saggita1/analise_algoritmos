// utils/benchmark.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

FILE *csv_fp = NULL;

void cleanup(int sig) {
    if (csv_fp) {
        fflush(csv_fp);
        fclose(csv_fp);
    }
    printf("\nBenchmark interrompido. Resultados salvos em resultados/benchmark.csv\n");
    exit(0);
}

void inicializar_csv() {
    csv_fp = fopen("resultados/benchmark.csv", "w");
    if (!csv_fp) {
        perror("Erro ao criar arquivo CSV");
        exit(1);
    }
    fprintf(csv_fp, "Algoritmo,Arquivo,Tamanho,Comparacoes,Tempo\n");
    fflush(csv_fp);
    fclose(csv_fp);
}

void executar_teste(char *algoritmo, char *arquivo_entrada) {
    char comando[500];
    snprintf(comando, sizeof(comando), "./%s %s resultados/benchmark.csv", 
             algoritmo, arquivo_entrada);
    
    printf("Executando: %s com %s\n", algoritmo, arquivo_entrada);
    int result = system(comando);
    
    if (result != 0) {
        printf("Erro ao executar %s\n", algoritmo);
    }
}

int main() {
    signal(SIGINT, cleanup);
    
    char *algoritmos[] = {"bubble", "insertion", "merge", "quick"};
    int num_algoritmos = 4;
    
    char *tipos[] = {"ordenado", "aleatorio", "decrescente"};
    int num_tipos = 3;
    
    int tamanhos[] = {1000, 10000, 100000, 500000, 1000000};
    int num_tamanhos = 5;
    
    printf("=== Iniciando Benchmark de Algoritmos de Ordenação ===\n");
    printf("Resultados serão salvos em resultados/benchmark.csv\n");
    printf("Pressione Ctrl+C para interromper e salvar resultados parciais\n\n");
    
    // Inicializar CSV
    inicializar_csv();
    
    int total_testes = num_algoritmos * num_tipos * num_tamanhos;
    int teste_atual = 0;
    
    for (int a = 0; a < num_algoritmos; a++) {
        for (int t = 0; t < num_tipos; t++) {
            for (int s = 0; s < num_tamanhos; s++) {
                teste_atual++;
                
                char arquivo_entrada[200];
                snprintf(arquivo_entrada, sizeof(arquivo_entrada), 
                        "entradas/entrada_%d_%s.txt", tamanhos[s], tipos[t]);
                
                printf("\n[%d/%d] ", teste_atual, total_testes);
                executar_teste(algoritmos[a], arquivo_entrada);
                
                // Pequena pausa para não sobrecarregar o sistema
                sleep(1); // 1 segundo
            }
        }
    }
    
    printf("\n=== Benchmark Concluído! ===\n");
    printf("Resultados salvos em: resultados/benchmark.csv\n");
    printf("Execute 'make graficos' para gerar os gráficos\n");
    
    return 0;
}