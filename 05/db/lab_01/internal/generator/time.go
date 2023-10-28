package generator

import "time"

const (
	postgresDateFmt = "2006-01-02"
	postgresTimeFmt = "15:04:05"
)

type postgresDate time.Time

func (t postgresDate) MarshalCSV() (string, error) {
	return time.Time(t).UTC().Format(postgresDateFmt), nil
}

type postgresTs time.Time

func (t postgresTs) MarshalCSV() (string, error) {
	return time.Time(t).UTC().Format(postgresDateFmt + " " + postgresTimeFmt), nil
}
