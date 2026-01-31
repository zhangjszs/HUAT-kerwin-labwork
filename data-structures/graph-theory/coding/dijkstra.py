# GUI based implementation of Dijkstra's algorithm
import sys

import matplotlib.pyplot as plt
import networkx as nx


# Node class
class Node:
    def __init__(self, name):
        self.name = name
        self.neighbors = {}
        self.distance = sys.maxsize
        self.parent = None
        self.parentEdgeWeight = None

    def add_neighbor(self, neighbor, weight):
        if (neighbor not in self.neighbors):
            self.neighbors[neighbor] = weight
        else:
            print(neighbor + ' 已经是 ' + self.name + ' 的邻居！')

    def print_neighbors(self):
        print('打印所有邻居及其权重...')
        for n, w in self.neighbors.items():
            print('邻居: ' + n + ' 权重: ' + str(w))


# Graph class
class Graph:
    def __init__(self):
        self.graph = {}

    def add_node(self, name):
        if (name not in self.graph):
            newNode = Node(name)
            self.graph[name] = newNode
        else:
            print(name + ' 节点已经存在！')

    def add_edge(self, u, v, w):
        if (u in self.graph and v in self.graph):
            self.graph[u].add_neighbor(v, w)
        else:
            print('在 ' + u + ' 和 ' + v + ' 之间添加边时出错！请检查图中是否存在这两个节点...')

    def print_graph(self):
        print('打印图形...')
        for u, u_node in self.graph.items():
            print(u + ': ' + str(u_node.neighbors))

    def Dijkstra(self, s):

        # check that starting node exists in the graph
        if (s not in self.graph):
            print('名称为 ' + s + ' 的节点不存在于图中！')
            return

        # init distance of source node and build unvisited minheap
        self.graph[s].distance = 0
        unvisited = MinHeap(self.graph)

        while (unvisited.currSize > 0):

            current_node = unvisited.getMin()

            # check if its neighbors can do any better by using one of its edges
            for n, w in current_node.neighbors.items():

                if (self.graph[n].distance > current_node.distance + w):
                    self.graph[n].distance = current_node.distance + w
                    self.graph[n].parent = current_node.name
                    self.graph[n].parentEdgeWeight = w
            unvisited.extract()

        for u, v in self.graph.items():
            print(s + ' 到 ' + u + ' 的最短距离: ' + str(v.distance))


# MinHeap class
class MinHeap:
    def __init__(self, graph):
        self.maxSize = len(graph)
        self.currSize = 0

        # build heap
        self.heap = [Node('dummyNode')] * (self.maxSize + 1)
        self.heap[0].distance = -1 * sys.maxsize
        self.min = 1

        for v in graph.values():
            self.insert(v)
        for pos in range(self.currSize // 2, 0, -1):
            self.heapify(pos)

    def getParent(self, elt):
        return elt // 2

    def getLeftChild(self, elt):
        return elt * 2

    def getRightChild(self, elt):
        return (elt * 2) + 1

    def isLeaf(self, elt):
        if (elt <= self.currSize) and (elt >= self.currSize // 2):
            return True
        else:
            return False

    def swap(self, elt1, elt2):
        self.heap[elt1], self.heap[elt2] = self.heap[elt2], self.heap[elt1]

    # 插入一个元素到堆中
    def insert(self, elt):
        if (self.currSize == self.maxSize):
            return
        self.currSize += 1
        self.heap[self.currSize] = elt

        currElt = self.currSize

        while (self.heap[self.getParent(self.currSize)].distance > self.heap[currElt].distance):
            self.swap(self.getParent(self.currSize), currElt)
            currElt = self.getParent(currElt)

    def heapify(self, elt):

        if self.currSize == 0:
            return

        if self.isLeaf(elt):
            return
        if (self.heap[elt].distance > self.heap[self.getLeftChild(elt)].distance or
                self.heap[elt].distance > self.heap[self.getRightChild(elt)].distance):

            # swap elt with smaller child and recurse
            smallerChild = self.getLeftChild(elt)
            if (self.heap[self.getRightChild(elt)].distance < self.heap[smallerChild].distance):
                smallerChild = self.getRightChild(elt)

            self.swap(elt, smallerChild)
            self.heapify(smallerChild)

    # extract minimum element from the heap
    def extract(self):
        self.heap[self.min] = self.heap[self.currSize]
        self.currSize -= 1
        self.heapify(self.min)

    def getMin(self):
        return self.heap[self.min]


def main():
    print('*********************************************')
    print('n：添加一个节点')
    print('e：添加一条边')
    print('g：查看当前的图形')
    print('d：运行Dijkstra算法')
    print('x：退出程序')
    print('*********************************************')

    # initialize graph
    g = Graph()

    # prompt user
    while True:
        action = input('请选择一个操作：')

        if action == 'n':
            nodeName = input('请输入一个唯一的节点名称：')

            if nodeName in g.graph:
                print('该名称的节点已经存在...')
                continue
            g.add_node(nodeName)
        elif action == 'e':
            fromNode = input('从哪个节点出发：')
            if fromNode not in g.graph:
                print('该节点不存在！')
                continue
            toNode = input('到哪个节点：')
            if toNode not in g.graph:
                print('该节点不存在！')
                continue
            weight = input('边的权值（不为负数！）：')
            try:
                weight = float(weight)
            except ValueError:
                print('无效的权值！')
                continue
            if weight < 0:
                print('不允许使用负权重！')
                continue
            g.add_edge(fromNode, toNode, weight)
        elif action == 'g':
            if len(g.graph) == 0:
                print('图中没有节点！')
                continue

            DG = nx.DiGraph()

            for u in g.graph.keys():
                DG.add_node(u)

            for u, v in g.graph.items():
                for n, w in v.neighbors.items():
                    DG.add_edge(u, n, weight=w)

            # draw graph
            pos = nx.random_layout(DG)
            nx.draw(DG, pos, with_labels=True)
            labels = nx.get_edge_attributes(DG, 'weight')
            nx.draw_networkx_edge_labels(DG, pos, edge_labels=labels)
            plt.draw()
            plt.show()


        elif action == 'd':
            start = input('从哪个节点开始运行Dijkstra算法：')
            if start not in g.graph:
                print('该节点不存在...')
                continue

            g.Dijkstra(start)

            DG = nx.DiGraph()

            for u in g.graph.keys():
                DG.add_node(u)

            for u, v in g.graph.items():
                if (v.parent != None):
                    DG.add_edge(v.parent, u, weight=v.parentEdgeWeight)

            # draw graph
            pos = nx.random_layout(DG)
            nx.draw(DG, pos, with_labels=True)
            labels = nx.get_edge_attributes(DG, 'weight')
            nx.draw_networkx_edge_labels(DG, pos, edge_labels=labels)
            plt.draw()
            plt.show()

        elif action == 'x':
            return
        else:
            print('无效的操作！请重试！')


if __name__ == "__main__":
    main()
