package com.example.test4_6;

import android.app.Activity;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {
    private MediaPlayer player; // MediaPlayer对象
    private boolean isPaused = false; // 是否暂停
    private TextView hint; // 声明显示提示信息的文本框
    private Button button2; // 暂停/继续按钮

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final Button button1 = findViewById(R.id.button1); // 获取播放按钮
        button2 = findViewById(R.id.button2); // 获取暂停/继续按钮
        final Button button3 = findViewById(R.id.button3); // 获取停止按钮
        hint = findViewById(R.id.hint); // 获取用户显示提示信息的文本框

        // 初始化MediaPlayer对象
        player = MediaPlayer.create(this, R.raw.music);

        // 为MediaPlayer对象添加完成事件监听器
        player.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
                stop(); // 停止播放音乐
            }
        });

        // 为“播放”按钮添加单击事件监听器
        button1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                play(); // 开始播放音乐
                button2.setEnabled(true); // 暂停/继续按钮可用
                button3.setEnabled(true); // 停止按钮可用
                button1.setEnabled(false); // “播放”按钮不可用
            }
        });

        // 为“暂停/继续”按钮添加单击事件监听器
        button2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (player.isPlaying()) {
                    player.pause(); // 暂停播放
                    isPaused = true;
                    ((Button) v).setText("继续");
                    hint.setText("暂停播放音频...");
                } else {
                    player.start(); // 继续播放
                    isPaused = false;
                    ((Button) v).setText("暂停");
                    hint.setText("继续播放音频...");
                }
            }
        });

        // 为“停止”按钮添加单击事件监听器
        button3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                stop(); // 停止播放
                hint.setText("停止播放音频...");
                button2.setEnabled(false); // 暂停/继续按钮不可用
                button3.setEnabled(false); // 停止按钮不可用
                button1.setEnabled(true); // “播放”按钮可用
            }
        });
    }

    // 播放音乐的方法
    private void play() {
        try {
            if (!player.isPlaying()) {
                player.prepareAsync();
                player.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
                    @Override
                    public void onPrepared(MediaPlayer mp) {
                        player.start(); // 开始播放
                        hint.setText("正在播放音频...");
                    }
                });
            }
        } catch (Exception e) {
            e.printStackTrace(); // 输出异常信息
        }
    }

    // 停止音乐的方法
    private void stop() {
        try {
            if (player != null) {
                player.stop();
                player.reset();
                player.release();
                player = MediaPlayer.create(this, R.raw.music);
                isPaused = false;
                button2.setText("暂停");
            }
        } catch (Exception e) {
            e.printStackTrace(); // 输出异常信息
        }
    }

    @Override
    protected void onDestroy() {
        if (player != null) {
            if (player.isPlaying()) {
                player.stop(); // 停止音频的播放
            }
            player.release(); // 释放资源
            player = null;
        }
        super.onDestroy();
    }
}



