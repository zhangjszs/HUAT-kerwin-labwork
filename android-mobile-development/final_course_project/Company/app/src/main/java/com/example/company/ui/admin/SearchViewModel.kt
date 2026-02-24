package com.example.company.ui.admin

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.company.data.entity.User
import com.example.company.data.repository.UserRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class SearchViewModel @Inject constructor(
    private val userRepository: UserRepository
) : ViewModel() {

    private val _searchResult = MutableLiveData<User?>()
    val searchResult: LiveData<User?> = _searchResult

    private val _error = MutableLiveData<String?>()
    val error: LiveData<String?> = _error

    fun search(name: String) {
        if (name.isBlank()) {
            _error.value = "请输入要搜索的用户名"
            return
        }
        viewModelScope.launch {
            val user = userRepository.findByName(name)
            if (user != null) {
                _searchResult.value = user
                _error.value = null
            } else {
                _searchResult.value = null
                _error.value = "未找到该用户"
            }
        }
    }
}
