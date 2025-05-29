import os
import pandas as pd
import matplotlib.pyplot as plt
import unidecode
import glob

PASTA_CSV = os.path.join(".", "resultados")
PASTA_SAIDA = os.path.join(".", "graficos")

os.makedirs(PASTA_SAIDA, exist_ok=True)

print("🔍 Procurando arquivos CSV em './resultados/'...")
arquivos_csv = glob.glob(os.path.join(PASTA_CSV, "*.csv"))

if len(arquivos_csv) < 3:
    print(f"❌ Esperado pelo menos 3 arquivos .csv, mas encontrados {len(arquivos_csv)}.")
    exit()
else:
    print(f"✅ {len(arquivos_csv)} arquivos encontrados. Usando os 3 primeiros:\n", arquivos_csv[:3])

dataframes = []
for caminho in arquivos_csv[:3]:
    try:
        df_temp = pd.read_csv(caminho)
        dataframes.append(df_temp)
        print(f"📥 Arquivo '{caminho}' lido com sucesso ({len(df_temp)} linhas).")
    except Exception as e:
        print(f"❌ Erro ao ler o arquivo '{caminho}':", e)
        exit()

df = pd.concat(dataframes, ignore_index=True)
print(f"\n📊 Total de dados combinados: {len(df)} linhas")

def extrair_tipo(arquivo):
    if "aleatorio" in arquivo.lower():
        return "Aleatoria"
    elif "ordenado" in arquivo.lower():
        return "Ordenada"
    elif "decrescente" in arquivo.lower():
        return "Decrescente"
    return "Desconhecida"

if "Arquivo" not in df.columns:
    print("❌ A coluna 'Arquivo' não existe no CSV!")
    exit()

df["TipoEntrada"] = df["Arquivo"].apply(extrair_tipo)
print("🎯 Tipos de entrada únicos:", df["TipoEntrada"].unique())

colunas_para_agrupar = ["Tamanho", "Algoritmo", "TipoEntrada"]
metricas = {
    "Comparacoes": "Número de Comparações",
    "Tempo": "Tempo de Execução (s)"
}

print("\n📊 Calculando médias por grupo...")
df_media = df.groupby(colunas_para_agrupar).mean(numeric_only=True).reset_index()

print("\n📈 Iniciando geração de gráficos com médias...\n")
for tipo_entrada in df_media["TipoEntrada"].unique():
    print(f"➡️  Gerando gráficos para tipo de entrada: {tipo_entrada}")
    for metrica, label in metricas.items():
        try:
            plt.figure(figsize=(10, 6))
            dados = df_media[df_media["TipoEntrada"] == tipo_entrada]

            if dados.empty:
                print(f"⚠️  Nenhum dado encontrado para tipo {tipo_entrada}")
                continue

            for algoritmo in dados["Algoritmo"].unique():
                dados_alg = dados[dados["Algoritmo"] == algoritmo].sort_values(by="Tamanho")

                print(f"     ↪️ Algoritmo: {algoritmo} ({len(dados_alg)} linhas)")
                plt.plot(dados_alg["Tamanho"], dados_alg[metrica], marker='o', label=algoritmo)

            plt.title(f"{label} - Entrada {tipo_entrada}")
            plt.xlabel("Tamanho da Entrada")
            plt.ylabel(label)
            plt.legend()
            plt.grid(True)
            plt.tight_layout()

            nome_arquivo = f"{metrica.lower()}_{unidecode.unidecode(tipo_entrada.lower())}.png".replace(" ", "_")
            caminho_completo = os.path.join(PASTA_SAIDA, nome_arquivo)
            plt.savefig(caminho_completo)
            print(f"💾 Gráfico salvo como: {caminho_completo}\n")

            print("🔍 Exibindo gráfico interativo (feche a janela para continuar)...")
            plt.show()
            plt.close()

        except Exception as e:
            print(f"❌ Erro ao gerar gráfico para '{tipo_entrada}' - '{metrica}':", e)
