/**
 * 图片懒加载脚本
 * 优化页面加载性能,仅在图片进入视口时才加载
 */

(function () {
    'use strict';

    // 懒加载配置
    const lazyLoadConfig = {
        rootMargin: '50px 0px', // 提前50px开始加载
        threshold: 0.01
    };

    // 创建Intersection Observer
    const imageObserver = new IntersectionObserver((entries, observer) => {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                const img = entry.target;
                loadImage(img);
                observer.unobserve(img);
            }
        });
    }, lazyLoadConfig);

    // 加载图片函数
    function loadImage(img) {
        const src = img.dataset.src;
        if (!src) return;

        // 创建一个临时图片对象来预加载
        const tempImg = new Image();

        tempImg.onload = () => {
            img.src = src;
            img.classList.add('loaded');
            img.removeAttribute('data-src');
        };

        tempImg.onerror = () => {
            // 加载失败,显示占位符
            img.classList.add('error');
            console.warn('图片加载失败:', src);
        };

        tempImg.src = src;
    }

    // 初始化懒加载
    function initLazyLoad() {
        // 查找所有需要懒加载的图片
        const lazyImages = document.querySelectorAll('img[data-src]');

        if ('IntersectionObserver' in window) {
            // 支持Intersection Observer
            lazyImages.forEach(img => {
                // 添加加载中样式
                img.classList.add('lazy-loading');
                imageObserver.observe(img);
            });
        } else {
            // 降级处理:直接加载所有图片
            lazyImages.forEach(img => {
                loadImage(img);
            });
        }
    }

    // DOM加载完成后初始化
    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', initLazyLoad);
    } else {
        initLazyLoad();
    }

    // 添加懒加载样式
    const style = document.createElement('style');
    style.textContent = `
        img.lazy-loading {
            background: linear-gradient(90deg, #f0f0f0 25%, #e0e0e0 50%, #f0f0f0 75%);
            background-size: 200% 100%;
            animation: loading 1.5s ease-in-out infinite;
            min-height: 100px;
        }
        
        img.loaded {
            animation: fadeIn 0.3s ease-in;
        }
        
        img.error {
            background: #f8d7da;
            border: 1px dashed #f5c6cb;
            position: relative;
        }
        
        img.error::after {
            content: '图片加载失败';
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            color: #721c24;
            font-size: 14px;
        }
        
        @keyframes loading {
            0% { background-position: 200% 0; }
            100% { background-position: -200% 0; }
        }
        
        @keyframes fadeIn {
            from { opacity: 0; }
            to { opacity: 1; }
        }
    `;
    document.head.appendChild(style);
})();
