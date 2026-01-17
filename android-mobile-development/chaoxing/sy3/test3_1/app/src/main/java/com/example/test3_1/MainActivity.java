package com.example.test3_1;

import android.os.Bundle;
import android.app.Activity;
import android.widget.EditText;
import android.util.Log;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;

public class MainActivity extends Activity {
    private Socket mysocket;
    private EditText clientText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        clientText = (EditText) findViewById(R.id.ClientText);

        new Thread(() -> {
            try {
                // 指定服务器的IP地址和服务器端口，和服务器端建立连接，这里假设服务器IP是正确的，实际中需准确配置
                mysocket = new Socket("10.21.56.243", 8080);
                // 通过PrintWriter向服务器发送信息
                String cmessage = "I am client!";
                PrintWriter out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(mysocket.getOutputStream(), "UTF-8")), true);
                out.println(cmessage);
                Log.d("Client", "已向服务器发送消息：" + cmessage);

                // 通过BufferedReader读取服务器发送过来的信息
                BufferedReader br = new BufferedReader(new InputStreamReader(mysocket.getInputStream()));
                String smessage = br.readLine();
                Log.d("Client", "收到服务器发来的消息：" + smessage);
                // 将信息显示在输入框中，注意要在主线程中更新UI，这里简单示例，实际需更严谨处理
                runOnUiThread(() -> clientText.setText(smessage));

                // 关闭资源
                out.close();
                br.close();
                mysocket.close();
            } catch (IOException e) {
                Log.e("Client", "客户端连接或通信出现异常：", e);
            }
        }).start();
    }
}