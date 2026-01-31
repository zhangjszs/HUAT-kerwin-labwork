import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button

class ImprovedREDSimulator:
    def __init__(self, wq=0.02, max_p=0.1, avg_queue_length=0, min_th=10, max_th=30, beta=3):
        self.wq = wq  # 队列权重
        self.max_p = max_p  # 最大丢包概率
        self.avg_queue_length = avg_queue_length  # 平均队列长度
        self.min_th = min_th  # 最小阈值
        self.max_th = max_th  # 最大阈值

        # 目标队列长度范围参数
        self.target_low = min_th + 0.4 * (max_th - min_th)  # 参数A
        self.target_high = min_th + 0.6 * (max_th - min_th)  # 参数B
        self.beta = beta  # S型函数的斜率参数

    def s_shaped_probability(self, queue_length):
        """使用S型升半哥西分布函数计算丢包概率"""
        if queue_length <= self.min_th:
            return 0
        elif queue_length >= 2 * self.max_th:
            return 1
        else:
            # S型函数参数
            beta = self.beta
            a = self.max_p / ((1 - self.max_p) * (self.max_th - self.min_th) ** beta)

            # S型升半哥西分布函数
            numerator = self.max_p * (queue_length - self.min_th) ** beta
            denominator = (1 - self.max_p) * (2 * self.max_th - self.min_th) ** beta + \
                          self.max_p * (queue_length - self.max_th) ** beta
            return numerator / denominator

    def update_max_p(self, queue_length):
        """动态自适应调整max_p"""
        if queue_length > self.target_high:
            # 当队列长度大于目标范围上限时增加max_p
            delta = (queue_length - self.target_high) / (2 * self.max_th - self.min_th)
            self.max_p += delta
        elif queue_length < self.target_low:
            # 当队列长度小于目标范围下限时减少max_p
            delta = (self.target_low - queue_length) / (2 * self.max_th - self.min_th)
            self.max_p *= (1 - delta)

        # 确保max_p在合理范围内
        self.max_p = max(0.01, min(self.max_p, 1.0))


def main():
    # 初始化改进的RED模拟器
    improved_red = ImprovedREDSimulator()

    # 创建图形界面
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 12))
    plt.subplots_adjust(left=0.1, bottom=0.25, hspace=0.3)

    # 生成队列长度数据
    queue_lengths = np.linspace(0, 60, 600)

    # 计算传统RED和改进RED的丢包概率
    traditional_probs = [
        0 if ql <= improved_red.min_th else
        1 if ql >= improved_red.max_th else
        improved_red.max_p * (ql - improved_red.min_th) /
        (improved_red.max_th - improved_red.min_th)
        for ql in queue_lengths
    ]

    improved_probs = [improved_red.s_shaped_probability(ql) for ql in queue_lengths]

    # 绘制对比曲线
    line1, = ax1.plot(queue_lengths, traditional_probs, 'b-', label='Traditional RED')
    line2, = ax1.plot(queue_lengths, improved_probs, 'r-', label='Improved RED')
    ax1.set_xlabel('Queue Length')
    ax1.set_ylabel('Drop Probability')
    ax1.set_title('Traditional vs Improved RED Drop Probability')
    ax1.grid(True)
    ax1.legend()

    # 绘制max_p动态调整曲线
    time_points = np.linspace(0, 50, 500)
    max_p_values = []
    queue_length = improved_red.min_th

    for t in time_points:
        # 模拟队列长度变化
        queue_length += np.sin(t / 2) * 5
        improved_red.update_max_p(queue_length)
        max_p_values.append(improved_red.max_p)

    line3, = ax2.plot(time_points, max_p_values, 'g-')
    ax2.set_xlabel('Time')
    ax2.set_ylabel('Max Drop Probability')
    ax2.set_title('Dynamic Adjustment of Max_p')
    ax2.grid(True)

    # 创建滑动条
    axcolor = 'lightgoldenrodyellow'
    ax_min_th = plt.axes([0.1, 0.15, 0.65, 0.03], facecolor=axcolor)
    ax_max_th = plt.axes([0.1, 0.1, 0.65, 0.03], facecolor=axcolor)
    ax_max_p = plt.axes([0.1, 0.05, 0.65, 0.03], facecolor=axcolor)
    ax_beta = plt.axes([0.1, 0.2, 0.65, 0.03], facecolor=axcolor)

    s_min_th = Slider(ax_min_th, 'Min Th', 0, 30, valinit=10)
    s_max_th = Slider(ax_max_th, 'Max Th', 10, 60, valinit=30)
    s_max_p = Slider(ax_max_p, 'Initial Max P', 0.01, 1.0, valinit=0.1)
    s_beta = Slider(ax_beta, 'Beta', 1, 5, valinit=3)

    def update(val):
        # 更新参数
        improved_red.min_th = s_min_th.val
        improved_red.max_th = s_max_th.val
        improved_red.max_p = s_max_p.val
        improved_red.beta = s_beta.val

        # 更新目标队列长度范围
        improved_red.target_low = improved_red.min_th + 0.4 * (improved_red.max_th - improved_red.min_th)
        improved_red.target_high = improved_red.min_th + 0.6 * (improved_red.max_th - improved_red.min_th)

        # 重新计算概率
        traditional_probs = [
            0 if ql <= improved_red.min_th else
            1 if ql >= improved_red.max_th else
            improved_red.max_p * (ql - improved_red.min_th) /
            (improved_red.max_th - improved_red.min_th)
            for ql in queue_lengths
        ]
        improved_probs = [improved_red.s_shaped_probability(ql) for ql in queue_lengths]

        # 更新图形
        line1.set_ydata(traditional_probs)
        line2.set_ydata(improved_probs)

        # 重新计算max_p动态调整
        max_p_values = []
        queue_length = improved_red.min_th
        for t in time_points:
            queue_length += np.sin(t / 2) * 5
            improved_red.update_max_p(queue_length)
            max_p_values.append(improved_red.max_p)
        line3.set_ydata(max_p_values)

        fig.canvas.draw_idle()

    s_min_th.on_changed(update)
    s_max_th.on_changed(update)
    s_max_p.on_changed(update)
    s_beta.on_changed(update)

    # 添加重置按钮
    reset_ax = plt.axes([0.8, 0.1, 0.1, 0.04])
    reset_button = Button(reset_ax, 'Reset', color=axcolor, hovercolor='0.975')

    def reset(event):
        s_min_th.reset()
        s_max_th.reset()
        s_max_p.reset()
        s_beta.reset()

    reset_button.on_clicked(reset)

    plt.show()


if __name__ == "__main__":
    main()