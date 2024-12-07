package com.example.test5;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity {
    private static final int REQUEST_CODE = 0x111;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button computeButton = findViewById(R.id.compute);

        computeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 获取输入数据
                EditText data1Field = findViewById(R.id.data1);
                EditText data2Field = findViewById(R.id.data2);

                String data1Str = data1Field.getText().toString();
                String data2Str = data2Field.getText().toString();

                if (data1Str.isEmpty() || data2Str.isEmpty()) {
                    return;
                }

                int data1 = Integer.parseInt(data1Str);
                int data2 = Integer.parseInt(data2Str);

                // 创建Intent，携带数据跳转
                Intent intent = new Intent(MainActivity.this, SecondActivity.class);
                intent.putExtra("key1", data1);
                intent.putExtra("key2", data2);

                // 启动SecondActivity并等待结果
                startActivityForResult(intent, REQUEST_CODE);
            }
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == REQUEST_CODE && resultCode == SecondActivity.RESULT_CODE) {
            // 接收返回数据
            int result = data.getIntExtra("result", 0);

            // 显示结果
            TextView resultView = findViewById(R.id.result);
            resultView.setText("结果为：" + result);
        }
    }
}
