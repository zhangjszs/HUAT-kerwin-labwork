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

plt.rcParams['font.sans-serif'] = ['SimHei', 'DejaVu Sans']
plt.rcParams['axes.unicode_minus'] = False

# 从 seaborn 内置数据集加载（无需下载）
df = sns.load_dataset('titanic')
print(f"数据集形状: {df.shape}")
df.head()

# === Next Section ===

# 缺失值分析
missing = df.isnull().sum()
missing_pct = (missing / len(df) * 100).round(2)
missing_df = pd.DataFrame({'缺失数量': missing, '缺失比例(%)': missing_pct})
missing_df[missing_df['缺失数量'] > 0].sort_values('缺失比例(%)', ascending=False)

# === Next Section ===

fig, axes = plt.subplots(1, 3, figsize=(15, 4))

# 总体生存率
df['survived'].value_counts().plot(kind='bar', ax=axes[0], color=['#e74c3c','#2ecc71'])
axes[0].set_title('总体生存分布
(0=死亡, 1=生存)')
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

# === Next Section ===

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