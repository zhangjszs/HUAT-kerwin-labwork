package com.example.homeworkfour;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class MainActivity extends AppCompatActivity {

    private static final int REQUEST_CODE_PERMISSION = 1;
    private TextView resultTextView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        resultTextView = findViewById(R.id.resultTextView);

        // Check permission
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, REQUEST_CODE_PERMISSION);
        } else {
            readFileAndCalculateSum();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == REQUEST_CODE_PERMISSION) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                readFileAndCalculateSum();
            } else {
                Toast.makeText(this, "Permission denied to read external storage", Toast.LENGTH_SHORT).show();
            }
        }
    }

    private void readFileAndCalculateSum() {
        File file = new File(Environment.getExternalStorageDirectory(), "numbers.txt");
        if (file.exists()) {
            // Read from file if it exists
            try (BufferedReader br = new BufferedReader(new FileReader(file))) {
                String line = br.readLine();
                if (line != null) {
                    String[] numbers = line.split(" ");
                    int sum = 0;
                    for (String number : numbers) {
                        sum += Integer.parseInt(number);
                    }
                    resultTextView.setText("Sum from file: " + sum);
                    return; // Exit after reading from file
                }
            } catch (IOException e) {
                e.printStackTrace();
                Toast.makeText(this, "Error reading file", Toast.LENGTH_SHORT).show();
            }
        }

        // Fallback: Calculate sum of 1 to 100 if file doesn't exist
        int sum = 0;
        for (int i = 1; i <= 100; i++) {
            sum += i;
        }
        resultTextView.setText("Sum (1-100): " + sum);
    }
}