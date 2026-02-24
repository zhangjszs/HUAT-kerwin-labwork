package com.example.company.ui.profile

import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class ProfileActivity : AppCompatActivity() {

    private val viewModel: ProfileViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_profile)

        val userId = intent.getIntExtra("user_id", -1)
        if (userId != -1) {
            viewModel.loadUser(userId)
        }

        viewModel.user.observe(this) { user ->
            user?.let {
                findViewById<TextView>(R.id.tv_name).text = it.name
                findViewById<TextView>(R.id.tv_gender).text = it.sex
                findViewById<TextView>(R.id.tv_phone).text = it.tel
                findViewById<TextView>(R.id.tv_position).text = it.position
                findViewById<TextView>(R.id.tv_hire_date).text = it.time
            }
        }

        findViewById<Button>(R.id.btn_back).setOnClickListener { finish() }
    }
}
