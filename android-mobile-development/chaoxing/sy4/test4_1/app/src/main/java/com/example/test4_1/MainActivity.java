package com.example.test4_1;

import android.app.Activity;
import android.os.Bundle;
import android.widget.FrameLayout;

public class MainActivity extends Activity {
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 获取布局文件中添加的帧布局管理器
        FrameLayout fl = (FrameLayout) findViewById(R.id.frameLayout1);
        // 将自定义的MyView视图添加到帧布局管理器中
        fl.addView(new MyView(this));
    }
}