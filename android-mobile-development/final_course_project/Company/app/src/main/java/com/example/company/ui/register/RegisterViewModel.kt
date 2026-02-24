package com.example.company.ui.register

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.company.data.entity.User
import com.example.company.data.repository.UserRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

sealed class RegisterState {
    object Idle : RegisterState()
    data class Success(val userId: Long) : RegisterState()
    data class Error(val message: String) : RegisterState()
}

@HiltViewModel
class RegisterViewModel @Inject constructor(
    private val userRepository: UserRepository
) : ViewModel() {

    private val _registerState = MutableLiveData<RegisterState>(RegisterState.Idle)
    val registerState: LiveData<RegisterState> = _registerState

    fun register(username: String, password1: String, password2: String) {
        if (username.isBlank() || password1.isBlank() || password2.isBlank()) {
            _registerState.value = RegisterState.Error("未完善信息，注册失败")
            return
        }
        if (password1 != password2) {
            _registerState.value = RegisterState.Error("密码不一致，注册失败")
            return
        }
        viewModelScope.launch {
            val user = User(name = username, password = password1)
            val id = userRepository.addUser(user)
            if (id != -1L) {
                _registerState.value = RegisterState.Success(id)
            } else {
                _registerState.value = RegisterState.Error("注册失败，用户可能已存在")
            }
        }
    }
}
