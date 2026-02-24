package com.example.company.util

import android.annotation.SuppressLint
import android.content.Context
import android.location.Location
import android.os.Build
import com.google.android.gms.location.LocationServices
import com.google.android.gms.location.Priority
import com.google.android.gms.tasks.CancellationTokenSource
import kotlinx.coroutines.suspendCancellableCoroutine
import kotlin.coroutines.resume

sealed class GeofenceResult {
    data class WithinRange(val distance: Float) : GeofenceResult()
    data class OutOfRange(val distance: Float) : GeofenceResult()
    object MockDetected : GeofenceResult()
    data class Error(val message: String) : GeofenceResult()
}

class LocationHelper(private val context: Context) {

    private val fusedClient = LocationServices.getFusedLocationProviderClient(context)

    @SuppressLint("MissingPermission")
    suspend fun getCurrentLocation(): Result<Location> = suspendCancellableCoroutine { cont ->
        val cts = CancellationTokenSource()
        fusedClient.getCurrentLocation(Priority.PRIORITY_HIGH_ACCURACY, cts.token)
            .addOnSuccessListener { location ->
                if (location != null) {
                    cont.resume(Result.success(location))
                } else {
                    cont.resume(Result.failure(Exception("无法获取位置")))
                }
            }
            .addOnFailureListener { e ->
                cont.resume(Result.failure(e))
            }
        cont.invokeOnCancellation { cts.cancel() }
    }

    fun distanceToCompany(location: Location): Float {
        val companyLocation = Location("company").apply {
            latitude = GeofenceConfig.COMPANY_LAT
            longitude = GeofenceConfig.COMPANY_LNG
        }
        return location.distanceTo(companyLocation)
    }

    fun isMockLocation(location: Location): Boolean {
        return if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            location.isMock
        } else {
            @Suppress("DEPRECATION")
            location.isFromMockProvider
        }
    }

    suspend fun checkGeofence(): GeofenceResult {
        val result = getCurrentLocation()
        return result.fold(
            onSuccess = { location ->
                if (isMockLocation(location)) {
                    GeofenceResult.MockDetected
                } else {
                    val distance = distanceToCompany(location)
                    if (distance <= GeofenceConfig.ALLOWED_RADIUS) {
                        GeofenceResult.WithinRange(distance)
                    } else {
                        GeofenceResult.OutOfRange(distance)
                    }
                }
            },
            onFailure = { e ->
                GeofenceResult.Error(e.message ?: "定位失败")
            }
        )
    }
}
