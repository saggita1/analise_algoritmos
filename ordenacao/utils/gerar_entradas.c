// utils/gerar_entradas.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void gerar_ordenado(int tamanho, char* nome_arquivo) {
    FILE *fp = fopen(nome_arquivo, "w");
    if (!fp) {
        perror("Erro ao criar arquivo");
        exit(1);
    }
    
    for (int i = 1; i <= tamanho; i++) {
        fprintf(fp, "%d\n", i);
    }
    fclose(fp);
    printf("Gerado: %s\n", nome_arquivo);
}

void gerar_aleatorio(int tamanho, char* nome_arquivo) {
    FILE *fp = fopen(nome_arquivo, "w");
    if (!fp) {
        perror("Erro ao criar arquivo");
        exit(1);
    }
    
    // Criar array ordenado
    int *arr = malloc(sizeof(int) * tamanho);
    for (int i = 0; i < tamanho; i++) {
        arr[i] = i + 1;
    }
    
    // Embaralhar (Fisher-Yates)
    for (int i = tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    
    // Escrever no arquivo
    for (int i = 0; i < tamanho; i++) {
        fprintf(fp, "%d\n", arr[i]);
    }
    
    free(arr);
    fclose(fp);
    printf("Gerado: %s\n", nome_arquivo);
}

void gerar_decrescente(int tamanho, char* nome_arquivo) {
    FILE *fp = fopen(nome_arquivo, "w");
    if (!fp) {
        perror("Erro ao criar arquivo");
        exit(1);
    }
    
    for (int i = tamanho; i >= 1; i--) {
        fprintf(fp, "%d\n", i);
    }
    fclose(fp);
    printf("Gerado: %s\n", nome_arquivo);
}

int main() {
    srand(time(NULL));
    
    int tamanhos[] = {1000, 10000, 100000, 500000, 1000000};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);
    
    char nome_arquivo[100];
    
    printf("Gerando arquivos de entrada...\n");
    
    for (int i = 0; i < num_tamanhos; i++) {
        int n = tamanhos[i];
        
        // Ordenado
        sprintf(nome_arquivo, "entradas/entrada_%d_ordenado.txt", n);
        gerar_ordenado(n, nome_arquivo);
        
        // Aleatório
        sprintf(nome_arquivo, "entradas/entrada_%d_aleatorio.txt", n);
        gerar_aleatorio(n, nome_arquivo);
        
        // Decrescente
        sprintf(nome_arquivo, "entradas/entrada_%d_decrescente.txt", n);
        gerar_decrescente(n, nome_arquivo);
    }
    
    printf("Geração concluída!\n");
    return 0;
}