package com.example.homeworkseven;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private EditText rectInput, circleInput, lineInput;
    private DrawingView drawingView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 初始化视图
        rectInput = findViewById(R.id.rectInput);
        circleInput = findViewById(R.id.circleInput);
        lineInput = findViewById(R.id.lineInput);
        drawingView = findViewById(R.id.drawingView);
        Button drawButton = findViewById(R.id.drawButton);

        // 绘制按钮点击事件
        drawButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                drawShapes();
            }
        });
    }

    // 解析输入并绘制图形
    private void drawShapes() {
        // 解析矩形坐标
        String rectText = rectInput.getText().toString();
        if (!rectText.isEmpty()) {
            String[] rectCoords = rectText.split(",");
            if (rectCoords.length == 4) {
                float left = Float.parseFloat(rectCoords[0].trim());
                float top = Float.parseFloat(rectCoords[1].trim());
                float right = Float.parseFloat(rectCoords[2].trim());
                float bottom = Float.parseFloat(rectCoords[3].trim());
                drawingView.setRect(left, top, right, bottom);
            }
        }

        // 解析圆坐标和半径
        String circleText = circleInput.getText().toString();
        if (!circleText.isEmpty()) {
            String[] circleCoords = circleText.split(",");
            if (circleCoords.length == 3) {
                float cx = Float.parseFloat(circleCoords[0].trim());
                float cy = Float.parseFloat(circleCoords[1].trim());
                float radius = Float.parseFloat(circleCoords[2].trim());
                drawingView.setCircle(cx, cy, radius);
            }
        }

        // 解析直线坐标
        String lineText = lineInput.getText().toString();
        if (!lineText.isEmpty()) {
            String[] lineCoords = lineText.split(",");
            if (lineCoords.length == 4) {
                float startX = Float.parseFloat(lineCoords[0].trim());
                float startY = Float.parseFloat(lineCoords[1].trim());
                float endX = Float.parseFloat(lineCoords[2].trim());
                float endY = Float.parseFloat(lineCoords[3].trim());
                drawingView.setLine(startX, startY, endX, endY);
            }
        }
    }
}