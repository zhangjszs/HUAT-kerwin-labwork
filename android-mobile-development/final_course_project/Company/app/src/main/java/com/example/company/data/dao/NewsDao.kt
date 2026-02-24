package com.example.company.data.dao

import androidx.room.*
import com.example.company.data.entity.News
import kotlinx.coroutines.flow.Flow

@Dao
interface NewsDao {
    @Query("SELECT * FROM NEWS")
    fun getAllNews(): Flow<List<News>>

    @Query("SELECT * FROM NEWS WHERE _id = :id")
    suspend fun getNewsById(id: Int): News?

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(news: News): Long
}
