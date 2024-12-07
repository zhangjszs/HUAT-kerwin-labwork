import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button

class REDSimulator:
    def __init__(self, max_p=0.1, avg_queue_length=0, min_th=5, max_th=15):
        self.max_p = max_p  # 最大丢包概率
        self.avg_queue_length = avg_queue_length  # 平均队列长度
        self.min_th = min_th  # 最小阈值
        self.max_th = max_th  # 最大阈值

    def calculate_drop_probability(self, queue_length):
        if queue_length < self.min_th:
            return 0
        elif queue_length > self.max_th:
            return 1
        else:
            # 线性增加的丢包概率
            pb = self.max_p * (queue_length - self.min_th) / (self.max_th - self.min_th)
            return pb

def main():
    # 初始化RED模拟器
    red = REDSimulator()

    # 创建图形界面
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 12))
    plt.subplots_adjust(left=0.1, bottom=0.35)

    # 生成初始数据
    queue_lengths = np.linspace(0, 30, 300)
    drop_probs = [red.calculate_drop_probability(ql) for ql in queue_lengths]

    # 绘制初始曲线
    line1, = ax1.plot(queue_lengths, drop_probs, 'b-', lw=2)
    ax1.set_xlabel('Queue Length')
    ax1.set_ylabel('Drop Probability')
    ax1.set_title('RED Algorithm Drop Probability Curve')
    ax1.grid(True)

    # 创建滑动条
    axcolor = 'lightgoldenrodyellow'
    ax_max_p = plt.axes([0.1, 0.2, 0.65, 0.03], facecolor=axcolor)
    ax_min_th = plt.axes([0.1, 0.15, 0.65, 0.03], facecolor=axcolor)
    ax_max_th = plt.axes([0.1, 0.1, 0.65, 0.03], facecolor=axcolor)

    s_max_p = Slider(ax_max_p, 'Max P', 0.01, 1.0, valinit=0.1)
    s_min_th = Slider(ax_min_th, 'Min Th', 0, 20, valinit=5)
    s_max_th = Slider(ax_max_th, 'Max Th', 5, 40, valinit=15)

    # 更新函数
    def update(val):
        # 获取新的参数值
        red.max_p = s_max_p.val
        red.min_th = s_min_th.val
        red.max_th = s_max_th.val

        # 确保max_th始终大于min_th
        if red.max_th <= red.min_th:
            red.max_th = red.min_th + 1
            s_max_th.set_val(red.max_th)

        # 重新计算丢包概率
        drop_probs = [red.calculate_drop_probability(ql) for ql in queue_lengths]
        line1.set_ydata(drop_probs)

        # 计算平均队列长度
        avg_queue_length = np.mean(queue_lengths)
        avg_drop_prob = np.mean(drop_probs)

        # 更新第二个图表
        ax2.clear()
        ax2.plot(queue_lengths, drop_probs, 'r-', lw=2)
        ax2.set_xlabel('Queue Length')
        ax2.set_ylabel('Drop Probability')
        ax2.set_title('Impact of Parameters on RED Algorithm')
        ax2.grid(True)
        ax2.axvline(x=avg_queue_length, color='g', linestyle='--', label='Avg Queue Length')
        ax2.axhline(y=avg_drop_prob, color='b', linestyle='--', label='Avg Drop Probability')
        ax2.legend()

        # 重绘图形
        fig.canvas.draw_idle()

    # 注册更新函数
    s_max_p.on_changed(update)
    s_min_th.on_changed(update)
    s_max_th.on_changed(update)

    # 添加重置按钮
    reset_ax = plt.axes([0.8, 0.15, 0.1, 0.04])
    reset_button = Button(reset_ax, 'Reset', color=axcolor, hovercolor='0.975')

    def reset(event):
        s_max_p.reset()
        s_min_th.reset()
        s_max_th.reset()

    reset_button.on_clicked(reset)

    plt.show()

if __name__ == "__main__":
    main()