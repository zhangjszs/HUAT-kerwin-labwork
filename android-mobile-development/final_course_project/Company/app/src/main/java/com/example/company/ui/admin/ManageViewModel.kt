package com.example.company.ui.admin

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.company.data.entity.User
import com.example.company.data.repository.AttendanceRepository
import com.example.company.data.repository.UserRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

sealed class ManageResult {
    object Idle : ManageResult()
    data class Success(val message: String) : ManageResult()
    data class Error(val message: String) : ManageResult()
}

@HiltViewModel
class ManageViewModel @Inject constructor(
    private val userRepository: UserRepository,
    private val attendanceRepository: AttendanceRepository
) : ViewModel() {

    private val _result = MutableLiveData<ManageResult>(ManageResult.Idle)
    val result: LiveData<ManageResult> = _result

    fun addUser(name: String, password: String, sex: String, tel: String, position: String, time: String) {
        if (name.isBlank() || password.isBlank()) {
            _result.value = ManageResult.Error("用户名和密码不能为空")
            return
        }
        viewModelScope.launch {
            val existing = userRepository.findByName(name)
            if (existing != null) {
                _result.value = ManageResult.Error("添加失败，用户已存在")
                return@launch
            }
            val id = userRepository.addUser(
                User(name = name, password = password, sex = sex, tel = tel, position = position, time = time)
            )
            _result.value = if (id != -1L) ManageResult.Success("添加成功") else ManageResult.Error("添加失败")
        }
    }

    fun deleteUser(name: String) {
        if (name.isBlank()) {
            _result.value = ManageResult.Error("请输入要删除的用户名")
            return
        }
        viewModelScope.launch {
            val userId = userRepository.getUserIdByName(name)
            if (userId != null) {
                attendanceRepository.deleteByUserId(userId)
            }
            val count = userRepository.deleteByName(name)
            _result.value = if (count > 0) ManageResult.Success("删除成功") else ManageResult.Error("删除失败，用户不存在")
        }
    }

    fun updateUser(name: String, password: String, sex: String, tel: String, position: String, time: String) {
        if (name.isBlank()) {
            _result.value = ManageResult.Error("请输入要更新的用户名")
            return
        }
        viewModelScope.launch {
            val existing = userRepository.findByName(name) ?: run {
                _result.value = ManageResult.Error("更新失败，用户不存在")
                return@launch
            }
            val updated = existing.copy(password = password, sex = sex, tel = tel, position = position, time = time)
            val count = userRepository.updateUser(updated)
            _result.value = if (count > 0) ManageResult.Success("更新成功") else ManageResult.Error("更新失败")
        }
    }

    fun resetResult() {
        _result.value = ManageResult.Idle
    }
}
