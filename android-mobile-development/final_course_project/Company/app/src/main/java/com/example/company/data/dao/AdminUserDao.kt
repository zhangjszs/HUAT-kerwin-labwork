package com.example.company.data.dao

import androidx.room.*
import com.example.company.data.entity.AdminUser

@Dao
interface AdminUserDao {
    @Query("SELECT * FROM ADMINUSER WHERE admin_name = :name AND admin_password = :password LIMIT 1")
    suspend fun validateAdmin(name: String, password: String): AdminUser?

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(admin: AdminUser): Long
}
