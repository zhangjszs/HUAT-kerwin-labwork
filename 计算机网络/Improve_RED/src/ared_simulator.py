import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button

class AREDSimulator:
    def __init__(self, alpha=0.002, beta=0.02, max_p=0.1, avg_queue_length=0, min_th=5, max_th=15):
        self.alpha = alpha  # 激进型调整参数
        self.beta = beta  # 保守型调整参数
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

    def update_max_p(self, avg_queue_length):
        if avg_queue_length < self.min_th:
            # 激进型调整
            self.max_p -= self.alpha * self.max_p
        elif avg_queue_length > self.max_th:
            # 保守型调整
            self.max_p += self.beta * (1 - self.max_p)
        # 确保max_p在合理范围内
        self.max_p = max(0.01, min(self.max_p, 1.0))

def main():
    # 初始化ARED模拟器
    ared = AREDSimulator()

    # 创建图形界面
    fig, ax = plt.subplots(figsize=(10, 8))
    plt.subplots_adjust(left=0.1, bottom=0.35)

    # 生成初始数据
    queue_lengths = np.linspace(0, 30, 300)
    drop_probs = [ared.calculate_drop_probability(ql) for ql in queue_lengths]

    # 绘制初始曲线
    line, = ax.plot(queue_lengths, drop_probs, 'b-', lw=2)

    ax.set_xlabel('Queue Length')
    ax.set_ylabel('Drop Probability')
    ax.set_title('ARED Algorithm Drop Probability Curve')
    ax.grid(True)

    # 创建滑动条
    axcolor = 'lightgoldenrodyellow'
    ax_alpha = plt.axes([0.1, 0.25, 0.65, 0.03], facecolor=axcolor)
    ax_beta = plt.axes([0.1, 0.2, 0.65, 0.03], facecolor=axcolor)
    ax_max_p = plt.axes([0.1, 0.15, 0.65, 0.03], facecolor=axcolor)
    ax_min_th = plt.axes([0.1, 0.1, 0.65, 0.03], facecolor=axcolor)
    ax_max_th = plt.axes([0.1, 0.05, 0.65, 0.03], facecolor=axcolor)

    s_alpha = Slider(ax_alpha, 'Alpha', 0.001, 0.1, valinit=0.002)
    s_beta = Slider(ax_beta, 'Beta', 0.001, 0.1, valinit=0.02)
    s_max_p = Slider(ax_max_p, 'Max P', 0.01, 1.0, valinit=0.1)
    s_min_th = Slider(ax_min_th, 'Min Th', 0, 20, valinit=5)
    s_max_th = Slider(ax_max_th, 'Max Th', 5, 40, valinit=15)

    # 更新函数
    def update(val):
        # 获取新的参数值
        ared.alpha = s_alpha.val
        ared.beta = s_beta.val
        ared.max_p = s_max_p.val
        ared.min_th = s_min_th.val
        ared.max_th = s_max_th.val

        # 确保max_th始终大于min_th
        if ared.max_th <= ared.min_th:
            ared.max_th = ared.min_th + 1
            s_max_th.set_val(ared.max_th)

        # 重新计算丢包概率
        drop_probs = [ared.calculate_drop_probability(ql) for ql in queue_lengths]
        line.set_ydata(drop_probs)

        # 重绘图形
        fig.canvas.draw_idle()

    # 注册更新函数
    s_alpha.on_changed(update)
    s_beta.on_changed(update)
    s_max_p.on_changed(update)
    s_min_th.on_changed(update)
    s_max_th.on_changed(update)

    # 添加重置按钮
    reset_ax = plt.axes([0.8, 0.1, 0.1, 0.04])
    reset_button = Button(reset_ax, 'Reset', color=axcolor, hovercolor='0.975')

    def reset(event):
        s_alpha.reset()
        s_beta.reset()
        s_max_p.reset()
        s_min_th.reset()
        s_max_th.reset()

    reset_button.on_clicked(reset)

    plt.show()


if __name__ == "__main__":
    main()