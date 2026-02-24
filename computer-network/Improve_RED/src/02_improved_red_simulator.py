import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button


class ImprovedREDSimulator:
    def __init__(self, max_p=0.1, min_th=10, max_th=30, beta=3):
        self.max_p = max_p
        self.min_th = min_th
        self.max_th = max_th
        self.beta = beta  # S型函数的斜率参数
        self.target_low = min_th + 0.4 * (max_th - min_th)   # 参数A
        self.target_high = min_th + 0.6 * (max_th - min_th)  # 参数B

    def s_shaped_probability(self, queue_length):
        """使用S型升半哥西分布函数计算丢包概率"""
        if queue_length <= self.min_th:
            return 0
        elif queue_length >= 2 * self.max_th:
            return 1
        else:
            b = self.beta
            numerator = self.max_p * (queue_length - self.min_th) ** b
            denominator = ((1 - self.max_p) * (2 * self.max_th - self.min_th) ** b +
                           self.max_p * (queue_length - self.max_th) ** b)
            return numerator / denominator

    def sigmoid_probability(self, queue_length, k=0.1):
        """Sigmoid 丢包概率（参考曲线）"""
        mid_point = (self.min_th + self.max_th) / 2
        scale = (self.max_th - self.min_th) / 4
        return self.max_p / (1 + np.exp(-k * (queue_length - mid_point) / scale))

    def traditional_probability(self, queue_length):
        """传统 RED 线性丢包概率"""
        if queue_length <= self.min_th:
            return 0
        elif queue_length >= self.max_th:
            return 1
        else:
            return self.max_p * (queue_length - self.min_th) / (self.max_th - self.min_th)

    def update_max_p(self, queue_length):
        """动态自适应调整 max_p"""
        if queue_length > self.target_high:
            delta = (queue_length - self.target_high) / (2 * self.max_th - self.min_th)
            self.max_p += delta
        elif queue_length < self.target_low:
            delta = (self.target_low - queue_length) / (2 * self.max_th - self.min_th)
            self.max_p *= (1 - delta)
        self.max_p = max(0.01, min(self.max_p, 1.0))


def main():
    sim = ImprovedREDSimulator()

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 12))
    plt.subplots_adjust(left=0.1, bottom=0.25, hspace=0.3)

    # --- 子图1: 三条丢包概率曲线对比 ---
    queue_lengths = np.linspace(0, 60, 600)

    trad_probs = [sim.traditional_probability(ql) for ql in queue_lengths]
    improved_probs = [sim.s_shaped_probability(ql) for ql in queue_lengths]
    sigmoid_probs = [sim.sigmoid_probability(ql) for ql in queue_lengths]

    line_trad, = ax1.plot(queue_lengths, trad_probs, 'b-', lw=2, label='Traditional RED')
    line_improved, = ax1.plot(queue_lengths, improved_probs, 'r-', lw=2, label='Improved RED (S-shaped)')
    line_sigmoid, = ax1.plot(queue_lengths, sigmoid_probs, 'g--', lw=2, label='Sigmoid Reference')
    ax1.set_xlabel('Queue Length')
    ax1.set_ylabel('Drop Probability')
    ax1.set_title('Traditional RED vs Improved RED vs Sigmoid')
    ax1.legend()
    ax1.grid(True)

    # --- 子图2: max_p 动态调整轨迹 ---
    time_points = np.linspace(0, 50, 500)

    def simulate_max_p_trajectory(s):
        """模拟正弦队列变化下的 max_p 调整"""
        tmp = ImprovedREDSimulator(s.max_p, s.min_th, s.max_th, s.beta)
        max_p_values = []
        q = tmp.min_th
        for t in time_points:
            q += np.sin(t / 2) * 5
            q = max(0, q)
            tmp.update_max_p(q)
            max_p_values.append(tmp.max_p)
        return max_p_values

    max_p_vals = simulate_max_p_trajectory(sim)
    line_maxp, = ax2.plot(time_points, max_p_vals, 'g-', lw=2)
    ax2.set_xlabel('Time')
    ax2.set_ylabel('Max Drop Probability')
    ax2.set_title('Dynamic Adjustment of max_p')
    ax2.grid(True)

    # --- 滑动条 ---
    axcolor = 'lightgoldenrodyellow'
    ax_beta = plt.axes([0.1, 0.15, 0.65, 0.03], facecolor=axcolor)
    ax_min_th = plt.axes([0.1, 0.10, 0.65, 0.03], facecolor=axcolor)
    ax_max_th = plt.axes([0.1, 0.05, 0.65, 0.03], facecolor=axcolor)
    ax_max_p = plt.axes([0.1, 0.00, 0.65, 0.03], facecolor=axcolor)

    s_beta = Slider(ax_beta, 'Beta', 1, 5, valinit=3)
    s_min_th = Slider(ax_min_th, 'Min Th', 0, 30, valinit=10)
    s_max_th = Slider(ax_max_th, 'Max Th', 10, 60, valinit=30)
    s_max_p = Slider(ax_max_p, 'Initial Max P', 0.01, 1.0, valinit=0.1)

    def update(val):
        sim.min_th = s_min_th.val
        sim.max_th = s_max_th.val
        sim.max_p = s_max_p.val
        sim.beta = s_beta.val
        sim.target_low = sim.min_th + 0.4 * (sim.max_th - sim.min_th)
        sim.target_high = sim.min_th + 0.6 * (sim.max_th - sim.min_th)

        if sim.max_th <= sim.min_th:
            sim.max_th = sim.min_th + 1
            s_max_th.set_val(sim.max_th)

        trad_probs = [sim.traditional_probability(ql) for ql in queue_lengths]
        improved_probs = [sim.s_shaped_probability(ql) for ql in queue_lengths]
        sigmoid_probs = [sim.sigmoid_probability(ql) for ql in queue_lengths]
        line_trad.set_ydata(trad_probs)
        line_improved.set_ydata(improved_probs)
        line_sigmoid.set_ydata(sigmoid_probs)

        max_p_vals = simulate_max_p_trajectory(sim)
        line_maxp.set_ydata(max_p_vals)
        ax2.relim()
        ax2.autoscale_view()

        fig.canvas.draw_idle()

    s_beta.on_changed(update)
    s_min_th.on_changed(update)
    s_max_th.on_changed(update)
    s_max_p.on_changed(update)

    reset_ax = plt.axes([0.8, 0.10, 0.1, 0.04])
    reset_button = Button(reset_ax, 'Reset', color=axcolor, hovercolor='0.975')

    def reset(event):
        s_beta.reset()
        s_min_th.reset()
        s_max_th.reset()
        s_max_p.reset()

    reset_button.on_clicked(reset)
    plt.show()


if __name__ == "__main__":
    main()
