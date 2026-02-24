package com.example.company.ui.news

import android.os.Bundle
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import com.example.company.R
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class NewsActivity : AppCompatActivity() {

    companion object {
        const val EXTRA_NEWS_ID = "newsId"
    }

    private val viewModel: NewsViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_news)

        val newsId = intent.getIntExtra(EXTRA_NEWS_ID, -1)
        if (newsId == -1) {
            Toast.makeText(this, "新闻不存在", Toast.LENGTH_SHORT).show()
            finish()
            return
        }

        viewModel.loadNews(newsId)

        viewModel.news.observe(this) { news ->
            if (news != null) {
                findViewById<TextView>(R.id.title).text = news.title
                findViewById<ImageView>(R.id.image_news).apply {
                    setImageResource(news.imageResourceId)
                    contentDescription = news.title
                }
                findViewById<TextView>(R.id.content).text = news.content
            }
        }
    }
}
