package app

import (
	"context"
	"encoding/json"
	"errors"
	"flag"
	"fmt"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/coder"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/ports"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/repository/nopcacher"
	redisRepo "git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/repository/redis"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/roledb"
	"github.com/redis/go-redis/v9"
	"net/http"
	"os"
	"time"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/managers"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/openapi"
	inmem "git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/repository/gcache"
	repo "git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/repository/postgres"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/server"
	"github.com/bluele/gcache"
	"github.com/rs/cors"
	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"
	"golang.org/x/sync/errgroup"
	"gorm.io/driver/postgres"
	"gorm.io/gorm"
	"moul.io/zapgorm2"
)

var (
	ErrEmptyFilename = errors.New("empty filename")
)

type App struct {
	cfg     Config
	logger  *zap.SugaredLogger
	srv     *server.Server
	handler http.Handler

	employeeRepo   ports.EmployeeRepository
	teamRepo       ports.TeamRepository
	departmentRepo ports.DepartmentRepository

	searchMng ports.SearchManager
}

func New() *App {
	return &App{}
}

func (a *App) Init() error {
	if err := a.readConfig(); err != nil {
		return fmt.Errorf("read config: %w", err)
	}

	if err := a.initLogger(); err != nil {
		return fmt.Errorf("init logger: %w", err)
	}

	roleDB := roledb.New()

	db, err := gorm.Open(postgres.Open(a.cfg.UserPostgres.DSN()), &gorm.Config{
		Logger: zapgorm2.New(a.logger.Desugar()),
	})
	if err != nil {
		a.logger.Fatalw("cant open user db conn", "error", err)

		return fmt.Errorf("open gorm db: %w", err)
	}
	roleDB.Append(models.User, db)

	db, err = gorm.Open(postgres.Open(a.cfg.HRPostgres.DSN()), &gorm.Config{
		Logger: zapgorm2.New(a.logger.Desugar()),
	})
	if err != nil {
		a.logger.Fatalw("cant open hr db conn", "error", err)

		return fmt.Errorf("open gorm db: %w", err)
	}
	roleDB.Append(models.HR, db)

	db, err = gorm.Open(postgres.Open(a.cfg.AdminPostgres.DSN()), &gorm.Config{
		Logger: zapgorm2.New(a.logger.Desugar()),
	})
	if err != nil {
		a.logger.Fatalw("cant open admin db conn", "error", err)

		return fmt.Errorf("open gorm db: %w", err)
	}
	roleDB.Append(models.Admin, db)

	a.departmentRepo = repo.NewDepartmentRepository(roleDB)
	a.employeeRepo = repo.NewEmployeeRepository(roleDB)
	subscriptionRepo := repo.NewSubscriptionRepository(roleDB)
	a.teamRepo = repo.NewTeamRepository(roleDB)
	vacationRepo := repo.NewVacationRepository(roleDB, a.employeeRepo)

	departmentMng := managers.NewDepartment(a.departmentRepo)
	teamMng := managers.NewTeam(a.teamRepo)
	a.searchMng = managers.NewSearch(a.initSearchCache(), a.teamRepo, a.departmentRepo, a.employeeRepo, a.logger)
	profileMng := managers.NewProfile(a.employeeRepo, a.departmentRepo, subscriptionRepo, vacationRepo, a.cfg.Token.Expire, a.cfg.Token.Secret)

	cfgMng := managers.NewConfig(departmentMng, profileMng, a.cfg.Initial.Name, a.cfg.Initial.Nickname, a.cfg.Initial.Password, a.cfg.Initial.DepartmentName)
	if err := cfgMng.Init(context.Background()); err != nil {
		return fmt.Errorf("init base params: %w", err)
	}

	a.srv = server.NewServer(departmentMng, teamMng, a.searchMng, profileMng)

	a.handler = openapi.NewRouter(openapi.NewDefaultApiController(a.srv))
	a.handler = server.Middleware(profileMng, a.handler)
	a.handler = cors.AllowAll().Handler(a.handler)

	return nil
}

func (a *App) Run(ctx context.Context) error {
	a.logger.Infow("starting application")

	var eg errgroup.Group

	eg.Go(func() error {
		return http.ListenAndServe(fmt.Sprintf(":%d", a.cfg.Port), a.handler)
	})

	return eg.Wait()
}

func (a *App) initSearchCache() ports.SearchCache {
	switch a.cfg.CacheType {
	case CacheTypeNop:
		return nopcacher.NewSearch()
	case CacheTypeInMemory:
		return inmem.NewSearch(
			gcache.New(a.cfg.Cache.Size).
				LFU().
				Expiration(a.cfg.Cache.TTL).Build(),
		)
	case CacheTypeRedis:
		rdb := redis.NewClient(&redis.Options{
			Addr:     a.cfg.Redis.Address,
			Password: a.cfg.Redis.Password,
			DB:       a.cfg.Redis.DB,
		})

		return redisRepo.NewSearch(rdb, a.initCoder(), a.cfg.Cache.TTL)
	default:
		return nil
	}
}

func (a *App) initCoder() ports.Coder {
	switch a.cfg.CacheCoder {
	case CacheCoderJSON:
		return coder.JSON
	case CacheCoderCBOR:
		return coder.CBOR
	case CacheCoderGzipJSON:
		return coder.GzipJSON
	case CacheCoderGzipCBOR:
		return coder.GzipCBOR
	default:
		return nil
	}
}

func (a *App) readConfig() error {
	cfgFilename := flag.String("config", "configs/config.json", "path of config file")
	flag.Parse()

	if cfgFilename == nil {
		return fmt.Errorf("get config file: %w", ErrEmptyFilename)
	}

	cfgFile, err := os.Open(*cfgFilename)
	if err != nil {
		return fmt.Errorf("open config file: %w", err)
	}

	if err := json.NewDecoder(cfgFile).Decode(&a.cfg); err != nil {
		return fmt.Errorf("read config: %w", err)
	}

	a.cfg.Token.Expire *= time.Second
	a.cfg.Cache.TTL *= time.Second

	return nil
}

func (a *App) initLogger() error {
	level, err := zap.ParseAtomicLevel(a.cfg.Logger.Level)
	if err != nil {
		return fmt.Errorf("parse level: %w", err)
	}

	cfg := zap.Config{
		Level:    level,
		Encoding: "json",
		EncoderConfig: zapcore.EncoderConfig{
			MessageKey:    "message",
			LevelKey:      "level",
			TimeKey:       "time",
			NameKey:       "name",
			CallerKey:     "caller",
			StacktraceKey: "stacktrace",
			EncodeTime:    zapcore.RFC3339TimeEncoder,
			EncodeCaller:  zapcore.FullCallerEncoder,
			EncodeLevel:   zapcore.CapitalLevelEncoder,
		},
		OutputPaths: []string{a.cfg.Logger.Path},
	}

	logger, err := cfg.Build()
	if err != nil {
		return fmt.Errorf("build config: %w", err)
	}

	a.logger = logger.Sugar()

	return nil
}
