# 为当前 clone 启用 notebook 自动清理（git clean filter，等价 nbstripout）
# 运行：powershell -ExecutionPolicy Bypass -File scripts/tools/setup_notebook_filter.ps1
$ErrorActionPreference = "Stop"

$root = git rev-parse --show-toplevel
if (-not $root) { throw "当前目录不在 git 仓库中" }

Push-Location $root
try {
    git config --local filter.nbclean.clean "node scripts/tools/clean_notebooks.mjs --filter"
    git config --local filter.nbclean.smudge "cat"
    Write-Host "已启用 filter.nbclean：git add 时会自动清空 .ipynb 的运行输出"
    Write-Host "撤销：git config --local --remove-section filter.nbclean"
} finally {
    Pop-Location
}
