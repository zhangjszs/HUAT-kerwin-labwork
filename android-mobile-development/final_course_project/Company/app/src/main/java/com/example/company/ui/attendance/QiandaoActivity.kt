package com.example.company.ui.attendance

import android.Manifest
import android.content.pm.PackageManager
import android.os.Bundle
import android.text.format.DateFormat
import android.widget.Button
import android.widget.TextView
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import androidx.lifecycle.lifecycleScope
import com.example.company.R
import com.example.company.util.GeofenceResult
import com.example.company.util.LocationHelper
import dagger.hilt.android.AndroidEntryPoint
import kotlinx.coroutines.launch
import java.util.Calendar

@AndroidEntryPoint
class QiandaoActivity : AppCompatActivity() {

    private val viewModel: AttendanceViewModel by viewModels()
    private lateinit var locationHelper: LocationHelper
    private lateinit var btnSignIn: Button
    private lateinit var btnSignOut: Button
    private lateinit var tvLocationStatus: TextView
    private lateinit var tvDistance: TextView
    private var geofenceOk = false

    private val permissionLauncher = registerForActivityResult(
        ActivityResultContracts.RequestMultiplePermissions()
    ) { permissions ->
        val granted = permissions.values.all { it }
        if (granted) {
            checkLocation()
        } else {
            tvLocationStatus.text = "位置权限被拒绝，无法签到"
            setButtonsEnabled(false)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_qiandao)

        locationHelper = LocationHelper(this)

        val userId = intent.getIntExtra("user_id", -1)
        val username = intent.getStringExtra("username")
        val currentDate = DateFormat.format("yyyy年MM月dd日", Calendar.getInstance()).toString()

        val tvTime = findViewById<TextView>(R.id.tv_qiandao_time)
        btnSignIn = findViewById(R.id.btn_qiandao_confirm)
        btnSignOut = findViewById(R.id.btn_qiantui_confirm)
        tvLocationStatus = findViewById(R.id.tv_location_status)
        tvDistance = findViewById(R.id.tv_distance)

        tvTime.text = "今天日期：$currentDate"

        if (userId != -1 && username != null) {
            Toast.makeText(this, "欢迎 $username 签到！", Toast.LENGTH_SHORT).show()
        }

        setButtonsEnabled(false)
        viewModel.loadTodayRecord(userId, currentDate)

        btnSignIn.setOnClickListener {
            if (geofenceOk) viewModel.signIn(userId, currentDate)
            else Toast.makeText(this, "不在打卡范围内", Toast.LENGTH_SHORT).show()
        }
        btnSignOut.setOnClickListener {
            if (geofenceOk) viewModel.signOut(userId, currentDate)
            else Toast.makeText(this, "不在打卡范围内", Toast.LENGTH_SHORT).show()
        }

        viewModel.state.observe(this) { state ->
            when (state) {
                is AttendanceState.SignedIn -> {
                    Toast.makeText(this, "签到成功！状态：${state.status}", Toast.LENGTH_SHORT).show()
                    btnSignIn.text = "今日已签到"
                    btnSignIn.isEnabled = false
                }
                is AttendanceState.SignedOut -> {
                    Toast.makeText(this, "签退成功！状态：${state.status}", Toast.LENGTH_SHORT).show()
                    btnSignOut.text = "今日已签退"
                    btnSignOut.isEnabled = false
                }
                is AttendanceState.AlreadySignedIn -> {
                    btnSignIn.text = "今日已签到"
                    btnSignIn.isEnabled = false
                    if (state.record.timeOut.isNotEmpty()) {
                        btnSignOut.text = "今日已签退"
                        btnSignOut.isEnabled = false
                    }
                }
                is AttendanceState.Error -> {
                    Toast.makeText(this, state.message, Toast.LENGTH_SHORT).show()
                }
                else -> {}
            }
        }

        requestLocationPermission()
    }

    private fun requestLocationPermission() {
        val fine = ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION)
        val coarse = ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION)
        if (fine == PackageManager.PERMISSION_GRANTED && coarse == PackageManager.PERMISSION_GRANTED) {
            checkLocation()
        } else {
            permissionLauncher.launch(arrayOf(
                Manifest.permission.ACCESS_FINE_LOCATION,
                Manifest.permission.ACCESS_COARSE_LOCATION
            ))
        }
    }

    private fun checkLocation() {
        lifecycleScope.launch {
            tvLocationStatus.text = "正在获取位置..."
            when (val result = locationHelper.checkGeofence()) {
                is GeofenceResult.WithinRange -> {
                    geofenceOk = true
                    setButtonsEnabled(true)
                    tvLocationStatus.text = "✓ 已进入打卡范围"
                    tvDistance.text = "距离公司：%.0f米".format(result.distance)
                }
                is GeofenceResult.OutOfRange -> {
                    geofenceOk = false
                    setButtonsEnabled(false)
                    tvLocationStatus.text = "✗ 不在打卡范围内"
                    tvDistance.text = "距离公司：%.0f米（需≤200米）".format(result.distance)
                }
                is GeofenceResult.MockDetected -> {
                    geofenceOk = false
                    setButtonsEnabled(false)
                    tvLocationStatus.text = "✗ 检测到模拟定位"
                    tvDistance.text = ""
                }
                is GeofenceResult.Error -> {
                    geofenceOk = false
                    setButtonsEnabled(false)
                    tvLocationStatus.text = "✗ 定位失败：${result.message}"
                    tvDistance.text = ""
                }
            }
        }
    }

    private fun setButtonsEnabled(enabled: Boolean) {
        btnSignIn.isEnabled = enabled
        btnSignOut.isEnabled = enabled
    }
}
