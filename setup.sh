#!/bin/bash

# Setup para Fedora - Instalação de dependências e estrutura
echo "=== Setup para Análise de Algoritmos de Ordenação - Fedora ==="

# Instalar dependências
echo "Instalando dependências..."
sudo dnf update -y
sudo dnf install -y gcc make R-core R-devel

# Instalar pacotes R necessários
echo "Instalando pacotes R..."
sudo R --slave -e "
if (!require('ggplot2')) install.packages('ggplot2', repos='https://cran.rstudio.com/')
if (!require('readr')) install.packages('readr', repos='https://cran.rstudio.com/')
if (!require('dplyr')) install.packages('dplyr', repos='https://cran.rstudio.com/')
if (!require('scales')) install.packages('scales', repos='https://cran.rstudio.com/')
"

# Criar estrutura de diretórios
echo "Criando estrutura de diretórios..."
mkdir -p ordenacao/{entradas,algoritmos,utils,resultados,graficos}

echo "Setup concluído!"
echo "Execute: cd ordenacao && make all"