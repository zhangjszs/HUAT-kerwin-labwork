package com.example.company.data.repository

import com.example.company.data.dao.AppealDao
import com.example.company.data.entity.Appeal
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class AppealRepository @Inject constructor(private val appealDao: AppealDao) {

    suspend fun insert(appeal: Appeal): Long = appealDao.insert(appeal)

    suspend fun getByUserId(userId: Int): List<Appeal> = appealDao.getByUserId(userId)

    suspend fun getPending(): List<Appeal> = appealDao.getPending()

    suspend fun getById(id: Int): Appeal? = appealDao.getById(id)

    suspend fun update(appeal: Appeal) = appealDao.update(appeal)
}
