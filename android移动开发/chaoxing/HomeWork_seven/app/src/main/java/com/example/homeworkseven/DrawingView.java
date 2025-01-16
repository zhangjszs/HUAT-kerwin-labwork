package com.example.homeworkseven;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.View;

public class DrawingView extends View {

    private Paint paint;
    private RectF rect;
    private float circleCx, circleCy, circleRadius;
    private float lineStartX, lineStartY, lineEndX, lineEndY;

    public DrawingView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init() {
        paint = new Paint();
        paint.setColor(Color.BLACK);
        paint.setStrokeWidth(5);
        paint.setStyle(Paint.Style.STROKE);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        // 绘制矩形
        if (rect != null) {
            canvas.drawRect(rect, paint);
        }

        // 绘制圆
        if (circleRadius > 0) {
            canvas.drawCircle(circleCx, circleCy, circleRadius, paint);
        }

        // 绘制直线
        canvas.drawLine(lineStartX, lineStartY, lineEndX, lineEndY, paint);
    }

    // 设置矩形坐标
    public void setRect(float left, float top, float right, float bottom) {
        rect = new RectF(left, top, right, bottom);
        invalidate(); // 刷新视图
    }

    // 设置圆坐标和半径
    public void setCircle(float cx, float cy, float radius) {
        circleCx = cx;
        circleCy = cy;
        circleRadius = radius;
        invalidate(); // 刷新视图
    }

    // 设置直线坐标
    public void setLine(float startX, float startY, float endX, float endY) {
        lineStartX = startX;
        lineStartY = startY;
        lineEndX = endX;
        lineEndY = endY;
        invalidate(); // 刷新视图
    }
}