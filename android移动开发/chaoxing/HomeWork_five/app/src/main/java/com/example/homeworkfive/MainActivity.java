package com.example.homeworkfive;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

public class MainActivity extends AppCompatActivity {

    private static final String FILE_NAME = "records.txt";
    private static final int LOCATION_PERMISSION_REQUEST_CODE = 1001; // Request code

    private TextView recordsTextView;
    private LocationManager locationManager;
    private LocationListener locationListener;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button recordButton = findViewById(R.id.recordButton);
        Button showRecordsButton = findViewById(R.id.showRecordsButton);
        recordsTextView = findViewById(R.id.recordsTextView);

        recordButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                recordTimeWithLocation(); // Call method with location
            }
        });

        showRecordsButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                showRecords();
            }
        });

        // Initialize LocationManager
        locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
    }

    private void recordTimeWithLocation() {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, LOCATION_PERMISSION_REQUEST_CODE);
        } else {
            getLocationAndRecord();
        }
    }

    private void getLocationAndRecord() {
        locationListener = new LocationListener() {
            @Override
            public void onLocationChanged(Location location) {
                String time = getCurrentTime();
                String locationInfo = "Longitude: " + location.getLongitude() + ", Latitude: " + location.getLatitude();
                String record = time + " - " + locationInfo;

                try (FileOutputStream fos = openFileOutput(FILE_NAME, Context.MODE_APPEND)) {
                    fos.write((record + "\n").getBytes());
                    Toast.makeText(MainActivity.this, "打卡成功: " + record, Toast.LENGTH_SHORT).show();
                } catch (IOException e) {

                    e.printStackTrace();
                    Toast.makeText(MainActivity.this, "打卡失败", Toast.LENGTH_SHORT).show();
                }

                // Remove listener after getting location to save battery
                locationManager.removeUpdates(locationListener);
            }

            @Override
            public void onProviderDisabled(String provider) {
                Toast.makeText(MainActivity.this, "Please enable GPS", Toast.LENGTH_SHORT).show();
            }
        };

        try {
            locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, locationListener);
        } catch (SecurityException e) {
            e.printStackTrace();
            Toast.makeText(this, "Failed to get location information", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == LOCATION_PERMISSION_REQUEST_CODE) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                getLocationAndRecord();
            } else {
                Toast.makeText(this, "Location permission denied", Toast.LENGTH_SHORT).show();
            }
        }
    }

    private void showRecords() {
        StringBuilder records = new StringBuilder();
        try (FileInputStream fis = openFileInput(FILE_NAME);
             InputStreamReader isr = new InputStreamReader(fis);
             BufferedReader br = new BufferedReader(isr)) {

            String line;
            while ((line = br.readLine()) != null) { // Correct way to read lines
                records.append(line).append("\n"); // Append line and newline
            }
        } catch (IOException e) {
            e.printStackTrace();
            Toast.makeText(this, "Failed to read records", Toast.LENGTH_SHORT).show();
        }
        recordsTextView.setText(records.toString().isEmpty() ? "No records yet" : records.toString());
    }

    private String getCurrentTime() {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
        return sdf.format(new Date());
    }
}