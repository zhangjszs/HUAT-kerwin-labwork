package com.example.company.ui.main

import android.content.Intent
import android.os.Bundle
import android.text.format.DateFormat
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.ListView
import android.widget.TextView
import android.widget.Toast
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import com.example.company.ui.appeal.AppealActivity
import com.example.company.ui.attendance.QiandaoActivity
import com.example.company.ui.history.AttendanceHistoryActivity
import com.example.company.ui.intro.IntroductionActivity
import com.example.company.ui.leave.LeaveRequestActivity
import com.example.company.ui.login.LoginActivity
import com.example.company.ui.news.NewsActivity
import com.example.company.ui.profile.ProfileActivity
import dagger.hilt.android.AndroidEntryPoint
import java.util.Calendar

@AndroidEntryPoint
class FirstActivity : AppCompatActivity() {

    private val viewModel: MainViewModel by viewModels()

    private var userId: Int = -1
    private var username: String? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_first)

        userId = intent.getIntExtra("user_id", -1)
        username = intent.getStringExtra("username")

        if (userId != -1 && !username.isNullOrEmpty()) {
            Toast.makeText(this, "欢迎回来, $username", Toast.LENGTH_SHORT).show()
        }

        val btnQiandao = findViewById<Button>(R.id.bt_qiandao)
        val tvTime = findViewById<TextView>(R.id.tv_qiandaotime)
        val currentDate = getCurrentDate()

        findViewById<Button>(R.id.btn_profile).setOnClickListener {
            startActivity(Intent(this, ProfileActivity::class.java).apply {
                putExtra("user_id", userId)
            })
        }

        btnQiandao.setOnClickListener {
            if (userId != -1 && username != null) {
                startActivity(Intent(this, QiandaoActivity::class.java).apply {
                    putExtra("user_id", userId)
                    putExtra("username", username)
                })
            } else {
                Toast.makeText(this, "用户信息获取失败，无法签到", Toast.LENGTH_SHORT).show()
            }
        }

        findViewById<Button>(R.id.login).setOnClickListener {
            startActivity(Intent(this, LoginActivity::class.java))
            finish()
        }

        findViewById<Button>(R.id.introduction).setOnClickListener {
            startActivity(Intent(this, IntroductionActivity::class.java))
        }

        findViewById<Button>(R.id.btn_appeal).setOnClickListener {
            startActivity(Intent(this, AppealActivity::class.java).apply {
                putExtra("user_id", userId)
            })
        }

        findViewById<Button>(R.id.btn_leave).setOnClickListener {
            startActivity(Intent(this, LeaveRequestActivity::class.java).apply {
                putExtra("user_id", userId)
            })
        }

        findViewById<Button>(R.id.btn_history).setOnClickListener {
            startActivity(Intent(this, AttendanceHistoryActivity::class.java).apply {
                putExtra("user_id", userId)
            })
        }

        val listNews = findViewById<ListView>(R.id.list_news)
        viewModel.newsList.observe(this) { newsList ->
            val adapter = ArrayAdapter(
                this, android.R.layout.simple_list_item_1,
                newsList.map { it.title }
            )
            listNews.adapter = adapter
            listNews.setOnItemClickListener { _, _, position, _ ->
                val newsId = newsList[position].id
                startActivity(Intent(this, NewsActivity::class.java).apply {
                    putExtra(NewsActivity.EXTRA_NEWS_ID, newsId)
                })
            }
        }

        viewModel.checkSignInStatus(userId, currentDate)
        viewModel.isSignedIn.observe(this) { signedIn ->
            if (signedIn) {
                tvTime.text = getString(R.string.date_signed_in, currentDate)
                btnQiandao.setBackgroundResource(R.drawable.yqd)
                btnQiandao.text = "✔签到"
            } else {
                tvTime.text = getString(R.string.date_not_signed, currentDate) +
                    "\n上班时间：09:00:00  下班时间：18:00:00"
                btnQiandao.setBackgroundResource(R.drawable.qd)
                btnQiandao.text = "签到"
            }
        }
    }

    private fun getCurrentDate(): String {
        return DateFormat.format("yyyy年MM月dd日", Calendar.getInstance()).toString()
    }
}
