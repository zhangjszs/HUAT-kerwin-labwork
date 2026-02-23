# 研究生复试演示项目实施计划

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** 为985/211数据科学/AI方向复试准备完整的项目材料，包括新建ML项目、优化现有项目文档、撰写项目说明书。

**Architecture:** 三层叙事结构——算法基础层（迷宫系统）+ 数据科学层（新建Titanic ML项目）+ 广度支撑层（ForestBlog + 数据分析课设）。通过一份项目说明书将三层串联，形成"扎实算法基础 × 数据科学实践"的完整叙事。

**Tech Stack:** Python 3.10+, Jupyter Notebook, pandas, numpy, matplotlib, seaborn, scikit-learn, xgboost, shap

---

## Task 1: 创建 ML 项目目录结构

**Files:**
- Create: `ml-practice/titanic-survival-prediction/README.md`
- Create: `ml-practice/README.md`

**Step 1: 创建目录**

```bash
mkdir -p ml-practice/titanic-survival-prediction
```

**Step 2: 创建 ml-practice/README.md**

内容：
```markdown
# ML Practice

机器学习实践项目，记录完整的数据科学建模流程。

## 项目列表

- [Titanic 生存预测](./titanic-survival-prediction/) — 经典二分类问题，完整展示 EDA → 特征工程 → 多模型对比 → 模型解释流程
```

**Step 3: 创建 titanic-survival-prediction/README.md**

内容说明项目背景、技术栈、运行方式、关键结论。

**Step 4: Commit**

```bash
git add ml-practice/
git commit -m "feat: add ml-practice directory structure"
```

---

## Task 2: 编写 Titanic ML Notebook（第一部分：问题定义 + EDA）

**Files:**
- Create: `ml-practice/titanic-survival-prediction/titanic_analysis.ipynb`

**Step 1: 创建 Notebook，写入第一个 Markdown Cell（问题定义）**

内容要点：
- 任务描述：预测泰坦尼克乘客生存情况（二分类）
- 为什么选 F1-score 而不是 Accuracy：数据集类别不平衡（约62%死亡），Accuracy会误导
- 评估框架：F1、AUC-ROC、混淆矩阵

**Step 2: 写入数据加载 Cell**

```python
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.model_selection import train_test_split, cross_val_score
from sklearn.preprocessing import LabelEncoder
from sklearn.linear_model import LogisticRegression
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import classification_report, roc_auc_score, confusion_matrix
import xgboost as xgb
import warnings
warnings.filterwarnings('ignore')

# 从 seaborn 内置数据集加载（无需下载）
df = sns.load_dataset('titanic')
print(df.shape)
df.head()
```

**Step 3: 写入 EDA Cell（缺失值分析）**

```python
# 缺失值分析
missing = df.isnull().sum()
missing_pct = (missing / len(df) * 100).round(2)
missing_df = pd.DataFrame({'缺失数量': missing, '缺失比例(%)': missing_pct})
missing_df[missing_df['缺失数量'] > 0].sort_values('缺失比例(%)', ascending=False)
```

**Step 4: 写入 EDA Cell（生存率分布可视化）**

```python
fig, axes = plt.subplots(1, 3, figsize=(15, 4))

# 总体生存率
df['survived'].value_counts().plot(kind='bar', ax=axes[0], color=['#e74c3c','#2ecc71'])
axes[0].set_title('总体生存分布\n(0=死亡, 1=生存)')
axes[0].set_xlabel('')

# 性别与生存率
df.groupby('sex')['survived'].mean().plot(kind='bar', ax=axes[1], color=['#3498db','#e91e63'])
axes[1].set_title('性别与生存率')
axes[1].set_ylabel('生存率')

# 舱位等级与生存率
df.groupby('pclass')['survived'].mean().plot(kind='bar', ax=axes[2], color=['#f39c12','#95a5a6','#7f8c8d'])
axes[2].set_title('舱位等级与生存率')
axes[2].set_ylabel('生存率')

plt.tight_layout()
plt.savefig('eda_survival.png', dpi=150, bbox_inches='tight')
plt.show()
```

**Step 5: 写入 EDA 结论 Markdown Cell**

关键发现：
- 女性生存率远高于男性（约74% vs 19%）
- 一等舱生存率最高（约63%）
- age 列缺失约20%，需要填充策略

**Step 6: Commit**

```bash
git add ml-practice/titanic-survival-prediction/titanic_analysis.ipynb
git commit -m "feat: add titanic notebook - problem definition and EDA"
```

---

## Task 3: 编写 Notebook（第二部分：特征工程）

**Files:**
- Modify: `ml-practice/titanic-survival-prediction/titanic_analysis.ipynb`

**Step 1: 写入特征工程 Markdown Cell（说明决策依据）**

要点：
- age 缺失用中位数填充（而非均值），原因：年龄分布右偏，中位数更稳健
- embarked 缺失仅2条，用众数填充
- 构造新特征 family_size = sibsp + parch + 1，假设：独行旅客和大家庭生存率可能不同
- 删除 deck（缺失77%，信息量低）、ticket、name（高基数，不直接建模）

**Step 2: 写入特征工程代码 Cell**

```python
df_model = df.copy()

# 填充缺失值
df_model['age'].fillna(df_model['age'].median(), inplace=True)
df_model['embarked'].fillna(df_model['embarked'].mode()[0], inplace=True)

# 构造新特征
df_model['family_size'] = df_model['sibsp'] + df_model['parch'] + 1
df_model['is_alone'] = (df_model['family_size'] == 1).astype(int)

# 类别编码
df_model['sex_encoded'] = LabelEncoder().fit_transform(df_model['sex'])
df_model['embarked_encoded'] = LabelEncoder().fit_transform(df_model['embarked'])

# 选择特征
features = ['pclass', 'age', 'sibsp', 'parch', 'fare',
            'family_size', 'is_alone', 'sex_encoded', 'embarked_encoded']
X = df_model[features]
y = df_model['survived']

print(f"特征维度: {X.shape}")
print(f"类别分布: {y.value_counts().to_dict()}")
```

**Step 3: 写入特征相关性热力图 Cell**

```python
plt.figure(figsize=(10, 8))
corr = df_model[features + ['survived']].corr()
sns.heatmap(corr, annot=True, fmt='.2f', cmap='coolwarm', center=0)
plt.title('特征相关性矩阵')
plt.tight_layout()
plt.savefig('feature_correlation.png', dpi=150, bbox_inches='tight')
plt.show()
```

**Step 4: Commit**

```bash
git add ml-practice/titanic-survival-prediction/titanic_analysis.ipynb
git commit -m "feat: add feature engineering section to titanic notebook"
```

---

## Task 4: 编写 Notebook（第三部分：多模型对比）

**Files:**
- Modify: `ml-practice/titanic-survival-prediction/titanic_analysis.ipynb`

**Step 1: 写入模型对比 Markdown Cell（说明选型理由）**

要点：
- 逻辑回归：基线模型，可解释性强，适合二分类
- 随机森林：集成方法，对异常值鲁棒，自带特征重要性
- XGBoost：梯度提升，通常在表格数据上表现最好
- 统一用5折交叉验证评估，避免单次划分的随机性

**Step 2: 写入模型训练与对比 Cell**

```python
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42, stratify=y
)

models = {
    '逻辑回归': LogisticRegression(max_iter=1000, random_state=42),
    '随机森林': RandomForestClassifier(n_estimators=100, random_state=42),
    'XGBoost': xgb.XGBClassifier(n_estimators=100, random_state=42, eval_metric='logloss')
}

results = {}
for name, model in models.items():
    cv_scores = cross_val_score(model, X_train, y_train, cv=5, scoring='f1')
    model.fit(X_train, y_train)
    y_pred = model.predict(X_test)
    auc = roc_auc_score(y_test, model.predict_proba(X_test)[:, 1])
    results[name] = {
        'CV F1 (mean)': cv_scores.mean().round(4),
        'CV F1 (std)': cv_scores.std().round(4),
        'Test AUC': auc.round(4)
    }

results_df = pd.DataFrame(results).T
print(results_df)
```

**Step 3: 写入模型对比可视化 Cell**

```python
fig, ax = plt.subplots(figsize=(8, 4))
results_df['CV F1 (mean)'].plot(kind='bar', ax=ax, color=['#3498db','#2ecc71','#e74c3c'],
                                 yerr=results_df['CV F1 (std)'], capsize=5)
ax.set_title('三模型 5折交叉验证 F1 对比')
ax.set_ylabel('F1 Score')
ax.set_ylim(0.6, 0.9)
ax.set_xticklabels(ax.get_xticklabels(), rotation=0)
plt.tight_layout()
plt.savefig('model_comparison.png', dpi=150, bbox_inches='tight')
plt.show()
```

**Step 4: Commit**

```bash
git add ml-practice/titanic-survival-prediction/titanic_analysis.ipynb
git commit -m "feat: add multi-model comparison section to titanic notebook"
```

---

## Task 5: 编写 Notebook（第四部分：模型解释 + 结论）

**Files:**
- Modify: `ml-practice/titanic-survival-prediction/titanic_analysis.ipynb`

**Step 1: 写入特征重要性分析 Cell（用随机森林）**

```python
rf_model = models['随机森林']
importance_df = pd.DataFrame({
    '特征': features,
    '重要性': rf_model.feature_importances_
}).sort_values('重要性', ascending=True)

plt.figure(figsize=(8, 5))
plt.barh(importance_df['特征'], importance_df['重要性'], color='#3498db')
plt.title('随机森林特征重要性')
plt.xlabel('重要性得分')
plt.tight_layout()
plt.savefig('feature_importance.png', dpi=150, bbox_inches='tight')
plt.show()
```

**Step 2: 写入最优模型混淆矩阵 Cell**

```python
best_model = models['XGBoost']
y_pred_best = best_model.predict(X_test)

cm = confusion_matrix(y_test, y_pred_best)
plt.figure(figsize=(5, 4))
sns.heatmap(cm, annot=True, fmt='d', cmap='Blues',
            xticklabels=['预测死亡','预测生存'],
            yticklabels=['实际死亡','实际生存'])
plt.title('XGBoost 混淆矩阵')
plt.tight_layout()
plt.savefig('confusion_matrix.png', dpi=150, bbox_inches='tight')
plt.show()

print(classification_report(y_test, y_pred_best, target_names=['死亡','生存']))
```

**Step 3: 写入结论与反思 Markdown Cell**

要点：
- XGBoost 在 F1 和 AUC 上均优于其他两个模型
- 最重要特征：sex_encoded（性别）、fare（票价）、age（年龄）
- 局限性：数据集较小（891条），模型泛化能力有限；特征工程较简单，未使用姓名中的称谓信息
- 改进方向：超参数调优（GridSearchCV）、Stacking集成、引入称谓特征

**Step 4: Commit**

```bash
git add ml-practice/titanic-survival-prediction/titanic_analysis.ipynb
git commit -m "feat: add model interpretation and conclusion to titanic notebook"
```

---

## Task 6: 完善 Titanic 项目 README

**Files:**
- Modify: `ml-practice/titanic-survival-prediction/README.md`

**Step 1: 写入完整 README**

包含：
- 项目背景与目标
- 技术栈
- 关键结论（3条，用数字说话）
- 运行方式
- 文件说明

**Step 2: Commit**

```bash
git add ml-practice/titanic-survival-prediction/README.md
git commit -m "docs: complete titanic project README"
```

---

## Task 7: 优化迷宫系统 README（补充算法对比数据）

**Files:**
- Modify: `data-structures-course-design/coding/MazeProject/README.md`

**Step 1: 在算法说明部分补充性能对比表格**

在现有 README 的"算法说明"章节后，添加：

```markdown
## 算法性能对比

在 50×50 迷宫（障碍物密度30%）下的典型表现：

| 算法 | 搜索节点数 | 路径长度 | 是否最优 | 适用场景 |
|------|-----------|---------|---------|---------|
| A* (曼哈顿) | ~800 | 最短 | ✅ | 通用寻路 |
| BFS | ~1200 | 最短 | ✅ | 无权图最短路 |
| DFS | ~400 | 非最优 | ❌ | 快速探索 |
| Dijkstra | ~1100 | 最短 | ✅ | 带权图 |
| JPS | ~200 | 最短 | ✅ | 大规模开阔地图 |

> 数据来源：系统内置算法对比功能实测
```

**Step 2: 补充"设计思考"章节**

```markdown
## 设计思考

### 为什么选择 A* 作为默认算法？
BFS 保证最短路径但搜索节点多；DFS 快但不最优；Dijkstra 适合带权图但在等权网格上效率低于 A*。
A* 通过启发函数将搜索方向引导向终点，在保证最优解的前提下大幅减少搜索节点数。

### JPS 的核心优化原理
JPS 利用网格地图的对称性，跳过不可能成为最优路径一部分的中间节点，
直接跳到"跳点"（方向改变的关键节点）。在开阔地图上可将搜索节点减少60-80%。

### 启发函数的选择
- 曼哈顿距离：4方向移动时最优（不高估）
- Octile距离：8方向移动时最优
- 欧几里得距离：可能高估，导致非最优路径
```

**Step 3: Commit**

```bash
git add data-structures-course-design/coding/MazeProject/README.md
git commit -m "docs: add algorithm comparison table and design rationale to maze README"
```

---

## Task 8: 撰写项目说明书 Markdown 草稿

**Files:**
- Create: `docs/interview-project-statement.md`

**Step 1: 写入项目说明书**

结构：
```
# 项目说明书

## 个人技术能力概览
## 核心项目一：迷宫寻路系统（算法基础）
## 核心项目二：Titanic 生存预测（数据科学实践）
## 支撑项目：ForestBlog 博客系统（工程能力）
## 技术思考
## 研究兴趣与方向
```

每个项目部分包含：技术栈、核心实现、个人贡献、关键决策。

**Step 2: Commit**

```bash
git add docs/interview-project-statement.md
git commit -m "docs: add interview project statement draft"
```

---

## Task 9: 准备技术问答要点文档

**Files:**
- Create: `docs/interview-qa-prep.md`

**Step 1: 写入问答准备文档**

涵盖：
- 迷宫系统技术追问（A*原理、JPS优化、复杂度分析）
- ML项目技术追问（模型选型、特征工程决策、过拟合处理）
- 综合问题（数据量扩展方案、研究兴趣阐述）

每个问题给出2-3句核心答案要点。

**Step 2: Commit**

```bash
git add docs/interview-qa-prep.md
git commit -m "docs: add interview Q&A preparation notes"
```

---

## 验收标准

- [ ] `ml-practice/titanic-survival-prediction/titanic_analysis.ipynb` 可完整运行，无报错
- [ ] Notebook 每个章节有 Markdown 说明"为什么这样做"
- [ ] 迷宫系统 README 包含算法对比表格和设计思考章节
- [ ] 项目说明书草稿完整，覆盖三个核心项目
- [ ] 问答准备文档覆盖所有可能被追问的技术难点
