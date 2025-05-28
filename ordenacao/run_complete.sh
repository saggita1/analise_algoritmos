#!/bin/bash

# run_complete.sh - Script para execução completa do projeto
echo "=== Análise de Algoritmos de Ordenação - Fedora ==="
echo "Este script executará todo o projeto automaticamente"
echo

# Verificar se estamos no diretório correto
if [ ! -f "Makefile" ]; then
    echo "Erro: Execute este script no diretório 'ordenacao'"
    echo "Primeiro execute: ./setup_fedora.sh && cd ordenacao"
    exit 1
fi

# Função para tratar Ctrl+C
cleanup() {
    echo
    echo "=== Execução Interrompida ==="
    echo "Resultados parciais foram salvos em:"
    echo "- resultados/benchmark.csv"
    echo "Para gerar gráficos com dados parciais: make graficos"
    exit 0
}

trap cleanup SIGINT

echo "1. Compilando algoritmos..."
make clean > /dev/null 2>&1
if ! make all; then
    echo "Erro na compilação!"
    exit 1
fi

echo "✓ Compilação concluída"
echo

echo "2. Gerando arquivos de entrada..."
if ! make entradas; then
    echo "Erro na geração de entradas!"
    exit 1
fi

echo "✓ Arquivos de entrada gerados"
echo "   Tamanhos: 1K, 10K, 100K, 500K, 1M elementos"
echo "   Tipos: ordenado, aleatório, decrescente"
echo

echo "3. Iniciando benchmark..."
echo "   Este processo pode levar alguns minutos"
echo "   Pressione Ctrl+C para interromper e salvar resultados parciais"
echo

# Criar diretório de resultados se não existir
mkdir -p resultados

if ! make test; then
    echo "Erro durante o benchmark!"
    exit 1
fi

echo
echo "✓ Benchmark concluído"
echo

echo "4. Gerando gráficos..."
if ! make graficos; then
    echo "Erro na geração de gráficos!"
    echo "Verifique se o R está instalado e os pacotes estão disponíveis"
    exit 1
fi

echo "✓ Gráficos gerados"
echo

echo "=== PROJETO CONCLUÍDO COM SUCESSO! ==="
echo
echo "📊 Resultados disponíveis em:"
echo "   - resultados/benchmark.csv (dados completos)"
echo "   - graficos/*.png (6 gráficos)"
echo
echo "📈 Gráficos gerados:"
echo "   - tempo_aleatorio.png"
echo "   - tempo_ordenado.png" 
echo "   - tempo_decrescente.png"
echo "   - comparacoes_aleatorio.png"
echo "   - comparacoes_ordenado.png"
echo "   - comparacoes_decrescente.png"
echo
echo "🔍 Para visualizar um resumo dos dados:"
echo "   head -20 resultados/benchmark.csv"
echo
echo "🖼️  Para ver os gráficos (GNOME):"
echo "   eog graficos/*.png"