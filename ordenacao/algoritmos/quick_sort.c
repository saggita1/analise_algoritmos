// algoritmos/quick_sort.c
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

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int *arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    
    for (int j = low; j <= high - 1; j++) {
        comparacoes++;
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quick_sort_rec(int *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        
        quick_sort_rec(arr, low, pi - 1);
        quick_sort_rec(arr, pi + 1, high);
    }
}

void quick_sort(int *arr, int n) {
    quick_sort_rec(arr, 0, n - 1);
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
    quick_sort(arr, n);
    clock_t fim = clock();
    
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    fprintf(log_fp, "QuickSort,%s,%d,%lld,%.6f\n", 
            nome_base, n, comparacoes, tempo);
    fflush(log_fp);
    
    printf("Quick Sort | Arquivo: %s | Comparações: %lld | Tempo: %.6fs\n", 
           nome_base, comparacoes, tempo);
    
    fclose(log_fp);
    free(arr);
    return 0;
}