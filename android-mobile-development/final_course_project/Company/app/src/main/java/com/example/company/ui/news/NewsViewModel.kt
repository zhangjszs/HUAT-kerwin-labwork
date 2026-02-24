package com.example.company.ui.news

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.company.data.entity.News
import com.example.company.data.repository.NewsRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class NewsViewModel @Inject constructor(
    private val newsRepository: NewsRepository
) : ViewModel() {

    private val _news = MutableLiveData<News?>()
    val news: LiveData<News?> = _news

    fun loadNews(id: Int) {
        viewModelScope.launch {
            _news.value = newsRepository.getNewsById(id)
        }
    }
}
