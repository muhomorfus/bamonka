package app

import (
	"fmt"
	"time"
)

const (
	CacheTypeRedis    = "redis"
	CacheTypeInMemory = "in_memory"
	CacheTypeNop      = "nop"
)

const (
	CacheCoderJSON     = "json"
	CacheCoderCBOR     = "cbor"
	CacheCoderGzipCBOR = "gzip_cbor"
	CacheCoderGzipJSON = "gzip_json"
)

type Config struct {
	AdminPostgres Postgres
	HRPostgres    Postgres
	UserPostgres  Postgres
	Logger        Logger
	Port          int
	Cache         Cache
	Token         Token
	Initial       Initial
	Redis         Redis
	CacheType     string
	CacheCoder    string
}

type Postgres struct {
	Host           string
	Port           int
	User, Password string
	Database       string
}

type Logger struct {
	Level string
	Path  string
}

type Cache struct {
	TTL  time.Duration
	Size int
}

type Token struct {
	Expire time.Duration
	Secret string
}

type Initial struct {
	DepartmentName string
	Nickname       string
	Name           string
	Password       string
}

type Redis struct {
	Address  string
	Password string
	DB       int
}

func (p Postgres) DSN() string {
	return fmt.Sprintf("host=%s port=%d user=%s password=%s dbname=%s sslmode=disable", p.Host, p.Port, p.User, p.Password, p.Database)
}
