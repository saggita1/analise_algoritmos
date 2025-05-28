// algoritmos/merge_sort.c
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

void merge(int *arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        comparacoes++;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    free(L);
    free(R);
}

void merge_sort_rec(int *arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        merge_sort_rec(arr, left, mid);
        merge_sort_rec(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void merge_sort(int *arr, int n) {
    merge_sort_rec(arr, 0, n - 1);
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
    merge_sort(arr, n);
    clock_t fim = clock();
    
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    fprintf(log_fp, "MergeSort,%s,%d,%lld,%.6f\n", 
            nome_base, n, comparacoes, tempo);
    fflush(log_fp);
    
    printf("Merge Sort | Arquivo: %s | Comparações: %lld | Tempo: %.6fs\n", 
           nome_base, comparacoes, tempo);
    
    fclose(log_fp);
    free(arr);
    return 0;
}