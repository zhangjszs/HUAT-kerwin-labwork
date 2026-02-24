package com.example.company.data.repository

import com.example.company.data.dao.NewsDao
import com.example.company.data.entity.News
import kotlinx.coroutines.flow.Flow
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class NewsRepository @Inject constructor(private val newsDao: NewsDao) {

    fun getAllNews(): Flow<List<News>> =
        newsDao.getAllNews()

    suspend fun getNewsById(id: Int): News? =
        newsDao.getNewsById(id)
}
