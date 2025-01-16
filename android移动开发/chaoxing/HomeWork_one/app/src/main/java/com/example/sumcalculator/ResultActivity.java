package com.example.sumcalculator;

import android.content.Intent;
import android.os.Bundle;
import android.view.View; // 确保导入 View 类
import android.widget.Button;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;

public class ResultActivity extends AppCompatActivity {

    private TextView resultTextView;
    private Button backButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_result);

        resultTextView = findViewById(R.id.resultTextView);
        backButton = findViewById(R.id.backButton);

        Intent intent = getIntent();
        int sum = intent.getIntExtra("SUM", 0);
        resultTextView.setText("结果: " + sum);

        backButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
    }
}
