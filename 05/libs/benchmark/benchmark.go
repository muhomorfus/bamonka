//go:build !windows

package benchmark

/*
#include <time.h>
#include <pthread.h>
#include <stdio.h>

long long get_cpu_time_ns() {
	struct timespec t;
	if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t)) {
		return -1;
	}

	return t.tv_sec * 1000000000LL + t.tv_nsec;
}
*/
import "C"
import "errors"

var (
	ErrCantGetTime = errors.New("cant get cpu time")
)

// getCPUTime is Go's adapter for clock_gettime() function.
func getCPUTime() int64 {
	return int64(C.get_cpu_time_ns())
}

// CPUTime gets current processor time in ns.
func CPUTime() (int64, error) {
	t := getCPUTime()
	if t == -1 {
		return 0, ErrCantGetTime
	}

	return t, nil
}

// CheckTime checks average time of executing f for n attempts.
func CheckTime(f func(), n int) (int64, error) {
	start, err := CPUTime()
	if err != nil {
		return 0, err
	}

	for i := 0; i < n; i++ {
		f()
	}

	end, err := CPUTime()
	if err != nil {
		return 0, err
	}

	return (end - start) / int64(n), nil
}
