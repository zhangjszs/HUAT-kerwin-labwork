import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button


class ImprovedREDSimulator:
    """改进型 RED 算法模拟器
    
    使用 S 型升半哥西分布函数计算丢包概率，并动态自适应调整 max_p。
    """
    def __init__(self, max_p=0.1, min_th=10, max_th=30, beta=3):
        self.max_p = max_p
        self.min_th = min_th
        self.max_th = max_th
        self.beta = beta  # S 型函数的幂参数（控制陡峭程度）
        self.target_low = min_th + 0.4 * (max_th - min_th)   # 目标区间下界 (A)
        self.target_high = min_th + 0.6 * (max_th - min_th)  # 目标区间上界 (B)

    def s_shaped_probability(self, queue_length):
        """使用 S 型升半哥西分布函数计算丢包概率

        采用标准广义升半哥西形式，半值点设在 max_th 处：
            P(x) = max_p * x^p / (a^p + max_p * x^p)
        其中 x = queue_length - min_th, a = max_th - min_th
        这样在 max_th 处概率约等于 max_p（小 max_p 时），并平滑增长到 1。
        """
        if queue_length <= self.min_th:
            return 0.0
        elif queue_length >= 2 * self.max_th:
            return 1.0
        else:
            b = self.beta
            x = queue_length - self.min_th
            a = self.max_th - self.min_th
            if a == 0:
                return 1.0 if queue_length > self.min_th else 0.0
            numerator = self.max_p * (x ** b)
            denominator = (a ** b) + self.max_p * (x ** b)
            return numerator / denominator

    def sigmoid_probability(self, queue_length, k=0.1):
        """Sigmoid 丢包概率（参考曲线）"""
        mid_point = (self.min_th + self.max_th) / 2
        scale = (self.max_th - self.min_th) / 4
        if scale == 0:
            return 1.0 if queue_length >= mid_point else 0.0
        return self.max_p / (1 + np.exp(-k * (queue_length - mid_point) / scale))

    def traditional_probability(self, queue_length):
        """传统 RED 线性丢包概率"""
        if queue_length <= self.min_th:
            return 0.0
        elif queue_length >= self.max_th:
            return 1.0
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


def simulate_max_p_trajectory(sim, time_points):
    """模拟正弦队列变化下的 max_p 调整轨迹"""
    tmp = ImprovedREDSimulator(sim.max_p, sim.min_th, sim.max_th, sim.beta)
    max_p_values = []
    queue_trace = []
    q = tmp.min_th + (tmp.max_th - tmp.min_th) / 2
    for t in time_points:
        # 模拟队列在阈值附近的波动
        q += np.sin(t / 3) * 3
        q = max(0, q)
        tmp.update_max_p(q)
        max_p_values.append(tmp.max_p)
        queue_trace.append(q)
    return max_p_values, queue_trace


def main():
    sim = ImprovedREDSimulator()

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(11, 10))
    plt.subplots_adjust(left=0.1, bottom=0.32, hspace=0.35)

    queue_lengths = np.linspace(0, 80, 800)

    trad_probs = [sim.traditional_probability(ql) for ql in queue_lengths]
    improved_probs = [sim.s_shaped_probability(ql) for ql in queue_lengths]
    sigmoid_probs = [sim.sigmoid_probability(ql) for ql in queue_lengths]

    # --- 子图1: 三条丢包概率曲线对比 ---
    line_trad, = ax1.plot(queue_lengths, trad_probs, 'b-', lw=2, label='Traditional RED')
    line_improved, = ax1.plot(queue_lengths, improved_probs, 'r-', lw=2.5, label='Improved RED (S-shaped)')
    line_sigmoid, = ax1.plot(queue_lengths, sigmoid_probs, 'g--', lw=2, label='Sigmoid Reference')
    ax1.axvline(x=sim.min_th, color='gray', linestyle=':', alpha=0.7, label=f'min_th={sim.min_th}')
    ax1.axvline(x=sim.max_th, color='purple', linestyle=':', alpha=0.7, label=f'max_th={sim.max_th}')
    ax1.axvline(x=2*sim.max_th, color='orange', linestyle=':', alpha=0.7, label=f'2×max_th={2*sim.max_th}')
    ax1.set_xlabel('Queue Length', fontsize=11)
    ax1.set_ylabel('Drop Probability', fontsize=11)
    ax1.set_title('Traditional RED vs Improved RED vs Sigmoid', fontsize=13, fontweight='bold')
    ax1.set_xlim(0, 80)
    ax1.set_ylim(-0.05, 1.05)
    ax1.legend(loc='upper left', fontsize=9)
    ax1.grid(True, alpha=0.3)

    # --- 子图2: max_p 动态调整轨迹 + 队列变化 ---
    time_points = np.linspace(0, 100, 1000)
    max_p_vals, q_trace = simulate_max_p_trajectory(sim, time_points)

    line_maxp, = ax2.plot(time_points, max_p_vals, 'g-', lw=2, label='max_p')
    ax2_twin = ax2.twinx()
    line_queue, = ax2_twin.plot(time_points, q_trace, 'c--', lw=1.5, alpha=0.6, label='Queue Length')

    ax2.set_xlabel('Time', fontsize=11)
    ax2.set_ylabel('Max Drop Probability', color='g', fontsize=11)
    ax2_twin.set_ylabel('Queue Length', color='c', fontsize=11)
    ax2.set_title('Dynamic Adjustment of max_p', fontsize=13, fontweight='bold')
    lines = [line_maxp, line_queue]
    ax2.legend(lines, [l.get_label() for l in lines], loc='upper right', fontsize=9)
    ax2.grid(True, alpha=0.3)

    # --- 滑动条 ---
    axcolor = 'lightgoldenrodyellow'
    slider_width = 0.6
    slider_left = 0.15
    slider_bottom_start = 0.20
    slider_height = 0.025
    slider_gap = 0.05

    ax_beta = plt.axes([slider_left, slider_bottom_start, slider_width, slider_height], facecolor=axcolor)
    ax_min_th = plt.axes([slider_left, slider_bottom_start - slider_gap, slider_width, slider_height], facecolor=axcolor)
    ax_max_th = plt.axes([slider_left, slider_bottom_start - 2*slider_gap, slider_width, slider_height], facecolor=axcolor)
    ax_max_p = plt.axes([slider_left, slider_bottom_start - 3*slider_gap, slider_width, slider_height], facecolor=axcolor)

    s_beta = Slider(ax_beta, 'Beta (power)', 1, 10, valinit=3)
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

        # 更新阈值线
        for child in ax1.collections + ax1.lines:
            if child.get_linestyle() == ':' and child not in [line_trad, line_improved, line_sigmoid]:
                child.remove()
        ax1.axvline(x=sim.min_th, color='gray', linestyle=':', alpha=0.7)
        ax1.axvline(x=sim.max_th, color='purple', linestyle=':', alpha=0.7)
        ax1.axvline(x=2*sim.max_th, color='orange', linestyle=':', alpha=0.7)

        max_p_vals, q_trace = simulate_max_p_trajectory(sim, time_points)
        line_maxp.set_ydata(max_p_vals)
        line_queue.set_ydata(q_trace)
        ax2.relim()
        ax2.autoscale_view()
        ax2_twin.relim()
        ax2_twin.autoscale_view()

        fig.canvas.draw_idle()

    s_beta.on_changed(update)
    s_min_th.on_changed(update)
    s_max_th.on_changed(update)
    s_max_p.on_changed(update)

    reset_ax = plt.axes([0.82, 0.14, 0.1, 0.04])
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
