package com.example.test4_1;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.RectF;
import android.view.View;

public class MyView extends View {
    // 添加构造函数
    public MyView(Context context) {
        super(context);
    }

    // 重写onDraw()函数
    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        // 设置背景颜色为白色
        canvas.drawColor(Color.WHITE);

        // 绘制圆形
        Paint paint1 = new Paint();
        // 设置颜色
        paint1.setColor(Color.rgb(0, 255, 0));
        // 设置笔触宽度
        paint1.setStrokeWidth(5);
        // 空心为Paint.Style.STROKE，实心为Paint.Style.FILL
        paint1.setStyle(Paint.Style.STROKE);
        canvas.drawCircle(100, 50, 30, paint1);

        // 绘制文本
        Paint paint2 = new Paint();
        paint2.setColor(Color.RED);
        paint2.setTextSize(20);
        canvas.drawText("天行健，君子自强不息", 50, 150, paint2);

        // 绘制路径
        Path pathLine = new Path();
        pathLine.moveTo(150, 200);
        pathLine.lineTo(200, 245);
        pathLine.lineTo(250, 200);
        pathLine.lineTo(300, 280);
        canvas.drawPath(pathLine, paint1);

        // 沿着路径绘制文本
        Path pathLine2 = new Path();
        // 添加矩形路径，Path.Direction.CW为顺时针，Path.Direction.CCW逆时针
        pathLine2.addRect(new RectF(30, 200, 70, 250), Path.Direction.CW);
        paint2.setTextSize(18);
        canvas.drawTextOnPath("湖北汽车工业学院计算机", pathLine2, 0, 0, paint2);
    }
}