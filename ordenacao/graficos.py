import os
import pandas as pd
import matplotlib.pyplot as plt
import unidecode

CAMINHO_CSV = os.path.join(".", "resultados", "benchmark.csv")
PASTA_SAIDA = os.path.join(".", "graficos")

os.makedirs(PASTA_SAIDA, exist_ok=True)

print("🔍 Lendo o arquivo CSV...")
try:
    df = pd.read_csv(CAMINHO_CSV) 
    print("✅ CSV lido com sucesso!\n")
except Exception as e:
    print("❌ Erro ao ler o CSV:", e)
    exit()

print("📄 Colunas detectadas:", df.columns.tolist())

print("\n🔎 Primeiras linhas do DataFrame:")
print(df.head())

def extrair_tipo(arquivo):
    if "aleatorio" in arquivo.lower():
        return "Aleatoria"
    elif "ordenado" in arquivo.lower():
        return "Ordenada"
    elif "decrescente" in arquivo.lower():
        return "Decrescente"
    return "Desconhecida"

print("\n🧠 Extraindo tipo de entrada...")
if "Arquivo" not in df.columns:
    print("❌ A coluna 'Arquivo' não existe no CSV!")
    exit()

df["TipoEntrada"] = df["Arquivo"].apply(extrair_tipo)
print("🎯 Tipos de entrada únicos encontrados:", df["TipoEntrada"].unique())

metricas = {
    "Comparacoes": "Número de Comparações",
    "Tempo": "Tempo de Execução (s)"
}

print("\n📈 Iniciando geração de gráficos...\n")
for tipo_entrada in df["TipoEntrada"].unique():
    print(f"➡️  Gerando gráficos para tipo de entrada: {tipo_entrada}")
    for metrica, label in metricas.items():
        print(f"   - Métrica: {label}")

        try:
            plt.figure(figsize=(10, 6))
            dados = df[df["TipoEntrada"] == tipo_entrada]

            if dados.empty:
                print(f"⚠️  Nenhum dado encontrado para tipo {tipo_entrada}")
                continue

            for algoritmo in dados["Algoritmo"].unique():
                dados_alg = dados[dados["Algoritmo"] == algoritmo].sort_values(by="Tamanho")

                print(f"     ↪️ Algoritmo: {algoritmo} ({len(dados_alg)} linhas)")
                print(f"        Tamanhos:", dados_alg["Tamanho"].tolist())
                print(f"        {metrica}:", dados_alg[metrica].tolist())

                plt.plot(dados_alg["Tamanho"], dados_alg[metrica], marker='o', label=algoritmo)

            plt.title(f"{label} - Entrada {tipo_entrada}")
            plt.xlabel("Tamanho da Entrada")
            plt.ylabel(label)
            plt.legend()
            plt.grid(True)
            plt.tight_layout()

            print("🔍 Exibindo gráfico interativo (feche a janela para continuar)...")
            plt.show()

            # aqui salva após interação
            nome_arquivo = f"{metrica.lower()}_{unidecode.unidecode(tipo_entrada.lower())}.png".replace(" ", "_")
            caminho_completo = os.path.join(PASTA_SAIDA, nome_arquivo)
            plt.savefig(caminho_completo)
            print(f"💾 Gráfico salvo como: {caminho_completo}\n")
            plt.close()

        except Exception as e:
            print(f"❌ Erro ao gerar gráfico para '{tipo_entrada}' - '{metrica}':", e)
