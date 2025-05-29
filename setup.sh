#!/bin/bash

echo "=== Setup para Análise de Algoritmos de Ordenação"

# Instalar dependências
echo "Instalando dependências..."
sudo dnf update -y
sudo dnf install -y gcc make R-core R-devel

# Criar estrutura de diretórios
echo "Criando estrutura de diretórios..."
mkdir -p ordenacao/{entradas,algoritmos,utils,resultados,graficos}

echo "Setup concluído!"
echo "Execute: cd ordenacao && make all"
