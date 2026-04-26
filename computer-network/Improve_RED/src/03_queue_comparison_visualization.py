import matplotlib.pyplot as plt
import numpy as np


def red_algorithm(avg_queue_length, min_th=10, max_th=50, max_p=0.1):
    """标准 RED 算法丢包概率
    
    基于平均队列长度的线性增长概率，超过 max_th 强制丢弃。
    """
    if avg_queue_length < min_th:
        return 0.0
    elif avg_queue_length >= max_th:
        return 1.0
    else:
        return ((avg_queue_length - min_th) / (max_th - min_th)) * max_p


def ared_algorithm(avg_queue_length, min_th=10, max_th=50, state=None):
    """自适应 RED (ARED) 算法
    
    根据平均队列长度自适应调整 max_p，保持队列在目标区间内。
    """
    if state is None:
        state = {'max_p': 0.1, 'alpha': 0.002, 'beta': 0.02}

    # 自适应调整 max_p
    if avg_queue_length < min_th:
        state['max_p'] -= state['alpha'] * state['max_p']
    elif avg_queue_length >= max_th:
        state['max_p'] += state['beta'] * (1 - state['max_p'])
    state['max_p'] = max(0.01, min(state['max_p'], 1.0))

    # 计算丢包概率
    if avg_queue_length < min_th:
        return 0.0
    elif avg_queue_length >= max_th:
        return 1.0
    else:
        return ((avg_queue_length - min_th) / (max_th - min_th)) * state['max_p']


def s_shaped_function(avg_queue_length, min_th, max_th, max_p, p=3):
    """S 型升半哥西分布函数丢包概率（修正版）

    采用标准广义升半哥西形式，半值点设在 max_th 处：
        P(x) = max_p * x^p / (a^p + max_p * x^p)
    其中 x = avg - min_th, a = max_th - min_th
    这样在 max_th 处概率约等于 max_p，并平滑增长到 1。
    """
    if avg_queue_length <= min_th:
        return 0.0
    elif avg_queue_length >= 2 * max_th:
        return 1.0
    else:
        x = avg_queue_length - min_th
        a = max_th - min_th
        if a == 0:
            return 1.0 if avg_queue_length > min_th else 0.0
        numerator = max_p * (x ** p)
        denominator = (a ** p) + max_p * (x ** p)
        return numerator / denominator


def adaptive_max_p(avg_queue_length, min_th, max_th, max_p):
    """改进 RED 的动态自适应调整 max_p"""
    A = min_th + 0.4 * (max_th - min_th)
    B = min_th + 0.6 * (max_th - min_th)
    if avg_queue_length > B:
        max_p += (avg_queue_length - B) / (2 * max_th - min_th)
    elif avg_queue_length < A:
        max_p *= (1 - (A - avg_queue_length) / (2 * max_th - min_th))
    return max(0.01, min(max_p, 1.0))


def improved_red_algorithm(avg_queue_length, min_th=10, max_th=50, state=None, p=3):
    """改进 RED 算法：S 型概率 + 自适应 max_p"""
    if state is None:
        state = {'max_p': 0.1}
    state['max_p'] = adaptive_max_p(avg_queue_length, min_th, max_th, state['max_p'])
    return s_shaped_function(avg_queue_length, min_th, max_th, state['max_p'], p)


def run_simulation():
    """运行三种 RED 算法的队列仿真并可视化对比"""
    # 仿真参数
    min_threshold = 10
    max_threshold = 50
    max_probability = 0.1
    arrival_rate = 25      # 平均到达率
    service_rate = 22      # 平均服务率（轻微拥塞）
    simulation_time = 300  # 仿真时长
    wq = 0.002             # EWMA 权重（RED 标准推荐值）

    # 初始化队列和状态
    queue_length_red = []
    queue_length_ared = []
    queue_length_improved = []
    avg_queue_red = []
    avg_queue_ared = []
    avg_queue_improved = []
    ared_max_p_trace = []
    improved_max_p_trace = []
    drop_rate_red = []
    drop_rate_ared = []
    drop_rate_improved = []
    throughput_red = []
    throughput_ared = []
    throughput_improved = []

    current_queue_red = 0
    current_queue_ared = 0
    current_queue_improved = 0

    avg_q_red = 0.0
    avg_q_ared = 0.0
    avg_q_improved = 0.0

    ared_state = {'max_p': max_probability, 'alpha': 0.002, 'beta': 0.02}
    improved_state = {'max_p': max_probability}

    for t in range(simulation_time):
        # 生成新到达的包数和服务能力
        new_packets = np.random.poisson(arrival_rate)
        service_capacity = np.random.poisson(service_rate)

        # --- RED 处理 ---
        dropped_red = 0
        for _ in range(new_packets):
            avg_q_red = (1 - wq) * avg_q_red + wq * current_queue_red
            drop_prob = red_algorithm(avg_q_red, min_threshold, max_threshold, max_probability)
            if np.random.random() < drop_prob:
                dropped_red += 1
            else:
                current_queue_red += 1
        service_red = min(current_queue_red, service_capacity)
        current_queue_red -= service_red
        current_queue_red = max(0, current_queue_red)

        # --- ARED 处理 ---
        dropped_ared = 0
        for _ in range(new_packets):
            avg_q_ared = (1 - wq) * avg_q_ared + wq * current_queue_ared
            drop_prob = ared_algorithm(avg_q_ared, min_threshold, max_threshold, ared_state)
            if np.random.random() < drop_prob:
                dropped_ared += 1
            else:
                current_queue_ared += 1
        service_ared = min(current_queue_ared, service_capacity)
        current_queue_ared -= service_ared
        current_queue_ared = max(0, current_queue_ared)

        # --- Improved RED 处理 ---
        dropped_improved = 0
        for _ in range(new_packets):
            avg_q_improved = (1 - wq) * avg_q_improved + wq * current_queue_improved
            drop_prob = improved_red_algorithm(
                avg_q_improved, min_threshold, max_threshold, improved_state)
            if np.random.random() < drop_prob:
                dropped_improved += 1
            else:
                current_queue_improved += 1
        service_improved = min(current_queue_improved, service_capacity)
        current_queue_improved -= service_improved
        current_queue_improved = max(0, current_queue_improved)

        # 记录数据
        queue_length_red.append(current_queue_red)
        queue_length_ared.append(current_queue_ared)
        queue_length_improved.append(current_queue_improved)
        avg_queue_red.append(avg_q_red)
        avg_queue_ared.append(avg_q_ared)
        avg_queue_improved.append(avg_q_improved)
        ared_max_p_trace.append(ared_state['max_p'])
        improved_max_p_trace.append(improved_state['max_p'])

        total_new = max(1, new_packets)
        drop_rate_red.append(dropped_red / total_new)
        drop_rate_ared.append(dropped_ared / total_new)
        drop_rate_improved.append(dropped_improved / total_new)
        throughput_red.append(service_red)
        throughput_ared.append(service_ared)
        throughput_improved.append(service_improved)

    time_axis = np.arange(simulation_time)

    # --- 绘图 ---
    fig = plt.figure(figsize=(14, 12))
    gs = fig.add_gridspec(3, 2, hspace=0.35, wspace=0.25)

    ax_queue = fig.add_subplot(gs[0, :])
    ax_avg = fig.add_subplot(gs[1, 0])
    ax_maxp = fig.add_subplot(gs[1, 1])
    ax_drop = fig.add_subplot(gs[2, 0])
    ax_throughput = fig.add_subplot(gs[2, 1])

    # 1. 队列长度时序
    ax_queue.plot(time_axis, queue_length_red, label='RED', color='blue', alpha=0.8, lw=1.2)
    ax_queue.plot(time_axis, queue_length_ared, label='ARED', color='orange', alpha=0.8, lw=1.2)
    ax_queue.plot(time_axis, queue_length_improved, label='Improved RED', color='green', alpha=0.8, lw=1.2)
    ax_queue.axhline(y=min_threshold, color='gray', linestyle='--', alpha=0.6, label=f'Min Th ({min_threshold})')
    ax_queue.axhline(y=max_threshold, color='red', linestyle='--', alpha=0.6, label=f'Max Th ({max_threshold})')
    ax_queue.fill_between(time_axis, min_threshold, max_threshold, alpha=0.1, color='green', label='Target Zone')
    ax_queue.set_xlabel('Time Units', fontsize=11)
    ax_queue.set_ylabel('Queue Length', fontsize=11)
    ax_queue.set_title('Queue Length Comparison (Instantaneous)', fontsize=13, fontweight='bold')
    ax_queue.legend(loc='upper right', fontsize=9, ncol=2)
    ax_queue.grid(True, alpha=0.3)

    # 2. 平均队列长度
    ax_avg.plot(time_axis, avg_queue_red, label='RED', color='blue', alpha=0.8, lw=1.2)
    ax_avg.plot(time_axis, avg_queue_ared, label='ARED', color='orange', alpha=0.8, lw=1.2)
    ax_avg.plot(time_axis, avg_queue_improved, label='Improved RED', color='green', alpha=0.8, lw=1.2)
    ax_avg.axhline(y=min_threshold, color='gray', linestyle='--', alpha=0.6)
    ax_avg.axhline(y=max_threshold, color='red', linestyle='--', alpha=0.6)
    ax_avg.fill_between(time_axis, min_threshold, max_threshold, alpha=0.1, color='green')
    ax_avg.set_xlabel('Time Units', fontsize=11)
    ax_avg.set_ylabel('Average Queue Length (EWMA)', fontsize=11)
    ax_avg.set_title('Average Queue Length Comparison', fontsize=13, fontweight='bold')
    ax_avg.legend(loc='upper right', fontsize=9)
    ax_avg.grid(True, alpha=0.3)

    # 3. max_p 自适应轨迹
    ax_maxp.plot(time_axis, ared_max_p_trace, color='orange', lw=2, label='ARED max_p')
    ax_maxp.plot(time_axis, improved_max_p_trace, color='green', lw=2, label='Improved RED max_p')
    ax_maxp.set_xlabel('Time Units', fontsize=11)
    ax_maxp.set_ylabel('Max Drop Probability', fontsize=11)
    ax_maxp.set_title('Adaptive max_p Trajectory', fontsize=13, fontweight='bold')
    ax_maxp.legend(loc='upper right', fontsize=9)
    ax_maxp.grid(True, alpha=0.3)

    # 4. 丢包率
    window = 10
    drop_red_smooth = np.convolve(drop_rate_red, np.ones(window)/window, mode='same')
    drop_ared_smooth = np.convolve(drop_rate_ared, np.ones(window)/window, mode='same')
    drop_improved_smooth = np.convolve(drop_rate_improved, np.ones(window)/window, mode='same')
    ax_drop.plot(time_axis, drop_red_smooth, color='blue', lw=1.5, label='RED')
    ax_drop.plot(time_axis, drop_ared_smooth, color='orange', lw=1.5, label='ARED')
    ax_drop.plot(time_axis, drop_improved_smooth, color='green', lw=1.5, label='Improved RED')
    ax_drop.set_xlabel('Time Units', fontsize=11)
    ax_drop.set_ylabel('Drop Rate (smoothed)', fontsize=11)
    ax_drop.set_title('Packet Drop Rate Comparison', fontsize=13, fontweight='bold')
    ax_drop.legend(loc='upper right', fontsize=9)
    ax_drop.grid(True, alpha=0.3)

    # 5. 吞吐量
    tp_red_smooth = np.convolve(throughput_red, np.ones(window)/window, mode='same')
    tp_ared_smooth = np.convolve(throughput_ared, np.ones(window)/window, mode='same')
    tp_improved_smooth = np.convolve(throughput_improved, np.ones(window)/window, mode='same')
    ax_throughput.plot(time_axis, tp_red_smooth, color='blue', lw=1.5, label='RED')
    ax_throughput.plot(time_axis, tp_ared_smooth, color='orange', lw=1.5, label='ARED')
    ax_throughput.plot(time_axis, tp_improved_smooth, color='green', lw=1.5, label='Improved RED')
    ax_throughput.set_xlabel('Time Units', fontsize=11)
    ax_throughput.set_ylabel('Throughput (packets/slot)', fontsize=11)
    ax_throughput.set_title('Throughput Comparison', fontsize=13, fontweight='bold')
    ax_throughput.legend(loc='upper right', fontsize=9)
    ax_throughput.grid(True, alpha=0.3)

    # 打印统计摘要
    print("=" * 50)
    print("Simulation Statistics (last 100 time units)")
    print("=" * 50)
    for name, q, dr, tp in [
        ("RED", queue_length_red, drop_rate_red, throughput_red),
        ("ARED", queue_length_ared, drop_rate_ared, throughput_ared),
        ("Improved RED", queue_length_improved, drop_rate_improved, throughput_improved)
    ]:
        print(f"{name:15s} | Avg Queue: {np.mean(q[-100:]):6.1f} | "
              f"Drop Rate: {np.mean(dr[-100:])*100:5.2f}% | "
              f"Throughput: {np.mean(tp[-100:]):6.1f}")
    print("=" * 50)

    plt.show()


if __name__ == "__main__":
    run_simulation()
