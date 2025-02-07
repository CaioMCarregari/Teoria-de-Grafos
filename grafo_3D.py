
import csv
import random 
from collections import defaultdict
from pyvis import network as net
from IPython.display import display, HTML
import plotly.graph_objects as go

class Grafo:
    grafo = defaultdict(list)

    def __init__(self, dirigido=False) -> None:
        self.dirigido = dirigido

    def addAresta(self, vertice, vizinho) -> object:
        if vizinho not in self.grafo[vertice]:
            self.grafo[vertice].append(vizinho)
            if not self.dirigido:
                self.grafo[vizinho].append(vertice)

        return self

    def rmAresta(self, vertice, vizinho) -> object:
        if vertice in self.grafo:
            self.grafo[vertice].remove(vizinho)
            if not self.dirigido:
                self.grafo[vizinho].remove(vertice)
        return self

    def getNumVertices(self) -> int:
        return len(self.grafo.keys())

    def getVertices(self) -> list:
        return self.grafo.keys()

    def showMapa3D(self, output_file="grafo_3D.html"):
        posicoes = {v: [random.uniform(-1, 1) for _ in range(3)] for v in self.grafo}
        x_vertices = [posicoes[v][0] for v in self.grafo]
        y_vertices = [posicoes[v][1] for v in self.grafo]
        z_vertices = [posicoes[v][2] for v in self.grafo]
        x_arestas, y_arestas, z_arestas = [], [], []
        
        for vertice in self.grafo:
            for vizinho in self.grafo[vertice]:
                x_arestas += [posicoes[vertice][0], posicoes[vizinho][0], None]
                y_arestas += [posicoes[vertice][1], posicoes[vizinho][1], None]
                z_arestas += [posicoes[vertice][2], posicoes[vizinho][2], None]

        trace_arestas = go.Scatter3d(
            x=x_arestas, y=y_arestas, z=z_arestas,
            mode='lines', line=dict(color='black', width=2),
            hoverinfo='none'
        )
        trace_vertices = go.Scatter3d(
            x=x_vertices, y=y_vertices, z=z_vertices,
            mode='markers', marker=dict(symbol='circle', size=10, color='skyblue')
        )

        layout = go.Layout(title="Grafo 3D", autosize=True, showlegend=True)
        data = [trace_arestas, trace_vertices]
        fig = go.Figure(data=data, layout=layout)
        # Salvando o gráfico em um arquivo HTML
        fig.write_html(output_file)
        print(f"Grafo salvo como {output_file}")

def GrafoFromCSV(enderecoArquivo="", dirigido=False) -> Grafo:
    grafo = Grafo(dirigido)
    with open(enderecoArquivo, "r") as arquivo:
        leitor_csv = csv.reader(arquivo)
        next(leitor_csv)  # Ignora a primeira linha
        linhas = [list(map(int, linha)) for linha in leitor_csv]
        for i in range(len(linhas)):
            for j in range(len(linhas[i])):
                if linhas[i][j] == 1:  # Apenas valores 1
                    grafo.addAresta(i + 1, j + 1)
    return grafo

if __name__ == "__main__":
    name = 'TEG.csv'
    g = GrafoFromCSV(name)
    g.showMapa3D('grafo_3D.html')
