#include "headers/components/general.h"

/* Swaping two variables a and b of any type with size. */
void swap(void *a, void *b, unsigned long size) {
    void *temp = malloc(size);
    if (!temp)
        return;
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    free(temp);
}
/* Returns the given value in radians. */
const float radians(const float value) {
    return value * (3.14159 / 180.0);
}
#ifdef VECTORIZED_CODE // #######################################################################################
/* Find how much in each direction the vectors array extends. Populate with values the (min) and (max) vec4 values */
void getvec4ArrayLimits(vec4 v[], const int array_len, vec4* min, vec4* max) {
    *min = v[0];
    *max = v[0];
    for (int i = 0; i < array_len; i++) {
        *min = _mm_min_ps(*min, v[i]);
        *max = _mm_max_ps(*max, v[i]);
    }
}
#else // ITERATIVE_CODE #########################################################################################
/* Find how much in each direction the vectors array extends. Populate with values the (min) and (max) vec4 values */
void getvec4ArrayLimits(vec4 v[], const int array_len, vec4 *min, vec4 *max) {
    *min = v[0];
    *max = v[0];
    for (int i = 0; i < array_len; i++) {
        if (min->m128_f32[0] > v[i].m128_f32[0])
            min->m128_f32[0] = v[i].m128_f32[0];

        if (max->m128_f32[0] < v[i].m128_f32[0])
            max->m128_f32[0] = v[i].m128_f32[0];

        if (min->m128_f32[1] > v[i].m128_f32[1])
            min->m128_f32[1] = v[i].m128_f32[1];

        if (max->m128_f32[1] < v[i].m128_f32[1])
            max->m128_f32[1] = v[i].m128_f32[1];

        if (min->m128_f32[2] > v[i].m128_f32[2])
            min->m128_f32[2] = v[i].m128_f32[2];

        if (max->m128_f32[2] < v[i].m128_f32[2])
            max->m128_f32[2] = v[i].m128_f32[2];
    }
}
#endif // VECTORIZED_CODE #######################################################################################
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) // ################
const static uint64_t epoch = ((uint64_t)116444736000000000ULL);
void usleep(const int usec) {
    HANDLE timer = 0;
    LARGE_INTEGER ft;

    ft.QuadPart = -(10 * usec); // Convert to 100 nanoseconds interval. Negative value indicates relative time.

    timer = CreateWaitableTimerA(NULL, TRUE, NULL);
    if (!timer)
        return;
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}
/* Initializes the given timeval struct, to the time of the day, at the moment, at which this function was called. */
void gettimeofday(struct timeval *tp, struct timezone *tzp) {
    FILETIME file_time;
    SYSTEMTIME system_time;
    ULARGE_INTEGER ularge;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    ularge.LowPart = file_time.dwLowDateTime;
    ularge.HighPart = file_time.dwHighDateTime;

    tp->tv_sec = (uint64_t)((ularge.QuadPart - epoch) / 10000000L);
    tp->tv_usec = (uint64_t)(system_time.wMilliseconds * 1000);
}
#endif // !WIN32 ###########################################################
/* Initializes the global timeval struct, to the time of the day, at the moment, at which this function was called. */
void initTimeCounter(metrics* mtr) {
    gettimeofday(&mtr->tv0, NULL);
}
/* Counts how much time has passed since the first time that we casted gettimeofday(). */
void updateTimeCounter(metrics* mtr) {
    mtr->LastFrameTimeCounter = mtr->TimeCounter;
    gettimeofday(&mtr->tv, NULL);
    mtr->TimeCounter = (float)(mtr->tv.tv_sec - mtr->tv0.tv_sec) + 0.000001 * ((float)(mtr->tv.tv_usec - mtr->tv0.tv_usec));
    mtr->deltaTime = mtr->TimeCounter - mtr->LastFrameTimeCounter;
}
void calculateFPS(metrics* mtr) {
    mtr->Frame++;
    if ((mtr->Frame % 60) == 0)
        mtr->FPS = 1.f / mtr->deltaTime;
    mtr->prevTime = mtr->TimeCounter;
}


