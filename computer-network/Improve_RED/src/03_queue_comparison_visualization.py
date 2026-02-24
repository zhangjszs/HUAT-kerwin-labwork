import matplotlib.pyplot as plt
import numpy as np


def red_algorithm(avg_queue_length, min_th=10, max_th=50, max_p=0.1):
    """RED 算法丢包概率"""
    if avg_queue_length < min_th:
        return 0
    elif avg_queue_length >= max_th:
        return max_p
    else:
        return ((avg_queue_length - min_th) / (max_th - min_th)) * max_p


def ared_algorithm(avg_queue_length, min_th=10, max_th=50, state=None):
    """
    真正的 ARED 算法：用 state dict 持久化 max_p，每步根据队列状态自适应调整。
    state 必须包含: max_p, alpha, beta
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
        return 0
    elif avg_queue_length >= max_th:
        return state['max_p']
    else:
        return ((avg_queue_length - min_th) / (max_th - min_th)) * state['max_p']


def s_shaped_function(avg_queue_length, min_th, max_th, max_p, p=3):
    """S型升半哥西分布函数丢包概率"""
    if avg_queue_length <= min_th:
        return 0
    elif avg_queue_length >= 2 * max_th:
        return 1
    else:
        numerator = max_p * (avg_queue_length - min_th) ** p
        denominator = ((1 - max_p) * (2 * max_th - min_th) ** p +
                       max_p * (avg_queue_length - max_th) ** p)
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
    """改进 RED 算法：S型概率 + 自适应 max_p"""
    if state is None:
        state = {'max_p': 0.1}
    state['max_p'] = adaptive_max_p(avg_queue_length, min_th, max_th, state['max_p'])
    return s_shaped_function(avg_queue_length, min_th, max_th, state['max_p'], p)


def run_simulation():
    min_threshold = 10
    max_threshold = 50
    max_probability = 0.1
    arrival_rate = 20
    service_rate = 20
    simulation_time = 200

    # 初始化队列和状态
    queue_length_red = []
    queue_length_ared = []
    queue_length_improved = []
    ared_max_p_trace = []

    current_queue_red = 0
    current_queue_ared = 0
    current_queue_improved = 0

    ared_state = {'max_p': max_probability, 'alpha': 0.002, 'beta': 0.02}
    improved_state = {'max_p': max_probability}

    for t in range(simulation_time):
        new_packets = np.random.poisson(arrival_rate)
        service_red = min(current_queue_red, np.random.poisson(service_rate))
        service_ared = min(current_queue_ared, np.random.poisson(service_rate))
        service_improved = min(current_queue_improved, np.random.poisson(service_rate))

        current_queue_red += new_packets - service_red
        current_queue_ared += new_packets - service_ared
        current_queue_improved += new_packets - service_improved

        drop_prob_red = red_algorithm(current_queue_red, min_threshold, max_threshold, max_probability)
        drop_prob_ared = ared_algorithm(current_queue_ared, min_threshold, max_threshold, ared_state)
        drop_prob_improved = improved_red_algorithm(
            current_queue_improved, min_threshold, max_threshold, improved_state)

        dropped_red = np.random.binomial(new_packets, drop_prob_red)
        dropped_ared = np.random.binomial(new_packets, drop_prob_ared)
        dropped_improved = np.random.binomial(new_packets, drop_prob_improved)

        current_queue_red = max(0, current_queue_red - dropped_red)
        current_queue_ared = max(0, current_queue_ared - dropped_ared)
        current_queue_improved = max(0, current_queue_improved - dropped_improved)

        queue_length_red.append(current_queue_red)
        queue_length_ared.append(current_queue_ared)
        queue_length_improved.append(current_queue_improved)
        ared_max_p_trace.append(ared_state['max_p'])

    # --- 绘图 ---
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 9), height_ratios=[2, 1])
    plt.subplots_adjust(hspace=0.3)

    ax1.plot(queue_length_red, label='RED', color='blue')
    ax1.plot(queue_length_ared, label='ARED', color='orange')
    ax1.plot(queue_length_improved, label='Improved RED', color='green')
    ax1.axhline(y=min_threshold, color='gray', linestyle='--', label=f'Min Th ({min_threshold})')
    ax1.axhline(y=max_threshold, color='red', linestyle='--', label=f'Max Th ({max_threshold})')
    ax1.set_xlabel('Time Units')
    ax1.set_ylabel('Queue Length')
    ax1.set_title('RED / ARED / Improved RED Queue Length Comparison')
    ax1.legend()
    ax1.grid(True)

    ax2.plot(ared_max_p_trace, color='orange', lw=2)
    ax2.set_xlabel('Time Units')
    ax2.set_ylabel('ARED max_p')
    ax2.set_title('ARED max_p Adaptive Adjustment Trajectory')
    ax2.grid(True)

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    run_simulation()
