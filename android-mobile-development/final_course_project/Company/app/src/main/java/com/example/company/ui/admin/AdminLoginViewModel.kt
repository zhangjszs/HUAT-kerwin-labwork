package com.example.company.ui.admin

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.company.data.repository.AdminRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

sealed class AdminLoginState {
    object Idle : AdminLoginState()
    object Success : AdminLoginState()
    data class Error(val message: String) : AdminLoginState()
}

@HiltViewModel
class AdminLoginViewModel @Inject constructor(
    private val adminRepository: AdminRepository
) : ViewModel() {

    private val _state = MutableLiveData<AdminLoginState>(AdminLoginState.Idle)
    val state: LiveData<AdminLoginState> = _state

    fun login(name: String, password: String) {
        if (name.isBlank() || password.isBlank()) {
            _state.value = AdminLoginState.Error("请输入用户名和密码")
            return
        }
        viewModelScope.launch {
            val admin = adminRepository.validateAdmin(name, password)
            _state.value = if (admin != null) AdminLoginState.Success
            else AdminLoginState.Error("用户名或密码不正确")
        }
    }
}
