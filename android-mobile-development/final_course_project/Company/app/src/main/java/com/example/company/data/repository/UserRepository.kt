package com.example.company.data.repository

import com.example.company.data.dao.UserDao
import com.example.company.data.entity.User
import kotlinx.coroutines.flow.Flow
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class UserRepository @Inject constructor(private val userDao: UserDao) {

    suspend fun validateUser(name: String, password: String): User? =
        userDao.validateUser(name, password)

    suspend fun findByName(name: String): User? =
        userDao.findByName(name)

    fun getAllUsers(): Flow<List<User>> =
        userDao.getAllUsers()

    suspend fun addUser(user: User): Long =
        userDao.insert(user)

    suspend fun updateUser(user: User): Int =
        userDao.update(user)

    suspend fun deleteByName(name: String): Int =
        userDao.deleteByName(name)

    suspend fun getUserIdByName(name: String): Int? =
        userDao.getUserIdByName(name)

    suspend fun getUserById(id: Int): User? =
        userDao.getUserById(id)
}
