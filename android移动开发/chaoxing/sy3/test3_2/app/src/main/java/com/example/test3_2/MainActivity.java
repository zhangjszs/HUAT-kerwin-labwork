package com.example.test3_2;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Scanner;

public class MainActivity extends AppCompatActivity {

    private EditText etName, etPasswd;
    private TextView responseText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        etName = findViewById(R.id.etName);
        etPasswd = findViewById(R.id.etPasswd);
        responseText = findViewById(R.id.responseText);

        Button btnGet = findViewById(R.id.btnGet);
        Button btnPost = findViewById(R.id.btnPost);

        btnGet.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        final String result = sendGetRequest();
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                responseText.setText(result);
                            }
                        });
                    }
                }).start();
            }
        });

        btnPost.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        final String result = sendPostRequest();
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                responseText.setText(result);
                            }
                        });
                    }
                }).start();
            }
        });
    }

    private String sendGetRequest() {
        StringBuilder response = new StringBuilder();
        try {
            URL url = new URL("http://10.21.56.243/Android3_2/test.jsp?username=" + etName.getText().toString() + "&password=" + etPasswd.getText().toString());
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod("GET");
            connection.connect();

            int responseCode = connection.getResponseCode();
            if (responseCode == HttpURLConnection.HTTP_OK) {
                BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
                String inputLine;
                while ((inputLine = in.readLine()) != null) {
                    response.append(inputLine);
                }
                in.close();
            } else {
                response.append("Error Code : ").append(responseCode);
            }
        } catch (Exception e) {
            e.printStackTrace();
            return "Error: " + e.getMessage();
        }
        return response.toString();
    }

    private String sendPostRequest() {
        StringBuilder response = new StringBuilder();
        try {
            URL url = new URL("http://192.168.103.173/Android3_2/test.jsp");
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod("POST");
            connection.setDoOutput(true);

            OutputStream os = connection.getOutputStream();
            String postParams = "username=" + etName.getText().toString() + "&password=" + etPasswd.getText().toString();
            os.write(postParams.getBytes());
            os.flush();
            os.close();

            int responseCode = connection.getResponseCode();
            if (responseCode == HttpURLConnection.HTTP_OK) {
                Scanner scanner = new Scanner(connection.getInputStream()).useDelimiter("\\A");
                response.append(scanner.hasNext() ? scanner.next() : "");
                scanner.close();
            } else {
                response.append("Error Code : ").append(responseCode);
            }
        } catch (Exception e) {
            e.printStackTrace();
            return "Error: " + e.getMessage();
        }
        return response.toString();
    }
}



