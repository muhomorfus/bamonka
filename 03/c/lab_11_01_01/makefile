# Компилятор
COMPILER := gcc

# Текущая директория
PWD := .

# Стандарт языка
STANDARD := c99

# Флаги компиляции
COMPILER_FLAGS := -std=$(STANDARD) -Wall -Wpedantic -Wextra -Werror

# Отладочные флаги
DEBUG_FLAGS := -g3 --coverage

# Флаги unit-тестов (check). Иногда добавить -lm -lsubunit
CHECK_FLAGS := -lcheck -lpthread -lrt

# Релизные флаги
RELEASE_FLAGS := -g0

# Флаги компоновки
LINKER_FLAGS :=

# Путь к заголовочникам
INCLUDE_PATH := $(PWD)/inc

# Путь к файлам исходного кода
SRC_PATH := $(PWD)/src

# Путь к выходной папке
OUT_PATH := $(PWD)/out

# Путь к папке с unit-тестами
UNIT_TESTS_PATH := $(PWD)/unit_tests

# Файлы исходного кода
SOURCES := $(wildcard $(SRC_PATH)/*.c)

# Файлы с зависимостями
DEPENDENCIES := $(subst $(SRC_PATH),$(OUT_PATH),$(SOURCES:.c=.d))

# Объектные файлы
OBJECTS := $(DEPENDENCIES:.d=.o)

# Объектные файлы для unit-тестов
OBJECTS_FOR_UNIT_TESTS := $(filter-out $(OUT_PATH)/main.o,$(OBJECTS))

# Файлы unit-тестов
UNIT_TESTS := $(wildcard $(UNIT_TESTS_PATH)/check_*.c)

# Файлы с зависимостями unit-тестов
UNIT_TESTS_DEPENDENCIES := $(subst $(UNIT_TESTS_PATH),$(OUT_PATH),$(UNIT_TESTS:.c=.d))

# Объектные файлы unit-тестов
UNIT_TESTS_OBJECTS := $(UNIT_TESTS_DEPENDENCIES:.d=.o)

# Название исполняемого файла для модульных тестов
UNIT_TESTS_EXECUTABLE_FILE := $(PWD)/unit_tests.exe

# Модульное тестирование
.PHONY: unit
.NOTPARALLELS: unit
unit: COMPILER_FLAGS += $(DEBUG_FLAGS)
unit: LINKER_FLAGS += $(DEBUG_FLAGS)
unit: $(UNIT_TESTS_EXECUTABLE_FILE)
	$(UNIT_TESTS_EXECUTABLE_FILE)
	gcov -f -r $(GCNO_FILES_FOR_UNIT_TESTS)
	mv $(PWD)/*.gcov $(OUT_PATH)/

# Очистка
.PHONY: clean
.NOTPARALLELS: clean
clean:
	rm -rf $(OUT_PATH)
	rm -f $(PWD)/*.exe

# Генерация зависимостей
$(OUT_PATH)/check_%.d: $(UNIT_TESTS_PATH)/check_%.c
	[ -f $(OUT_PATH) ] || mkdir -p $(OUT_PATH)
	$(COMPILER) -MM -I $(INCLUDE_PATH) $< > $@

$(OUT_PATH)/%.d: $(SRC_PATH)/%.c
	[ -f $(OUT_PATH) ] || mkdir -p $(OUT_PATH)
	$(COMPILER) -MM -I $(INCLUDE_PATH) $< > $@

# Сборка объектных файлов
$(OUT_PATH)/check_%.o: include $(UNIT_TESTS_DEPENDENCIES)
$(OUT_PATH)/check_%.o: $(UNIT_TESTS_PATH)/check_%.c
	[ -f $(OUT_PATH) ] || mkdir -p $(OUT_PATH)
	$(COMPILER) $(COMPILER_FLAGS) -I $(INCLUDE_PATH) -I /opt/homebrew/Cellar/check/0.15.2/include -c $< -o $@

$(OUT_PATH)/%.o: include $(DEPENDENCIES)
$(OUT_PATH)/%.o: $(SRC_PATH)/%.c
	[ -f $(OUT_PATH) ] || mkdir -p $(OUT_PATH)
	$(COMPILER) $(COMPILER_FLAGS) -I $(INCLUDE_PATH) -c $< -o $@

# Сборка unit_tests.exe
$(UNIT_TESTS_EXECUTABLE_FILE): $(UNIT_TESTS_OBJECTS) $(OBJECTS_FOR_UNIT_TESTS)
	$(COMPILER) $(LINKER_FLAGS) $^ -o $@ $(CHECK_FLAGS)


