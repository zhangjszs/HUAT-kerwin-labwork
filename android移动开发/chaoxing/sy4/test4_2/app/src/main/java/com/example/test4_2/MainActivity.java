package com.example.test4_2;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.os.Bundle;
import android.view.View;

public class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // 设置显示MyView对象，所以没用到布局文件
        setContentView(new MyView(this));
    }

    // 定义一个内部类MyView
    public class MyView extends View {
        // 添加构造函数
        public MyView(Context context) {
            super(context);
        }

        @Override
        protected void onDraw(Canvas canvas) {
            super.onDraw(canvas);

            Paint paint = new Paint();
            Resources res = getResources();
            Bitmap bitmap = BitmapFactory.decodeResource(res, R.drawable.rabbit);

            // 在[20, 20]处绘制原图
            canvas.drawBitmap(bitmap, 20, 20, paint);

            // 将原图平移到指定位置
            Matrix mxt = new Matrix();
            mxt.setTranslate(150, 20);
            canvas.drawBitmap(bitmap, mxt, paint);

            // 绘制旋转变换后的图像
            Matrix mxr = new Matrix();
            mxr.setRotate(60); // 先旋转
            mxr.postTranslate(70, 100); // 再平移
            canvas.drawBitmap(bitmap, mxr, paint);

            // 绘制倾斜变换后的图像
            Matrix mxsk = new Matrix();
            mxsk.setSkew(2f, 1f); // 先倾斜
            mxsk.postTranslate(80, 90); // 再平移
            canvas.drawBitmap(bitmap, mxsk, paint);

            // 绘制缩放变换后的图像
            Matrix mxsc = new Matrix();
            mxsc.setScale(1.5f, -2.0f);
            mxsc.postTranslate(100, 380); // 再平移
            canvas.drawBitmap(bitmap, mxsc, paint);
        }
    }
}