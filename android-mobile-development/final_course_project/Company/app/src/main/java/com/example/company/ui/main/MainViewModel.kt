package com.example.company.ui.main

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.asLiveData
import androidx.lifecycle.viewModelScope
import com.example.company.data.entity.News
import com.example.company.data.repository.AttendanceRepository
import com.example.company.data.repository.NewsRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class MainViewModel @Inject constructor(
    private val newsRepository: NewsRepository,
    private val attendanceRepository: AttendanceRepository
) : ViewModel() {

    val newsList: LiveData<List<News>> = newsRepository.getAllNews().asLiveData()

    private val _isSignedIn = MutableLiveData(false)
    val isSignedIn: LiveData<Boolean> = _isSignedIn

    fun checkSignInStatus(userId: Int, date: String) {
        viewModelScope.launch {
            val record = attendanceRepository.getTodayRecord(userId, date)
            _isSignedIn.value = record != null
        }
    }
}
