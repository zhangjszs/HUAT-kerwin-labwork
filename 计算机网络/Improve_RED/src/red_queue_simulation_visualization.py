import matplotlib.pyplot as plt
import numpy as np

def red_algorithm(avg_queue_length, min_th=10, max_th=50, max_p=0.1):
    """
    计算RED算法的丢包概率
    :param avg_queue_length: 当前队列长度
    :param min_th: 最小阈值
    :param max_th: 最大阈值
    :param max_p: 最大丢包概率
    :return: 丢包概率
    """
    if avg_queue_length < min_th:
        return 0
    elif avg_queue_length >= max_th:
        return max_p
    else:
        return ((avg_queue_length - min_th) / (max_th - min_th)) * max_p

def ared_algorithm(avg_queue_length, min_th=10, max_th=50, max_p=0.1, alpha=0.01, beta=0.9):
    """
    计算 ARED 算法的丢包概率
    :param avg_queue_length: 当前队列长度
    :param min_th: 最小阈值
    :param max_th: 最大阈值
    :param max_p: 最大丢包概率
    :param alpha: 增加因子
    :param beta: 减少因子
    :return: 丢包概率
    """
    if avg_queue_length < min_th:
        return 0
    elif avg_queue_length >= max_th:
        return max_p
    else:
        return ((avg_queue_length - min_th) / (max_th - min_th)) * max_p

def s_shaped_function(avg_queue_length, min_th, max_th, max_p, p=3):
    """
    计算基于S型升半哥西分布函数的丢包概率
    :param avg_queue_length: 当前队列长度
    :param min_th: 最小阈值
    :param max_th: 最大阈值
    :param max_p: 最大丢包概率
    :param p: 参数p
    :return: 丢包概率
    """
    if avg_queue_length <= min_th:
        return 0
    elif avg_queue_length >= 2 * max_th:
        return 1
    else:
        a = max_p / ((1 - max_p) * (max_th - min_th)**p)
        return a * (avg_queue_length - min_th)**p / (1 + a * (avg_queue_length - min_th)**p)

def adaptive_max_p(avg_queue_length, min_th, max_th, max_p):
    """
    动态自适应调整最大丢包概率
    :param avg_queue_length: 当前队列长度
    :param min_th: 最小阈值
    :param max_th: 最大阈值
    :param max_p: 最大丢包概率
    :return: 调整后的最大丢包概率
    """
    A = min_th + 0.4 * (max_th - min_th)
    B = min_th + 0.6 * (max_th - min_th)
    
    if avg_queue_length > B:
        max_p += (avg_queue_length - B) / (2 * max_th - min_th)
    elif avg_queue_length < A:
        max_p *= (1 - (A - avg_queue_length) / (2 * max_th - min_th))
    
    return max_p

def improved_red_algorithm(avg_queue_length, min_th=10, max_th=50, max_p=0.1, p=3):
    """
    计算改进的RED算法的丢包概率
    :param avg_queue_length: 当前队列长度
    :param min_th: 最小阈值
    :param max_th: 最大阈值
    :param max_p: 最大丢包概率
    :param p: 参数p
    :return: 丢包概率
    """
    # 动态自适应调整最大丢包概率
    max_p = adaptive_max_p(avg_queue_length, min_th, max_th, max_p)
    
    # 使用S型升半哥西分布函数计算丢包概率
    drop_prob = s_shaped_function(avg_queue_length, min_th, max_th, max_p, p)
    
    return drop_prob

# RED算法参数
min_threshold = 10
max_threshold = 50
max_probability = 0.1

# 模拟参数
arrival_rate = 20  # 每时间单位的到达包数
service_rate = 20  # 每时间单位的处理包数
simulation_time = 100  # 模拟时间单位数

# 初始化变量
queue_length_red = []
queue_length_ared = []
queue_length_improved_red = []
current_queue_red = 0
current_queue_ared = 0
current_queue_improved_red = 0

for t in range(simulation_time):
    # 新到达的包
    new_packets = np.random.poisson(arrival_rate)
    
    # 服务完成的包
    service_completions_red = min(current_queue_red, np.random.poisson(service_rate))
    service_completions_ared = min(current_queue_ared, np.random.poisson(service_rate))
    service_completions_improved_red = min(current_queue_improved_red, np.random.poisson(service_rate))
    
    # 更新队列长度
    current_queue_red += new_packets - service_completions_red
    current_queue_ared += new_packets - service_completions_ared
    current_queue_improved_red += new_packets - service_completions_improved_red
    
    # 使用 RED 算法计算丢包概率
    drop_prob_red = red_algorithm(current_queue_red, min_threshold, max_threshold, max_probability)
    
    # 使用 ARED 算法计算丢包概率
    drop_prob_ared = ared_algorithm(current_queue_ared, min_threshold, max_threshold, max_probability)
    
    # 使用 改进RED 算法计算丢包概率
    drop_prob_improved_red = improved_red_algorithm(current_queue_improved_red, min_threshold, max_threshold, max_probability)
    
    # 根据计算的丢包概率随机丢弃包
    dropped_packets_red = np.random.binomial(new_packets, drop_prob_red)
    dropped_packets_ared = np.random.binomial(new_packets, drop_prob_ared)
    dropped_packets_improved_red = np.random.binomial(new_packets, drop_prob_improved_red)
    current_queue_red -= dropped_packets_red
    current_queue_ared -= dropped_packets_ared
    current_queue_improved_red -= dropped_packets_improved_red
    
    # 存储当前时间步的队列长度
    queue_length_red.append(current_queue_red)
    queue_length_ared.append(current_queue_ared)
    queue_length_improved_red.append(current_queue_improved_red)

# 绘图
plt.figure(figsize=(12, 6))
plt.plot(queue_length_red, label='RED Queue Length', color='blue')
plt.plot(queue_length_ared, label='ARED Queue Length', color='orange')
plt.plot(queue_length_improved_red, label='Improved RED Queue Length', color='green')
plt.axhline(y=min_threshold, color='green', linestyle='--', label=f'Min Threshold ({min_threshold})')
plt.axhline(y=max_threshold, color='red', linestyle='--', label=f'MAX Threshold ({max_threshold})')

# 添加标签和标题
plt.xlabel('Time Units')
plt.ylabel('Queue Length')
plt.title('RED, ARED, SRED')
plt.legend()
plt.grid(True)
plt.show()






