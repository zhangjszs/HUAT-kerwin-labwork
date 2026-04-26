import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button


class REDSimulator:
    """传统 RED 算法模拟器"""
    def __init__(self, max_p=0.1, min_th=5, max_th=15):
        self.max_p = max_p
        self.min_th = min_th
        self.max_th = max_th

    def calculate_drop_probability(self, queue_length):
        """计算丢包概率：标准 RED 使用线性增长，超过 max_th 强制丢弃"""
        if queue_length < self.min_th:
            return 0.0
        elif queue_length >= self.max_th:
            return 1.0
        else:
            return self.max_p * (queue_length - self.min_th) / (self.max_th - self.min_th)


class AREDSimulator:
    """自适应 RED (ARED) 算法模拟器
    
    ARED 动态调整 max_p 以保持队列在目标范围内。
    """
    def __init__(self, alpha=0.002, beta=0.02, max_p=0.1, min_th=5, max_th=15):
        self.alpha = alpha  # 队列偏小时 max_p 的衰减系数
        self.beta = beta    # 队列偏大时 max_p 的增长系数
        self.max_p = max_p
        self.min_th = min_th
        self.max_th = max_th

    def calculate_drop_probability(self, queue_length):
        """计算丢包概率"""
        if queue_length < self.min_th:
            return 0.0
        elif queue_length >= self.max_th:
            return 1.0
        else:
            return self.max_p * (queue_length - self.min_th) / (self.max_th - self.min_th)

    def update_max_p(self, avg_queue_length):
        """根据平均队列长度自适应调整 max_p"""
        if avg_queue_length < self.min_th:
            self.max_p -= self.alpha * self.max_p
        elif avg_queue_length > self.max_th:
            self.max_p += self.beta * (1 - self.max_p)
        self.max_p = max(0.01, min(self.max_p, 1.0))


def simulate_ared_trajectory(ared_sim, time_points):
    """模拟正弦队列长度变化下的 max_p 调整轨迹"""
    sim = AREDSimulator(
        ared_sim.alpha, ared_sim.beta, ared_sim.max_p,
        ared_sim.min_th, ared_sim.max_th
    )
    max_p_values = []
    queue_trace = []
    for t in time_points:
        # 生成在阈值附近波动的队列长度
        amplitude = (sim.max_th - sim.min_th) * 0.8
        offset = (sim.max_th + sim.min_th) / 2
        q = offset + amplitude * np.sin(t / 5)
        q = max(0, q)
        sim.update_max_p(q)
        max_p_values.append(sim.max_p)
        queue_trace.append(q)
    return max_p_values, queue_trace


def main():
    red = REDSimulator()
    ared = AREDSimulator()

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(11, 10))
    # 为底部滑动条留出充足空间
    plt.subplots_adjust(left=0.1, bottom=0.38, hspace=0.35)

    queue_lengths = np.linspace(0, 40, 400)
    red_probs = [red.calculate_drop_probability(ql) for ql in queue_lengths]
    ared_probs = [ared.calculate_drop_probability(ql) for ql in queue_lengths]

    # --- 子图1: RED vs ARED 丢包概率曲线 ---
    line_red, = ax1.plot(queue_lengths, red_probs, 'b-', lw=2.5, label='RED')
    line_ared, = ax1.plot(queue_lengths, ared_probs, 'r--', lw=2.5, label='ARED (current max_p)')
    ax1.axvline(x=red.min_th, color='gray', linestyle=':', alpha=0.7, label=f'min_th={red.min_th}')
    ax1.axvline(x=red.max_th, color='purple', linestyle=':', alpha=0.7, label=f'max_th={red.max_th}')
    ax1.set_xlabel('Queue Length', fontsize=11)
    ax1.set_ylabel('Drop Probability', fontsize=11)
    ax1.set_title('RED vs ARED Drop Probability Curves', fontsize=13, fontweight='bold')
    ax1.set_xlim(0, 40)
    ax1.set_ylim(-0.05, 1.05)
    ax1.legend(loc='upper left', fontsize=9)
    ax1.grid(True, alpha=0.3)

    # --- 子图2: ARED max_p 自适应调整轨迹 ---
    time_points = np.linspace(0, 80, 800)
    max_p_vals, q_trace = simulate_ared_trajectory(ared, time_points)

    line_maxp, = ax2.plot(time_points, max_p_vals, 'g-', lw=2, label='max_p')
    ax2_twin = ax2.twinx()
    line_queue, = ax2_twin.plot(time_points, q_trace, 'c--', lw=1.5, alpha=0.6, label='Queue Length')

    ax2.set_xlabel('Time', fontsize=11)
    ax2.set_ylabel('max_p', color='g', fontsize=11)
    ax2_twin.set_ylabel('Queue Length', color='c', fontsize=11)
    ax2.set_title('ARED max_p Adaptive Adjustment Trajectory', fontsize=13, fontweight='bold')
    lines = [line_maxp, line_queue]
    ax2.legend(lines, [l.get_label() for l in lines], loc='upper right', fontsize=9)
    ax2.grid(True, alpha=0.3)

    # --- 滑动条 ---
    axcolor = 'lightgoldenrodyellow'
    slider_width = 0.6
    slider_left = 0.15
    slider_bottom_start = 0.26
    slider_height = 0.025
    slider_gap = 0.05

    ax_max_p = plt.axes([slider_left, slider_bottom_start, slider_width, slider_height], facecolor=axcolor)
    ax_min_th = plt.axes([slider_left, slider_bottom_start - slider_gap, slider_width, slider_height], facecolor=axcolor)
    ax_max_th = plt.axes([slider_left, slider_bottom_start - 2*slider_gap, slider_width, slider_height], facecolor=axcolor)
    ax_alpha = plt.axes([slider_left, slider_bottom_start - 3*slider_gap, slider_width, slider_height], facecolor=axcolor)
    ax_beta = plt.axes([slider_left, slider_bottom_start - 4*slider_gap, slider_width, slider_height], facecolor=axcolor)

    s_max_p = Slider(ax_max_p, 'Max P', 0.01, 1.0, valinit=0.1)
    s_min_th = Slider(ax_min_th, 'Min Th', 0, 20, valinit=5)
    s_max_th = Slider(ax_max_th, 'Max Th', 5, 40, valinit=15)
    s_alpha = Slider(ax_alpha, 'Alpha', 0.001, 0.1, valinit=0.002)
    s_beta = Slider(ax_beta, 'Beta', 0.001, 0.1, valinit=0.02)

    def update(val):
        red.max_p = s_max_p.val
        red.min_th = s_min_th.val
        red.max_th = s_max_th.val
        ared.max_p = s_max_p.val
        ared.min_th = s_min_th.val
        ared.max_th = s_max_th.val
        ared.alpha = s_alpha.val
        ared.beta = s_beta.val

        # 防止 max_th <= min_th
        if red.max_th <= red.min_th:
            red.max_th = red.min_th + 1
            ared.max_th = red.max_th
            s_max_th.set_val(red.max_th)

        # 更新子图1
        red_probs = [red.calculate_drop_probability(ql) for ql in queue_lengths]
        ared_probs = [ared.calculate_drop_probability(ql) for ql in queue_lengths]
        line_red.set_ydata(red_probs)
        line_ared.set_ydata(ared_probs)
        # 更新阈值线
        for child in ax1.collections + ax1.lines:
            if child.get_linestyle() == ':' and child not in [line_red, line_ared]:
                child.remove()
        ax1.axvline(x=red.min_th, color='gray', linestyle=':', alpha=0.7)
        ax1.axvline(x=red.max_th, color='purple', linestyle=':', alpha=0.7)

        # 更新子图2
        max_p_vals, q_trace = simulate_ared_trajectory(ared, time_points)
        line_maxp.set_ydata(max_p_vals)
        line_queue.set_ydata(q_trace)
        ax2.relim()
        ax2.autoscale_view()
        ax2_twin.relim()
        ax2_twin.autoscale_view()

        fig.canvas.draw_idle()

    s_max_p.on_changed(update)
    s_min_th.on_changed(update)
    s_max_th.on_changed(update)
    s_alpha.on_changed(update)
    s_beta.on_changed(update)

    reset_ax = plt.axes([0.82, 0.18, 0.1, 0.04])
    reset_button = Button(reset_ax, 'Reset', color=axcolor, hovercolor='0.975')

    def reset(event):
        s_max_p.reset()
        s_min_th.reset()
        s_max_th.reset()
        s_alpha.reset()
        s_beta.reset()

    reset_button.on_clicked(reset)
    plt.show()


if __name__ == "__main__":
    main()
