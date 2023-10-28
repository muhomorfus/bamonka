MIGRATIONS_DIR := deployments/migrations
POSTGRES := postgres://user:password@127.0.0.1:5437/balteca?sslmode=disable

.PHONY: migrate
migrate:
	@goose --dir=$(MIGRATIONS_DIR) postgres "$(POSTGRES)" up

.PHONY: migrate-down
migrate-down:
	@goose --dir=$(MIGRATIONS_DIR) postgres "$(POSTGRES)" down

MIGRATION := default

.PHONY: new-migration
new-migration:
	goose --dir="$(MIGRATIONS_DIR)" create $(MIGRATION) sql

.PHONY: compose-up
compose-up:
	docker-compose --project-directory deployments up -d

.PHONY: .generate-swagger
.generate-swagger:
	docker run --rm \
      -v ${PWD}:/local openapitools/openapi-generator-cli generate \
      -i /local/api/swagger.yml \
      -g go-server \
      -o /local/internal/openapi/ \
      --additional-properties=outputAsLibrary=true,sourceFolder=.

.PHONY: goimports
goimports:
	goimports -w .

.PHONY: generate-swagger
generate-swagger: .generate-swagger goimports

.PHONY: generate-mocks
generate-mocks:
	mkdir -p ./internal/mocks
	minimock -i ./internal/ports.DepartmentRepository -o ./internal/mocks/ -s _mock.go
	minimock -i ./internal/ports.EmployeeRepository -o ./internal/mocks/ -s _mock.go
	minimock -i ./internal/ports.SubscriptionRepository -o ./internal/mocks/ -s _mock.go
	minimock -i ./internal/ports.TeamRepository -o ./internal/mocks/ -s _mock.go
	minimock -i ./internal/ports.VacationRepository -o ./internal/mocks/ -s _mock.go

.PHONY: test
test:
	go test ./...

.PHONY: plot
plot:
	gnuplot -persist scripts/1.gpi
	gnuplot -persist scripts/1_caches.gpi
	gnuplot -persist scripts/2.gpi
	gnuplot -persist scripts/2_memory.gpi
