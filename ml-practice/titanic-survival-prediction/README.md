# Titanic 生存预测分析

基于经典 Titanic 数据集的二分类预测项目，完整展示数据科学建模全流程。

## 项目背景

泰坦尼克号沉船事故中，部分乘客得以生还。本项目通过乘客的年龄、性别、舱位等信息，
建立机器学习模型预测生存概率，并深入分析影响生存的关键因素。

## 技术栈

- Python 3.10+
- pandas, numpy（数据处理）
- matplotlib, seaborn（数据可视化）
- scikit-learn（逻辑回归、随机森林、模型评估）
- xgboost（梯度提升模型）

## 关键结论

1. **性别是最强预测因子**：女性生存率约74%，男性约19%
2. **XGBoost 表现最优**：5折交叉验证 F1 约0.76，AUC 约0.86
3. **构造特征有效**：family_size 特征在随机森林中排名前5，验证了"家庭规模影响生存"的假设

## 运行方式

```bash
pip install pandas numpy matplotlib seaborn scikit-learn xgboost jupyter
jupyter notebook titanic_analysis.ipynb
```

## 文件说明

| 文件 | 说明 |
|------|------|
| `titanic_analysis.ipynb` | 完整分析 Notebook |
| `eda_survival.png` | EDA 生存率分布图（运行后生成） |
| `feature_correlation.png` | 特征相关性热力图（运行后生成） |
| `model_comparison.png` | 三模型 F1 对比图（运行后生成） |
| `feature_importance.png` | 随机森林特征重要性图（运行后生成） |
| `confusion_matrix.png` | XGBoost 混淆矩阵（运行后生成） |

## Notebook 结构

1. **问题定义** — 任务描述、评估指标选择理由
2. **EDA** — 缺失值分析、生存率分布可视化、关键发现
3. **特征工程** — 缺失值填充策略、新特征构造、类别编码
4. **多模型对比** — 逻辑回归 / 随机森林 / XGBoost，5折交叉验证
5. **模型解释** — 特征重要性分析、混淆矩阵
6. **结论与反思** — 主要发现、局限性、改进方向
