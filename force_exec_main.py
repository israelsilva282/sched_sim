import subprocess
import os
import matplotlib.pyplot as plt

def calcular_valor_medio(amostras_tme):
    n = len(amostras_tme)
    soma = sum(amostra for amostra in amostras_tme)
    valor_medio = soma / n
    return valor_medio

def calcular_variancia(amostras_tme):
    n = len(amostras_tme)
    valor_medio = calcular_valor_medio(amostras_tme)
    soma = sum((valor_medio - amostra) ** 2 for amostra in amostras_tme)
    variancia = soma / n
    return variancia

def main(algoritmo, numero_de_processos):
    # Definir o número de rodadas
    numero_de_rodadas = 10

    valores_tme = []  # Lista para armazenar os valores de TME

    for _ in range(numero_de_rodadas):
        # Executar o programa C e capturar o resultado completo
        resultado = subprocess.check_output(["./main_" + algoritmo, str(numero_de_processos)]).decode().strip()

        # Separar as linhas do resultado
        linhas = resultado.split("\n")

        # Verificar se cada linha contém o TME
        for linha in linhas:
            if "TME:" in linha:
                valor_tme = float(linha.split("TME: ")[1].strip())
                valores_tme.append(valor_tme)

    return valores_tme

def salvar_valores_tme(diretorio, nome_arquivo, valores_tme):
    caminho_arquivo = os.path.join(diretorio, nome_arquivo)
    with open(caminho_arquivo, "w") as f:
        for valor in valores_tme:
            f.write(str(valor) + "\n")

def main_fifo_exec():
    algoritmos = ["fifo", "ljf", "prio_dynamic", "prio_static"]
    quantidade_processos = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]

    resultados = {}  # Dicionário para armazenar os resultados por algoritmo

    for algoritmo in algoritmos:
        resultados[algoritmo] = {}  # Dicionário para armazenar os resultados por quantidade de processos

        for numero_de_processos in quantidade_processos:
            resultados[algoritmo][numero_de_processos] = main(algoritmo, numero_de_processos)

    medias_tme = []  # Lista para armazenar as médias do TME
    variancias_tme = []  # Lista para armazenar as variâncias do TME

    for numero_de_processos in quantidade_processos:
        media_algoritmos = []  # Lista para armazenar as médias do TME para cada algoritmo

        for algoritmo in algoritmos:
            valor_medio = calcular_valor_medio(resultados[algoritmo][numero_de_processos])
            media_algoritmos.append(valor_medio)

        media_geral = sum(media_algoritmos) / len(media_algoritmos)
        medias_tme.append(media_geral)

        variancia_geral = calcular_variancia(media_algoritmos)
        variancias_tme.append(variancia_geral)

    # Criar o gráfico
    plt.errorbar(quantidade_processos, medias_tme, yerr=variancias_tme, fmt="o", capsize=5)
    plt.xlabel("Quantidade de Processos")
    plt.ylabel("Tempo Médio de Espera")
    plt.title("Tempo Médio de Espera em Relação à Quantidade de Processos")
    plt.grid(True)

    # Exibir o gráfico
    plt.show()

    # Salvar o gráfico em um arquivo
    plt.savefig("grafico_tme.png")

# Chamar a função principal
main_fifo_exec()

# Abrir o arquivo do gráfico para visualização
import os
os.system("grafico_tme.png")
