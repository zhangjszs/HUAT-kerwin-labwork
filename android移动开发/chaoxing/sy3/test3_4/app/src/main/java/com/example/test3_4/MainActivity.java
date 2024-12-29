package com.example.test3_4;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.location.LocationProvider;
import android.os.Bundle;
import android.widget.TextView;
import androidx.core.app.ActivityCompat;

public class MainActivity extends Activity {
    private TextView infoView = null;
    private TextView locationView = null;
    private LocationManager locationManager = null;
    private LocationListener locationListener = null;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        infoView = (TextView) findViewById(R.id.infoView);
        locationView = (TextView) findViewById(R.id.locationView);

        // Initialize LocationManager and LocationListener
        locationManager_init();
    }

    // Initialize LocationManager
    void locationManager_init() {
        locationManager = (LocationManager) this.getSystemService(Context.LOCATION_SERVICE);
        locationListener_init();

        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, 1);
            return;
        }
        // Request location updates
        locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 1000, 0, locationListener);
    }

    void locationListener_init() {
        locationListener = new LocationListener() {
            // Location changed
            public void onLocationChanged(Location location) {
                infoView.setText("时间: " + location.getTime() + "\n");
                infoView.append("经度: " + location.getLongitude() + "\n");
                infoView.append("纬度: " + location.getLatitude() + "\n");
                locationView.setText("海拔: " + location.getAltitude() + "\n");
            }

            // Provider disabled
            public void onProviderDisabled(String provider) {
                infoView.setText("当前GPS状态: 禁用\n");
            }

            // Provider enabled
            public void onProviderEnabled(String provider) {
                infoView.setText("当前GPS状态: 开启\n");
            }

            // Status changed
            public void onStatusChanged(String provider, int status, Bundle extras) {
                if (status == LocationProvider.AVAILABLE) {
                    infoView.setText("当前GPS状态: 可见的\n");
                } else if (status == LocationProvider.OUT_OF_SERVICE) {
                    infoView.setText("当前GPS状态: 服务区外\n");
                } else if (status == LocationProvider.TEMPORARILY_UNAVAILABLE) {
                    infoView.setText("当前GPS状态: 暂停服务\n");
                }
            }
        };
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (locationManager != null && locationListener != null) {
            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_GRANTED || ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) == PackageManager.PERMISSION_GRANTED) {
                locationManager.removeUpdates(locationListener);
            }
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (locationManager != null && locationListener != null) {
            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_GRANTED || ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) == PackageManager.PERMISSION_GRANTED) {
                locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 1000, 0, locationListener);
            }
        }
    }
}



