package com.example.company.data.repository

import com.example.company.data.dao.AdminUserDao
import com.example.company.data.entity.AdminUser
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class AdminRepository @Inject constructor(private val adminUserDao: AdminUserDao) {

    suspend fun validateAdmin(name: String, password: String): AdminUser? =
        adminUserDao.validateAdmin(name, password)
}
