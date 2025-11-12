/**
 * Qt演示文稿 - 公共JavaScript文件
 * 统一的交互功能
 */

// 全局状态
const AppState = {
    timerRunning: false,
    timerStart: 0,
    timerElapsed: 0,
    timerInterval: null
};

// DOM加载完成后初始化
document.addEventListener('DOMContentLoaded', function () {
    initControls();
    initKeyboard();
    initTouch();
    initProgress();
});

// 初始化控制按钮
function initControls() {
    const controls = document.createElement('div');
    controls.className = 'presentation-controls';
    controls.innerHTML = `
        <button class="control-btn" onclick="toggleFullscreen()" title="全屏 (F)">
            <i class="fas fa-expand"></i>
        </button>
        <button class="control-btn" onclick="toggleTimer()" title="计时器 (T)">
            <i class="fas fa-clock"></i>
        </button>
        <button class="control-btn" onclick="toggleNotes()" title="备注 (N)">
            <i class="fas fa-sticky-note"></i>
        </button>
    `;
    document.body.appendChild(controls);

    // 计时器显示
    const timer = document.createElement('div');
    timer.className = 'timer-display';
    timer.id = 'timerDisplay';
    timer.innerHTML = `
        <i class="fas fa-stopwatch"></i>
        <span id="timerText">00:00:00</span>
        <div class="timer-controls">
            <button onclick="startTimer()">开始</button>
            <button onclick="pauseTimer()">暂停</button>
            <button onclick="resetTimer()">重置</button>
        </div>
    `;
    document.body.appendChild(timer);

    // 演讲者备注
    const notes = document.createElement('div');
    notes.className = 'speaker-notes';
    notes.id = 'speakerNotes';
    notes.innerHTML = `
        <button class="close-btn" onclick="hideNotes()">&times;</button>
        <h3>演讲者备注</h3>
        <div id="notesContent"></div>
    `;
    document.body.appendChild(notes);

    loadNotesForPage();
}

// 键盘快捷键
function initKeyboard() {
    document.addEventListener('keydown', function (e) {
        if (e.target.tagName === 'INPUT' || e.target.tagName === 'TEXTAREA') return;

        switch (e.key.toLowerCase()) {
            case 'arrowleft':
            case 'pageup':
                navigateTo('prev');
                e.preventDefault();
                break;
            case 'arrowright':
            case 'pagedown':
            case ' ':
                navigateTo('next');
                e.preventDefault();
                break;
            case 'home':
                navigateTo(1);
                e.preventDefault();
                break;
            case 'end':
                navigateTo(9);
                e.preventDefault();
                break;
            case 'f':
                toggleFullscreen();
                e.preventDefault();
                break;
            case 't':
                toggleTimer();
                e.preventDefault();
                break;
            case 'n':
                toggleNotes();
                e.preventDefault();
                break;
            case 'escape':
                if (document.fullscreenElement) exitFullscreen();
                hideNotes();
                e.preventDefault();
                break;
        }
    });
}

// 触摸手势
function initTouch() {
    let touchStart = 0;
    document.addEventListener('touchstart', e => {
        touchStart = e.changedTouches[0].screenX;
    });
    document.addEventListener('touchend', e => {
        const touchEnd = e.changedTouches[0].screenX;
        const diff = touchStart - touchEnd;
        if (Math.abs(diff) > 50) {
            navigateTo(diff > 0 ? 'next' : 'prev');
        }
    });
}

// 进度指示器
function initProgress() {
    const page = getCurrentPage();
    const total = 9;
    const indicator = document.createElement('div');
    indicator.className = 'progress-indicator';
    indicator.innerHTML = `
        <div class="progress-bar">
            <div class="progress-fill" style="width: ${(page / total) * 100}%"></div>
        </div>
        <div class="progress-text">${page} / ${total}</div>
    `;
    document.body.insertBefore(indicator, document.body.firstChild);
}

// 获取当前页码
function getCurrentPage() {
    const match = window.location.pathname.match(/(\d+)\.html/);
    return match ? parseInt(match[1]) : 1;
}

// 导航
function navigateTo(target) {
    const current = getCurrentPage();
    let next = current;

    if (target === 'next') next = Math.min(current + 1, 9);
    else if (target === 'prev') next = Math.max(current - 1, 1);
    else next = target;

    if (next !== current) {
        window.location.href = `${next}.html`;
    }
}

// 全屏功能
function toggleFullscreen() {
    if (!document.fullscreenElement) {
        document.documentElement.requestFullscreen().then(() => {
            document.body.classList.add('fullscreen-mode');
            updateFullscreenIcon();
        }).catch(console.error);
    } else {
        exitFullscreen();
    }
}

function exitFullscreen() {
    if (document.fullscreenElement) {
        document.exitFullscreen().then(() => {
            document.body.classList.remove('fullscreen-mode');
            updateFullscreenIcon();
        });
    }
}

function updateFullscreenIcon() {
    const btn = document.querySelector('.presentation-controls .control-btn:first-child i');
    if (btn) {
        btn.className = document.fullscreenElement ? 'fas fa-compress' : 'fas fa-expand';
    }
}

// 计时器功能
function toggleTimer() {
    const timer = document.getElementById('timerDisplay');
    timer.classList.toggle('active');
    if (timer.classList.contains('active') && !AppState.timerRunning) {
        startTimer();
    }
}

function startTimer() {
    if (!AppState.timerRunning) {
        AppState.timerStart = Date.now() - AppState.timerElapsed;
        AppState.timerRunning = true;
        AppState.timerInterval = setInterval(updateTimer, 100);
    }
}

function pauseTimer() {
    if (AppState.timerRunning) {
        AppState.timerRunning = false;
        clearInterval(AppState.timerInterval);
        AppState.timerElapsed = Date.now() - AppState.timerStart;
    }
}

function resetTimer() {
    pauseTimer();
    AppState.timerElapsed = 0;
    updateTimerDisplay(0);
}

function updateTimer() {
    if (AppState.timerRunning) {
        updateTimerDisplay(Date.now() - AppState.timerStart);
    }
}

function updateTimerDisplay(ms) {
    const s = Math.floor(ms / 1000);
    const h = Math.floor(s / 3600);
    const m = Math.floor((s % 3600) / 60);
    const sec = s % 60;
    const text = `${String(h).padStart(2, '0')}:${String(m).padStart(2, '0')}:${String(sec).padStart(2, '0')}`;
    const el = document.getElementById('timerText');
    if (el) el.textContent = text;
}

// 备注功能
function toggleNotes() {
    const notes = document.getElementById('speakerNotes');
    if (notes.classList.contains('active')) {
        hideNotes();
    } else {
        showNotes();
    }
}

function showNotes() {
    const notes = document.getElementById('speakerNotes');
    notes.classList.add('active');
    loadNotesForPage();
}

function hideNotes() {
    const notes = document.getElementById('speakerNotes');
    notes.classList.remove('active');
}

function loadNotesForPage() {
    const page = getCurrentPage();
    const content = document.getElementById('notesContent');
    if (!content) return;

    const notes = {
        1: '<p><strong>欢迎词</strong></p><p>介绍团队成员和演示主题，控制在2分钟以内。重点突出Qt的跨平台特性。</p>',
        2: '<p><strong>大纲概览</strong></p><p>快速浏览各个要点，每个停留5-10秒，让听众了解演示结构。</p>',
        3: '<p><strong>Qt定义</strong></p><p>详细介绍Qt的定义、历史和核心特点。重点强调跨平台特性和6大核心特点。</p>',
        4: '<p><strong>Qt优势</strong></p><p>突出Qt在嵌入式系统中的优势，可以举车载系统等实际案例。</p>',
        5: '<p><strong>下载指南</strong></p><p>演示下载过程，提醒学生记录官网地址。强调开源版和商业版的区别。</p>',
        6: '<p><strong>安装步骤</strong></p><p>重点讲解组件选择步骤，这是最容易出错的地方。建议准备截图演示。</p>',
        7: '<p><strong>环境配置</strong></p><p>演示环境变量配置，建议准备好命令行窗口实时演示。</p>',
        8: '<p><strong>IDE介绍</strong></p><p>介绍Qt Creator各个模式的用途，可以打开IDE实际演示。</p>',
        9: '<p><strong>控件展示</strong></p><p>展示常用Widgets，可以简单演示几个控件的实际使用效果。</p>'
    };

    content.innerHTML = notes[page] || '<p>本页暂无备注。</p><p><strong>快捷键提示：</strong></p><ul><li>F - 全屏</li><li>T - 计时器</li><li>N - 备注</li><li>←→ - 翻页</li></ul>';
}
