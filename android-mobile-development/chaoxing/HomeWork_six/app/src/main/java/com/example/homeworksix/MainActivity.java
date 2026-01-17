package com.example.homeworksix;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private EditText inputEditText;
    private TextView sumTextView;
    private TextView productTextView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 初始化视图
        inputEditText = findViewById(R.id.inputEditText);
        sumTextView = findViewById(R.id.sumTextView);
        productTextView = findViewById(R.id.productTextView);
        Button calculateButton = findViewById(R.id.calculateButton);

        // 计算按钮点击事件
        calculateButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                calculate();
            }
        });
    }

    // 计算和与乘积
    private void calculate() {
        String input = inputEditText.getText().toString();
        if (input.isEmpty()) {
            sumTextView.setText("请输入n的值");
            productTextView.setText("请输入n的值");
            return;
        }

        int n = Integer.parseInt(input);

        // 启动计算和的线程
        new Thread(new Runnable() {
            @Override
            public void run() {
                int sum = calculateSum(n);
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        sumTextView.setText("和: " + sum);
                    }
                });
            }
        }).start();

        // 启动计算乘积的线程
        new Thread(new Runnable() {
            @Override
            public void run() {
                long product = calculateProduct(n);
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        productTextView.setText("乘积: " + product);
                    }
                });
            }
        }).start();
    }

    // 计算1到n的和
    private int calculateSum(int n) {
        int sum = 0;
        for (int i = 1; i <= n; i++) {
            sum += i;
        }
        return sum;
    }

    // 计算1到n的乘积
    private long calculateProduct(int n) {
        long product = 1;
        for (int i = 1; i <= n; i++) {
            product *= i;
        }
        return product;
    }
}