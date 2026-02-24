package com.example.company.data.dao

import androidx.room.*
import com.example.company.data.entity.Appeal

@Dao
interface AppealDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(appeal: Appeal): Long

    @Query("SELECT * FROM APPEAL WHERE user_id = :userId ORDER BY _id DESC")
    suspend fun getByUserId(userId: Int): List<Appeal>

    @Query("SELECT * FROM APPEAL WHERE status = 'PENDING' ORDER BY _id DESC")
    suspend fun getPending(): List<Appeal>

    @Query("SELECT * FROM APPEAL WHERE _id = :id")
    suspend fun getById(id: Int): Appeal?

    @Update
    suspend fun update(appeal: Appeal)
}
