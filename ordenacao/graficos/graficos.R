# graficos/graficos.R
library(ggplot2)
library(readr)
library(dplyr)
library(scales)

# Verificar se o arquivo existe
if (!file.exists("../resultados/benchmark.csv")) {
  cat("Erro: Arquivo benchmark.csv não encontrado!\n")
  cat("Execute o benchmark primeiro: make test\n")
  quit(status = 1)
}

# Ler dados
cat("Lendo dados do benchmark...\n")
dados <- read_csv("../resultados/benchmark.csv", show_col_types = FALSE)

# Extrair tipo do arquivo
dados$Tipo <- sapply(strsplit(dados$Arquivo, "_"), function(x) {
  tipo <- x[3]
  gsub("\\.txt$", "", tipo)
})

# Função para criar gráficos de tempo
criar_grafico_tempo <- function(tipo_entrada, titulo) {
  dados_filtrados <- dados[dados$Tipo == tipo_entrada, ]
  
  p <- ggplot(dados_filtrados, aes(x = Tamanho, y = Tempo, color = Algoritmo)) +
    geom_line(size = 1.2) +
    geom_point(size = 2) +
    scale_x_log10(labels = scales::comma) +
    scale_y_log10(labels = scales::comma) +
    labs(
      title = titulo,
      x = "Tamanho da Entrada",
      y = "Tempo (segundos)",
      color = "Algoritmo"
    ) +
    theme_minimal() +
    theme(
      plot.title = element_text(hjust = 0.5, size = 14, face = "bold"),
      legend.position = "bottom"
    )
  
  nome_arquivo <- paste0("tempo_", tipo_entrada, ".png")
  ggsave(nome_arquivo, p, width = 10, height = 6, dpi = 300)
  cat(paste("Salvo:", nome_arquivo, "\n"))
}

# Função para criar gráficos de comparações
criar_grafico_comparacoes <- function(tipo_entrada, titulo) {
  dados_filtrados <- dados[dados$Tipo == tipo_entrada, ]
  
  p <- ggplot(dados_filtrados, aes(x = Tamanho, y = Comparacoes, color = Algoritmo)) +
    geom_line(size = 1.2) +
    geom_point(size = 2) +
    scale_x_log10(labels = scales::comma) +
    scale_y_log10(labels = scales::comma) +
    labs(
      title = titulo,
      x = "Tamanho da Entrada",
      y = "Número de Comparações",
      color = "Algoritmo"
    ) +
    theme_minimal() +
    theme(
      plot.title = element_text(hjust = 0.5, size = 14, face = "bold"),
      legend.position = "bottom"
    )
  
  nome_arquivo <- paste0("comparacoes_", tipo_entrada, ".png")
  ggsave(nome_arquivo, p, width = 10, height = 6, dpi = 300)
  cat(paste("Salvo:", nome_arquivo, "\n"))
}

# Gerar todos os gráficos
cat("Gerando gráficos...\n")

# Verificar se temos dados para cada tipo
tipos_disponiveis <- unique(dados$Tipo)
cat("Tipos de entrada encontrados:", paste(tipos_disponiveis, collapse = ", "), "\n")

# Gráficos de tempo
if ("aleatorio" %in% tipos_disponiveis) {
  criar_grafico_tempo("aleatorio", "Tempo de Execução - Entradas Aleatórias")
}
if ("ordenado" %in% tipos_disponiveis) {
  criar_grafico_tempo("ordenado", "Tempo de Execução - Entradas Ordenadas")
}
if ("decrescente" %in% tipos_disponiveis) {
  criar_grafico_tempo("decrescente", "Tempo de Execução - Entradas Decrescentes")
}

# Gráficos de comparações
if ("aleatorio" %in% tipos_disponiveis) {
  criar_grafico_comparacoes("aleatorio", "Número de Comparações - Entradas Aleatórias")
}
if ("ordenado" %in% tipos_disponiveis) {
  criar_grafico_comparacoes("ordenado", "Número de Comparações - Entradas Ordenadas")
}
if ("decrescente" %in% tipos_disponiveis) {
  criar_grafico_comparacoes("decrescente", "Número de Comparações - Entradas Decrescentes")
}

# Resumo estatístico
cat("\n=== Resumo Estatístico ===\n")
resumo <- dados %>%
  group_by(Algoritmo, Tipo) %>%
  summarise(
    Tempo_Medio = mean(Tempo),
    Comparacoes_Media = mean(Comparacoes),
    .groups = 'drop'
  )

print(resumo)

cat("\nGráficos gerados com sucesso!\n")
cat("Arquivos salvos no diretório graficos/\n")
