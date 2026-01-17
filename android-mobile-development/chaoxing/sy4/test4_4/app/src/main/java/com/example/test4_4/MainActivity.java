package com.example.test4_4;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.ImageView;

public class MainActivity extends Activity {
    @Override
    public void onCreate(Bundle savedInstanceState) { // 重写onCreate方法
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main); // 设置屏幕
        Button btn1 = (Button) findViewById(R.id.btn1); // 获取Button对象
        btn1.setOnClickListener(new View.OnClickListener() { // 为Button对象添加OnClickListener监听器
            @Override
            public void onClick(View v) { // 重写onClick方法
                ImageView iv = (ImageView) findViewById(R.id.iv);
                // 设置ImageView的图片来源，等价于android:src="@drawable/bigandroid"
                iv.setImageDrawable(getResources().getDrawable(R.drawable.bigandroid));
                // 加载anim文件夹下的动画文件
                Animation animation = AnimationUtils.loadAnimation(MainActivity.this, R.anim.frame_anim);
                iv.startAnimation(animation); // 启动动画
            }
        });
    }
}