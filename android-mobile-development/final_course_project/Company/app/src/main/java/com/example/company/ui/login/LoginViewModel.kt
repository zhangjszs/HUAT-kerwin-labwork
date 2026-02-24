package com.example.company.ui.login

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.company.data.entity.User
import com.example.company.data.repository.UserRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

sealed class LoginState {
    object Idle : LoginState()
    object Loading : LoginState()
    data class Success(val user: User) : LoginState()
    data class Error(val message: String) : LoginState()
}

@HiltViewModel
class LoginViewModel @Inject constructor(
    private val userRepository: UserRepository
) : ViewModel() {

    private val _loginState = MutableLiveData<LoginState>(LoginState.Idle)
    val loginState: LiveData<LoginState> = _loginState

    fun login(name: String, password: String) {
        if (name.isBlank()) {
            _loginState.value = LoginState.Error("请输入用户名")
            return
        }
        if (password.isBlank()) {
            _loginState.value = LoginState.Error("请输入密码")
            return
        }
        _loginState.value = LoginState.Loading
        viewModelScope.launch {
            val user = userRepository.validateUser(name, password)
            if (user != null) {
                _loginState.value = LoginState.Success(user)
            } else {
                _loginState.value = LoginState.Error("用户名或密码不正确，请重新输入")
            }
        }
    }
}
