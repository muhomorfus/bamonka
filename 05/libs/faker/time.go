package faker

import (
	"time"
)

const (
	postgresDateFmt = "2006-01-02"
	postgresTimeFmt = "15:04:05"
)

// TimeLimit is the type for time limits.
type TimeLimit struct {
	from time.Time
	to   time.Time
}

// NewTimeLimitYear creates new time limit between two years.
func NewTimeLimitYear(from, to int) TimeLimit {
	return TimeLimit{
		from: time.Date(from, time.January, 1, 0, 0, 0, 0, time.UTC),
		to:   time.Date(to, time.January, 1, 0, 0, 0, 0, time.UTC),
	}
}

// NewTimeLimitHour creates new time limit between two hours in current day.
func NewTimeLimitHour(from, to int) TimeLimit {
	date := time.Now()
	return TimeLimit{
		from: time.Date(date.Year(), date.Month(), date.Day(), from, 0, 0, 0, time.UTC),
		to:   time.Date(date.Year(), date.Month(), date.Day(), to, 0, 0, 0, time.UTC),
	}
}

// NewTimeLimitTime creates new time limit between two times.
func NewTimeLimitTime(from, to time.Time) TimeLimit {
	return TimeLimit{
		from: from,
		to:   to,
	}
}

// Time generates random time with limits [from; to).
func Time(l TimeLimit) time.Time {
	return time.Unix(Int64WithLimits(l.from.Unix(), l.to.Unix()), 0).UTC()
}

// PostgresDate generates date formatted with Postgres format.
func PostgresDate(l TimeLimit) string {
	return Time(l).Format(postgresDateFmt)
}

// PostgresYear year.
func PostgresYear(l TimeLimit) int {
	return Time(l).Year()
}

// PostgresTime generates time formatted with Postgres format.
func PostgresTime(l TimeLimit) string {
	return Time(l).Format(postgresTimeFmt)
}

// PostgresTimeRounded generates time formatted with Postgres format with given round.
func PostgresTimeRounded(l TimeLimit, d time.Duration) string {
	return Time(l).Round(d).Format(postgresTimeFmt)
}
