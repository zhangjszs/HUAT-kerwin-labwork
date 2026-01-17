package com.example.test4_7;

import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

public class MainActivity extends Activity implements View.OnClickListener, SurfaceHolder.Callback {
    String path = "/mnt/shared/Pictures/shipin.mp4"; // 视频路径
    Button play_Button;
    Button pause_Button;
    boolean isPause = false;
    SurfaceView surfaceView;
    SurfaceHolder surfaceHolder;
    MediaPlayer mediaPlayer;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, 1);
        } else {
            initViews();
        }
    }

    private void initViews() {
        play_Button = findViewById(R.id.play2_Button);
        play_Button.setOnClickListener(this);

        pause_Button = findViewById(R.id.pause2_Button);
        pause_Button.setOnClickListener(this);

        surfaceView = findViewById(R.id.surfaceView);
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(this);
        surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);

        mediaPlayer = new MediaPlayer();
    }

    @Override
    public void onClick(View v) {
        if (v == play_Button) { // 按下播放电影按钮
            isPause = false;
            playVideo(path);
        } else if (v == pause_Button) { // 按下暂停按钮
            if (isPause == false) { // 如果正在播放则将其暂停
                mediaPlayer.pause();
                isPause = true;
            } else { // 如果暂停中则继续播放
                mediaPlayer.start();
                isPause = false;
            }
        }
    }

    private void playVideo(String strPath) { // 自定义播放影片函数
        if (mediaPlayer.isPlaying()) {
            mediaPlayer.reset();
        }
        mediaPlayer.setDisplay(surfaceHolder); // 设置视频以SurfaceHolder播放
        try {
            mediaPlayer.setDataSource(strPath);
            mediaPlayer.prepareAsync(); // 使用异步准备
            mediaPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
                @Override
                public void onPrepared(MediaPlayer mp) {
                    mediaPlayer.start(); // 准备完成后开始播放
                }
            });
        } catch (Exception e) {
            Toast.makeText(this, "无法播放视频", Toast.LENGTH_SHORT).show();
            e.printStackTrace();
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {}

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        // 初始化MediaPlayer对象
        mediaPlayer = new MediaPlayer();
        mediaPlayer.setDisplay(holder);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        if (mediaPlayer != null && mediaPlayer.isPlaying()) {
            mediaPlayer.stop();
            mediaPlayer.release();
            mediaPlayer = null;
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mediaPlayer != null) {
            if (mediaPlayer.isPlaying()) {
                mediaPlayer.stop();
            }
            mediaPlayer.release();
            mediaPlayer = null;
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        if (requestCode == 1) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                initViews();
            } else {
                Toast.makeText(this, "Permission denied to read external storage", Toast.LENGTH_SHORT).show();
            }
        }
    }
}



