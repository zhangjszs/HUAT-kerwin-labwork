package com.example.company.data.dao

import androidx.room.*
import com.example.company.data.entity.User
import kotlinx.coroutines.flow.Flow

@Dao
interface UserDao {
    @Query("SELECT * FROM USER WHERE name = :name AND password = :password LIMIT 1")
    suspend fun validateUser(name: String, password: String): User?

    @Query("SELECT * FROM USER WHERE name = :name LIMIT 1")
    suspend fun findByName(name: String): User?

    @Query("SELECT * FROM USER ORDER BY name DESC")
    fun getAllUsers(): Flow<List<User>>

    @Insert(onConflict = OnConflictStrategy.ABORT)
    suspend fun insert(user: User): Long

    @Update
    suspend fun update(user: User): Int

    @Query("DELETE FROM USER WHERE name = :name")
    suspend fun deleteByName(name: String): Int

    @Query("SELECT _id FROM USER WHERE name = :name LIMIT 1")
    suspend fun getUserIdByName(name: String): Int?

    @Query("SELECT * FROM USER WHERE _id = :id LIMIT 1")
    suspend fun getUserById(id: Int): User?
}
