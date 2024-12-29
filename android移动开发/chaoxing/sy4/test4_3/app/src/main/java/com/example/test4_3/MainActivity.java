package com.example.test4_3;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.drawable.AnimationDrawable;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

public class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button btn = findViewById(R.id.btn);
        final ImageView iv = findViewById(R.id.iv);

        btn.setOnClickListener(new View.OnClickListener() { // 为按钮设置监听器
            @SuppressLint("ResourceType")
            @Override
            public void onClick(View v) { // 重写onClick方法
                /* 将动画文件设置为ImageView的背景 */
                iv.setBackgroundResource(R.anim.frame_anim);
                // 获取AnimationDrawable对象
                AnimationDrawable ad = (AnimationDrawable) iv.getBackground();
                ad.start(); // 启动帧动画AnimationDrawable
            }
        });
    }
}