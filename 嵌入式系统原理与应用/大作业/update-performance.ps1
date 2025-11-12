# 性能优化脚本 - 批量更新HTML文件
# 将CDN资源替换为本地资源,添加图片懒加载

Write-Host "🚀 开始性能优化..." -ForegroundColor Green

$files = 3..9 | ForEach-Object { "$_.html" }

foreach ($file in $files) {
    if (Test-Path $file) {
        Write-Host "📝 处理 $file..." -ForegroundColor Cyan
        
        $content = Get-Content $file -Raw -Encoding UTF8
        
        # 替换Tailwind CSS CDN
        $content = $content -replace 'https://cdn\.jsdelivr\.net/npm/tailwindcss@2\.2\.19/dist/tailwind\.min\.css', 'assets/libs/tailwind.min.css'
        
        # 替换Font Awesome CDN
        $content = $content -replace 'https://cdn\.jsdelivr\.net/npm/@fortawesome/fontawesome-free@6\.4\.0/css/all\.min\.css', 'assets/libs/fontawesome.min.css'
        
        # 替换Mermaid CDN (仅第6页)
        $content = $content -replace 'https://cdn\.jsdelivr\.net/npm/mermaid@11\.6\.0/dist/mermaid\.min\.js', 'assets/libs/mermaid.min.js'
        
        # 图片懒加载:将 src= 替换为 data-src=,添加占位符SVG
        $content = $content -replace '<img\s+src="(https://[^"]+)"', '<img data-src="$1" src="data:image/svg+xml,%3Csvg xmlns=''http://www.w3.org/2000/svg'' viewBox=''0 0 400 300''%3E%3C/svg%3E"'
        
        # 添加 loading="lazy" 属性
        $content = $content -replace '(<img[^>]*)(>)', '$1 loading="lazy"$2'
        
        # 如果还没有懒加载脚本,添加它
        if ($content -notmatch 'lazyload\.js') {
            $content = $content -replace '(<script src="assets/js/common\.js"></script>)', '<script src="assets/js/lazyload.js"></script>`n    $1'
        }
        
        # 保存文件
        $content | Set-Content $file -Encoding UTF8 -NoNewline
        Write-Host "✅ $file 更新完成" -ForegroundColor Green
    }
    else {
        Write-Host "⚠️  $file 不存在" -ForegroundColor Yellow
    }
}

Write-Host "`n🎉 所有文件更新完成!" -ForegroundColor Green
Write-Host "📊 建议使用浏览器开发者工具测试加载速度" -ForegroundColor Cyan
