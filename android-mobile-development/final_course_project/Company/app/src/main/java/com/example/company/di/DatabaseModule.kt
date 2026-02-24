package com.example.company.di

import android.content.Context
import com.example.company.data.CompanyDatabase
import com.example.company.data.dao.AdminUserDao
import com.example.company.data.dao.AppealDao
import com.example.company.data.dao.AttendanceDao
import com.example.company.data.dao.LeaveRequestDao
import com.example.company.data.dao.NewsDao
import com.example.company.data.dao.UserDao
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.android.qualifiers.ApplicationContext
import dagger.hilt.components.SingletonComponent
import javax.inject.Singleton

@Module
@InstallIn(SingletonComponent::class)
object DatabaseModule {

    @Provides
    @Singleton
    fun provideDatabase(@ApplicationContext context: Context): CompanyDatabase =
        CompanyDatabase.getDatabase(context)

    @Provides
    fun provideUserDao(db: CompanyDatabase): UserDao = db.userDao()

    @Provides
    fun provideAdminUserDao(db: CompanyDatabase): AdminUserDao = db.adminUserDao()

    @Provides
    fun provideNewsDao(db: CompanyDatabase): NewsDao = db.newsDao()

    @Provides
    fun provideAttendanceDao(db: CompanyDatabase): AttendanceDao = db.attendanceDao()

    @Provides
    fun provideAppealDao(db: CompanyDatabase): AppealDao = db.appealDao()

    @Provides
    fun provideLeaveRequestDao(db: CompanyDatabase): LeaveRequestDao = db.leaveRequestDao()
}
