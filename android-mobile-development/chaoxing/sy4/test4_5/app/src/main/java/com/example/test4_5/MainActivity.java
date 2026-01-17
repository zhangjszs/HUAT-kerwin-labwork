package com.example.test4_5;

import android.app.Activity;
import android.media.AudioAttributes;
import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Button;

import java.util.HashMap;

public class MainActivity extends Activity {
    // 声明一个 SoundPool 对象
    private SoundPool soundpool;
    // 创建一个 HashMap 对象
    private HashMap<Integer, Integer> soundmap = new HashMap<>();

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button windbell = findViewById(R.id.button1); // 获取“风铃声”按钮
        Button cuckoo = findViewById(R.id.button2); // 获取“布谷鸟叫声”按钮
        Button doorbell = findViewById(R.id.button3); // 获取“门铃声”按钮
        Button phonebell = findViewById(R.id.button4); // 获取“电话声”按钮

        // 初始化 SoundPool 对象
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
            AudioAttributes audioAttributes = new AudioAttributes.Builder()
                    .setUsage(AudioAttributes.USAGE_MEDIA)
                    .setContentType(AudioAttributes.CONTENT_TYPE_SONIFICATION)
                    .build();

            soundpool = new SoundPool.Builder()
                    .setMaxStreams(5)
                    .setAudioAttributes(audioAttributes)
                    .build();
        } else {
            soundpool = new SoundPool(5, AudioManager.STREAM_MUSIC, 0);
        }

        // 设置加载完成监听器
        soundpool.setOnLoadCompleteListener(new SoundPool.OnLoadCompleteListener() {
            @Override
            public void onLoadComplete(SoundPool soundPool, int sampleId, int status) {
                if (status == 0) {
                    // 音频加载成功
                } else {
                    // 音频加载失败
                }
            }
        });

        // 将要播放的音频流保存到 HashMap 对象中
        soundmap.put(1, soundpool.load(this, R.raw.windbell, 1));
        soundmap.put(2, soundpool.load(this, R.raw.cuckoo, 1));
        soundmap.put(3, soundpool.load(this, R.raw.doorbell, 1));
        soundmap.put(4, soundpool.load(this, R.raw.phonebell, 1));
        soundmap.put(5, soundpool.load(this, R.raw.ring, 1));

        // 为各按钮添加单击事件监听器
        windbell.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                soundpool.play(soundmap.get(1), 1, 1, 0, 0, 1); // 播放指定的音频
            }
        });

        cuckoo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                soundpool.play(soundmap.get(2), 1, 1, 0, 0, 1); // 播放指定的音频
            }
        });

        doorbell.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                soundpool.play(soundmap.get(3), 1, 1, 0, 0, 1); // 播放指定的音频
            }
        });

        phonebell.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                soundpool.play(soundmap.get(4), 1, 1, 0, 0, 1); // 播放指定的音频
            }
        });
    }

    // 重写键被按下的事件
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        soundpool.play(soundmap.get(5), 1, 1, 0, 0, 1); // 播放按键音
        return true;
    }
}