plt.figure(figsize=(10, 8))
corr = df_model[features + ['survived']].corr()
sns.heatmap(corr, annot=True, fmt='.2f', cmap='coolwarm', center=0)
plt.title('特征相关性矩阵')
plt.tight_layout()
plt.savefig('feature_correlation.png', dpi=150, bbox_inches='tight')
plt.show()

# === Next Section ===

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
    cv_scores = cross_val_score(model, X_train, y_train, cv=5, scoring="f1")
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

# === Next Section ===

fig, ax = plt.subplots(figsize=(8, 4))
results_df['CV F1 (mean)'].plot(
    kind='bar', ax=ax,
    color=['#3498db', '#2ecc71', '#e74c3c'],
    yerr=results_df['CV F1 (std)'], capsize=5
)
ax.set_title('三模型 5折交叉验证 F1 对比')
ax.set_ylabel('F1 Score')
ax.set_ylim(0.5, 0.9)
ax.set_xticklabels(ax.get_xticklabels(), rotation=0)
plt.tight_layout()
plt.savefig('model_comparison.png', dpi=150, bbox_inches='tight')
plt.show()