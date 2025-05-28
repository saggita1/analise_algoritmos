// algoritmos/insertion_sort.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <string.h>

long long comparacoes = 0;
FILE *log_fp = NULL;

void salvar_log_emergencia(int sig) {
    if (log_fp) {
        fflush(log_fp);
        fclose(log_fp);
    }
    printf("\nLog salvo antes da interrupção.\n");
    exit(0);
}

void insertion_sort(int *arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0) {
            comparacoes++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
}

int* ler_arquivo(char *nome_arquivo, int *tamanho) {
    FILE *fp = fopen(nome_arquivo, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }
    
    int capacidade = 1000;
    int *vetor = malloc(sizeof(int) * capacidade);
    int valor, count = 0;
    
    while (fscanf(fp, "%d", &valor) == 1) {
        if (count >= capacidade) {
            capacidade *= 2;
            vetor = realloc(vetor, sizeof(int) * capacidade);
        }
        vetor[count++] = valor;
    }
    fclose(fp);
    *tamanho = count;
    return vetor;
}

int main(int argc, char *argv[]) {
    signal(SIGINT, salvar_log_emergencia);
    
    if (argc != 3) {
        printf("Uso: %s arquivo_entrada.txt arquivo_log.csv\n", argv[0]);
        return 1;
    }
    
    int n;
    int *arr = ler_arquivo(argv[1], &n);
    
    log_fp = fopen(argv[2], "a");
    if (!log_fp) {
        perror("Erro ao abrir log");
        free(arr);
        return 1;
    }
    
    char *nome_base = strrchr(argv[1], '/');
    nome_base = nome_base ? nome_base + 1 : argv[1];
    
    clock_t inicio = clock();
    comparacoes = 0;
    insertion_sort(arr, n);
    clock_t fim = clock();
    
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    fprintf(log_fp, "InsertionSort,%s,%d,%lld,%.6f\n", 
            nome_base, n, comparacoes, tempo);
    fflush(log_fp);
    
    printf("Insertion Sort | Arquivo: %s | Comparações: %lld | Tempo: %.6fs\n", 
           nome_base, comparacoes, tempo);
    
    fclose(log_fp);
    free(arr);
    return 0;
}