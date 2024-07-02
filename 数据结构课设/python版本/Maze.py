import json
import sys
import time

from PyQt5.QtCore import Qt, QBasicTimer
from PyQt5.QtGui import QPainter, QColor, QFont, QPen
from PyQt5.QtWidgets import QMainWindow, QApplication, QLabel, QPushButton


class config:
    WIDTH = 20  # 地图列数
    HEIGHT = 20  # 地图行数
    blockLength = 30  # 绘制画面时每一个节点方块的边长


class point:  # 点类（每一个唯一坐标只有对应的一个实例）
    _list = []  # 储存所有的point类实例
    _tag = True  # 标记最新创建的实例是否为_list中的已有的实例，True表示不是已有实例

    def __new__(cls, x, y):  # 重写new方法实现对于同样的坐标只有唯一的一个实例
        for i in point._list:
            if i.x == x and i.y == y:
                point._tag = False
                return i
        nt = super(point, cls).__new__(cls)
        point._list.append(nt)
        return nt

    def __init__(self, x, y):
        if point._tag:
            self.x = x
            self.y = y
            self.father = None
            self.F = 0  # 当前点的评分  F=G+H
            self.G = 0  # 起点到当前节点所花费的消耗
            self.cost = 0  # 父节点到此节点的消耗
        else:
            point._tag = True

    @classmethod
    def clear(cls):  # clear方法，每次搜索结束后，将所有点数据清除，以便进行下一次搜索的时候点数据不会冲突。
        point._list = []

    def __eq__(self, T):  # 重写==运算以便实现point类的in运算
        if type(self) == type(T):
            return (self.x, self.y) == (T.x, T.y)
        else:
            return False

    def __str__(self):
        return '(%d,%d)[F=%d,G=%d,cost=%d][father:(%s)]' % (self.x, self.y, self.F, self.G, self.cost, str((
            self.father.x,
            self.father.y)) if self.father != None else 'null')


class A_Search:  # 核心部分，寻路类
    def __init__(self, arg_start, arg_end, arg_map):
        self.start = arg_start  # 储存此次搜索的开始点
        self.end = arg_end  # 储存此次搜索的目的点
        self.Map = arg_map  # 一个二维数组，为此次搜索的地图引用
        self.open = []  # 开放列表：储存即将被搜索的节点
        self.close = []  # 关闭列表：储存已经搜索过的节点
        self.result = []  # 当计算完成后，将最终得到的路径写入到此属性中
        self.count = 0  # 记录此次搜索所搜索过的节点数
        self.useTime = 0  # 记录此次搜索花费的时间--在此演示中无意义，因为process方法变成了一个逐步处理的生成器，统计时间无意义。
        # 开始进行初始数据处理
        self.open.append(arg_start)

    def cal_F(self, loc):
        # print('计算值：',loc)
        G = loc.father.G + loc.cost
        H = self.getEstimate(loc)
        F = G + H
        # print("F=%d G=%d H=%d"%(F,G,H))
        return {'G': G, 'H': H, 'F': F}

    def F_Min(self):  # 搜索open列表中F值最小的点并将其返回，同时判断open列表是否为空，为空则代表搜索失败
        if len(self.open) <= 0:
            return None
        t = self.open[0]
        for i in self.open:
            if i.F < t.F:
                t = i
        return t

    def getAroundPoint(self, loc):  # 获取指定点周围所有可通行的点，并将其对应的移动消耗进行赋值。
        l = [(loc.x, loc.y + 1, 10), (loc.x + 1, loc.y + 1, 14), (loc.x + 1, loc.y, 10), (loc.x + 1, loc.y - 1, 14),
             (loc.x, loc.y - 1, 10), (loc.x - 1, loc.y - 1, 14), (loc.x - 1, loc.y, 10), (loc.x - 1, loc.y + 1, 14)]
        for i in l[::-1]:
            if i[0] < 0 or i[0] >= config.HEIGHT or i[1] < 0 or i[1] >= config.WIDTH:
                l.remove(i)
        nl = []
        for i in l:
            if self.Map[i[0]][i[1]] == 0:
                nt = point(i[0], i[1])
                nt.cost = i[2]
                nl.append(nt)
        return nl

    def addToOpen(self, l,
                  father):  # 此次判断的点周围的可通行点加入到open列表中，如此点已经在open列表中则对其进行判断，如果此次路径得到的F值较之之前的F值更小，则将其父节点更新为此次判断的点，同时更新F、G值。
        for i in l:
            if i not in self.open:
                if i not in self.close:
                    i.father = father
                    self.open.append(i)
                    r = self.cal_F(i)
                    i.G = r['G']
                    i.F = r['F']
            else:
                tf = i.father
                i.father = father
                r = self.cal_F(i)
                if i.F > r['F']:
                    i.G = r['G']
                    i.F = r['F']
                # i.father=father
                else:
                    i.father = tf

    def getEstimate(self, loc):  # H :从点loc移动到终点的预估花费
        return (abs(loc.x - self.end.x) + abs(loc.y - self.end.y)) * 10

    def DisplayPath(self):  # 在此演示中无意义
        print('搜索花费的时间:%.2fs.迭代次数%d,路径长度:%d' % (self.useTime, self.count, len(self.result)))
        if self.result != None:
            for i in self.result:
                self.Map[i.x][i.y] = 8
            for i in self.Map:
                for j in i:
                    if j == 0:
                        print('%s' % '□', end='')
                    elif j == 1:
                        print('%s' % '▽', end='')
                    elif j == 8:
                        print('%s' % '★', end='')
                print('')
        else:
            print('搜索失败，无可通行路径')

    def process(self):  # 使用yield将process方法变成一个生成器，可以逐步的对搜索过程进行处理并返回关键数据
        while True:
            self.count += 1
            tar = self.F_Min()  # 先获取open列表中F值最低的点tar
            if tar == None:
                self.result = None
                self.count = -1
                break
            else:
                aroundP = self.getAroundPoint(tar)  # 获取tar周围的可用点列表aroundP
                self.addToOpen(aroundP, tar)  # 把aroundP加入到open列表中并更新F值以及设定父节点
                self.open.remove(tar)  # 将tar从open列表中移除
                self.close.append(tar)  # 已经迭代过的节点tar放入close列表中
                if self.end in self.open:  # 判断终点是否已经处于open列表中
                    e = self.end
                    self.result.append(e)
                    while True:
                        e = e.father
                        if e == None:
                            break
                        self.result.append(e)
                    yield (tar, self.open, self.close)
                    break

            # self.repaint()
            # print('返回')
            yield (tar, self.open, self.close)
        # time.sleep(2)
        self.useTime = time2 - time1


class GameBoard(QMainWindow):  # 可视化类，pyqt5进行编写。
    def __init__(self):
        print('初始化地图...')
        self.Map = []
        for i in range(config.HEIGHT):
            col = []
            for j in range(config.WIDTH):
                col.append(0)
            self.Map.append(col)
        self.startPoint = None
        self.endPoint = None
        self.search = None
        self.centerTimer = None
        self.yi = None
        self.special = None
        self.displayFlush = False
        super().__init__()
        print('初始化UI...')
        self.initUI()

    def initUI(self):
        # 开始初始化UI部分
        # 创建UI控件
        self.label_tips = QLabel(
            "<p style='color:green'>使用说明：</p>右键单击格子选定起始点,左键格子选定格子为墙壁或删除墙壁。\n<p style='color:green'>颜色说明：</p>\n黄色代表起点，绿色代表终点，黑色代表墙壁，红色代表待搜索的open列表，灰色代表已搜索过的close列表，蓝色代表当前搜索到的路径",
            self)
        self.label_display = QLabel("", self)
        self.button_start = QPushButton("开始搜索", self)
        self.button_clearSE = QPushButton("重选起始点", self)
        self.button_clearWall = QPushButton("清空地图墙壁", self)
        self.button_saveMap = QPushButton("保存地图", self)
        self.button_loadMap = QPushButton("加载地图", self)

        # 设置控件属性
        self.label_tips.setWordWrap(True)
        self.label_display.setWordWrap(True)
        # 设置控件样式
        self.label_display.setStyleSheet("border:1px solid black")
        self.label_display.setAlignment(Qt.AlignLeft)
        self.label_display.setAlignment(Qt.AlignTop)
        # 设置控件的尺寸和位置
        self.label_tips.resize(200, 150)
        self.button_saveMap.resize(80, 30)
        self.button_loadMap.resize(80, 30)
        self.label_display.resize(200, 300)

        self.label_tips.move(100 + (config.WIDTH - 1) * config.blockLength, 0)
        self.label_display.move(100 + (config.WIDTH - 1) * config.blockLength, 400)
        self.button_start.move(100 + (config.WIDTH - 1) * config.blockLength, 200)
        self.button_clearSE.move(100 + (config.WIDTH - 1) * config.blockLength, 250)
        self.button_clearWall.move(100 + (config.WIDTH - 1) * config.blockLength, 300)
        self.button_saveMap.move(100 + (config.WIDTH - 1) * config.blockLength, 350)
        self.button_loadMap.move(200 + (config.WIDTH - 1) * config.blockLength, 350)
        # 给控件绑定事件
        self.button_start.clicked.connect(self.button_StartEvent)
        self.button_clearSE.clicked.connect(self.button_Clear)
        self.button_clearWall.clicked.connect(self.button_Clear)
        self.button_saveMap.clicked.connect(self.button_SaveMap)
        self.button_loadMap.clicked.connect(self.button_LoadMap)
        # UI初始化完成
        self.setGeometry(0, 0, 150 + (config.WIDTH * config.blockLength - config.blockLength) + 200,
                         150 + (config.HEIGHT * config.blockLength - config.blockLength))
        self.setMinimumSize(150 + (config.WIDTH * config.blockLength - config.blockLength) + 200,
                            150 + (config.HEIGHT * config.blockLength - config.blockLength))
        self.setMaximumSize(150 + (config.WIDTH * config.blockLength - config.blockLength) + 200,
                            150 + (config.HEIGHT * config.blockLength - config.blockLength))
        self.setWindowTitle('A*搜索')
        self.show()

    def addDisplayText(self, text):
        if self.displayFlush:
            self.label_display.setText(text + '\n')
            self.displayFlush = False
        else:
            self.label_display.setText(self.label_display.text() + text + '\n')

    def mousePressEvent(self, event):
        x, y = event.x() - 50, event.y() - 50
        x = x // config.blockLength
        y = y // config.blockLength
        if x >= 0 and x < config.WIDTH and y >= 0 and y < config.HEIGHT:
            if event.button() == Qt.LeftButton:
                if (x, y) != self.startPoint and (x, y) != self.endPoint:
                    self.Map[y][x] = (1 if self.Map[y][x] == 0 else 0)
            if event.button() == Qt.RightButton:
                if self.Map[y][x] == 0:
                    if self.startPoint == None:
                        self.startPoint = (x, y)
                        self.addDisplayText('添加了一个起点:(%d,%d)' % (x, y))
                    elif self.endPoint == None and self.startPoint != (x, y):
                        self.endPoint = (x, y)
                        self.addDisplayText('添加了一个终点:(%d,%d)' % (x, y))
            self.repaint()

    def button_StartEvent(self):
        sender = self.sender()
        print(sender)
        if self.startPoint != None and self.endPoint != None:
            if self.centerTimer == None:
                self.centerTimer = QBasicTimer()
            self.button_start.setEnabled(False)
            self.button_clearSE.setEnabled(False)
            self.button_clearWall.setEnabled(False)
            self.centerTimer.start(200, self)
            self.search = A_Search(point(self.startPoint[1], self.startPoint[0]),
                                   point(self.endPoint[1], self.endPoint[0]), self.Map)
            self.yi = self.search.process()
            self.addDisplayText('开始进行搜索')

    def button_SaveMap(self):
        with open('map.txt', 'w') as f:
            f.write(json.dumps(self.Map))
            self.addDisplayText('地图保存成功-->map.txt')

    # else:
    # self.addDisplayText('地图保存失败')
    def button_LoadMap(self):
        try:
            with open('map.txt', 'r') as f:
                self.Map = json.loads(f.read())
                config.HEIGHT = len(self.Map)
                config.WIDTH = len(self.Map[0])
                self.addDisplayText('地图加载成功')
                self.repaint()
        except Exception as e:
            print('失败', e, type(e))
            if type(e) == FileNotFoundError:
                self.addDisplayText('地图加载失败:地图文件不存在')
            elif type(e) == json.decoder.JSONDecodeError:
                self.addDisplayText('地图加载失败:错误的地图文件')

    def button_Clear(self):
        sender = self.sender()
        print(self.button_clearSE, type(self.button_clearSE))
        if sender == self.button_clearSE:
            self.startPoint = None
            self.endPoint = None
            self.repaint()
            self.addDisplayText('清空起始点')
        elif sender == self.button_clearWall:
            for i in range(len(self.Map)):
                for j in range(len(self.Map[i])):
                    self.Map[i][j] = 0
            self.repaint()
            self.addDisplayText('清空所有墙壁')

    def paintEvent(self, event):
        qp = QPainter()
        qp.begin(self)
        self.drawBoard(event, qp)
        qp.end()

    def drawBoard(self, event, qp):
        self.drawMap(qp)

    def drawMap(self, qp):  # 画面绘制方法，每次地图有所改动都将重绘
        time1 = time.time()
        if self.search != None:
            if self.special != None:
                e = self.special[0]
                path = [e]
                while True:
                    e = e.father
                    if e != None:
                        path.append(e)
                    else:
                        break
            else:
                path = None
            pen = QPen(QColor(0, 0, 0), 1, Qt.SolidLine)
            qp.setPen(pen)
            for i in range(len(self.Map)):
                for j in range(len(self.Map[i])):
                    wordTag = False
                    if i == self.search.start.x and j == self.search.start.y:
                        qp.setBrush(QColor(255, 255, 0))
                    elif i == self.search.end.x and j == self.search.end.y:
                        qp.setBrush(QColor(100, 200, 50))
                    else:
                        if self.Map[i][j] == 0:
                            tagx = True
                            if path:
                                for k in path:
                                    if k.x == i and k.y == j:
                                        tagx = False
                                        qp.setBrush(QColor(0, 100, 255))
                            if tagx:
                                if self.special != None:
                                    if i == self.special[0].x and j == self.special[0].y:
                                        qp.setBrush(QColor(0, 255, 0))
                                    else:
                                        tag = True
                                        for k in self.special[1]:
                                            if k.x == i and k.y == j:
                                                tag = False
                                                wordTag = True
                                                word = str(k.F)

                                                qp.setBrush(QColor(150, 0, 0))
                                                break
                                            else:
                                                qp.setBrush(QColor(220, 220, 220))
                                        if tag:
                                            for k in self.special[2]:
                                                if k.x == i and k.y == j:
                                                    qp.setBrush(QColor(150, 150, 150))
                                                    break
                                                else:
                                                    qp.setBrush(QColor(220, 220, 220))
                                else:
                                    qp.setBrush(QColor(220, 220, 220))
                        elif self.Map[i][j] == 1:
                            qp.setBrush(QColor(0, 0, 0))
                        else:
                            qp.setBrush(QColor(255, 0, 0))
                    qp.drawRect(50 + j * config.blockLength, 50 + i * config.blockLength, config.blockLength,
                                config.blockLength)
                    if wordTag:
                        qp.setFont(QFont('楷体', 5, QFont.Light))
                        qp.drawText(50 + 10 + j * config.blockLength, 50 + 10 + i * config.blockLength, word)
                        wordTag = False
        else:
            for i in range(len(self.Map)):
                for j in range(len(self.Map[i])):
                    if (j, i) == self.startPoint:
                        qp.setBrush(QColor(255, 255, 0))
                    elif (j, i) == self.endPoint:
                        qp.setBrush(QColor(100, 200, 50))
                    else:
                        if self.Map[i][j] == 0:
                            qp.setBrush(QColor(220, 220, 220))
                        elif self.Map[i][j] == 1:
                            qp.setBrush(QColor(0, 0, 0))
                        else:
                            qp.setBrush(QColor(255, 0, 0))

                    qp.drawRect(50 + j * config.blockLength, 50 + i * config.blockLength, config.blockLength,
                                config.blockLength)
        time2 = time.time()

    # print('绘制时间：',time2-time1)
    def timerEvent(self, e):
        try:
            data = next(self.yi)
        except Exception as e:
            self.addDisplayText('搜索结束:')
            if self.search.result == None:
                self.addDisplayText('未找到可行路径')
            else:
                self.addDisplayText('总计搜索节点数：%d' % self.search.count)
                self.addDisplayText('最终路径长度：%d' % len(self.search.result))
            self.centerTimer.stop()
            self.search = None
            self.yi = None
            self.special = None
            point.clear()
            self.button_start.setEnabled(True)
            self.button_clearSE.setEnabled(True)
            self.button_clearWall.setEnabled(True)
            self.displayFlush = True
        else:
            self.special = data
            self.repaint()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = GameBoard()
    sys.exit(app.exec_())
