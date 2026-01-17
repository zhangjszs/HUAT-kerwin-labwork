package com.example.test5;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class SecondActivity extends Activity {
    public static final int RESULT_CODE = 0x222;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_second);

        // 接收数据
        Intent intent = getIntent();
        int num1 = intent.getIntExtra("key1", 0);
        int num2 = intent.getIntExtra("key2", 0);

        // 计算结果
        final int sum = num1 + num2;

        Button backButton = findViewById(R.id.back);

        backButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 将计算结果返回
                Intent resultIntent = new Intent();
                resultIntent.putExtra("result", sum);

                // 设置返回结果
                setResult(RESULT_CODE, resultIntent);

                // 关闭当前Activity
                finish();
            }
        });
    }
}
