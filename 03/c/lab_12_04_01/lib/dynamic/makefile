# Компилятор
COMPILER := gcc

# Текущая директория
PWD := .

# Стандарт языка
STANDARD := c99

# Флаги компиляции
COMPILER_FLAGS := -std=$(STANDARD) -fPIC -Wall -Wpedantic -Wextra -Werror

# Отладочные флаги
DEBUG_FLAGS := -g3

# Флаги unit-тестов (check). Иногда добавить -lm -lsubunit
CHECK_FLAGS := -lcheck -lpthread -lrt -lm -lsubunit

# Релизные флаги
RELEASE_FLAGS := -g0

# Флаги компоновки
LINKER_FLAGS := -lm -L . -larray

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

# Файлы gcno
GCNO_FILES := $(OBJECTS:.o=.gcno)

# Файлы gcno для unit-тестов
GCNO_FILES_FOR_UNIT_TESTS := $(OBJECTS_FOR_UNIT_TESTS:.o=.gcno)

# Расширение для флажка сборки
BUILD_STATE_EXT := buildstate

# Флажок релизной сборки
RELEASE_BUILD_FILE := $(PWD)/.release.$(BUILD_STATE_EXT)

# Флажок отладочной сборки
DEBUG_BUILD_FILE := $(PWD)/.debug.$(BUILD_STATE_EXT)

# Флажок сборки модульных тестов
UNIT_BUILD_FILE := $(PWD)/.unit.$(BUILD_STATE_EXT)

# Название исполняемого файла
LIBRARY_FILE := $(PWD)/libarray.so

# Название исполняемого файла для модульных тестов
UNIT_TESTS_EXECUTABLE_FILE := $(PWD)/unit_tests.exe

# Релизная сборка
.PHONY: release
release: COMPILER_FLAGS += $(RELEASE_FLAGS)
release: $(RELEASE_BUILD_FILE) | $(LIBRARY_FILE)

# Очистка
.PHONY: clean
.NOTPARALLELS: clean
clean:
	rm -f $(PWD)/.*.$(BUILD_STATE_EXT)
	rm -rf $(OUT_PATH)
	rm -f $(PWD)/*.exe
	rm -f $(PWD)/*.so

# Отладочная сборка
.PHONY: debug
debug: COMPILER_FLAGS += $(DEBUG_FLAGS)
debug: $(DEBUG_BUILD_FILE) | $(LIBRARY_FILE)

# Модульное тестирование
.PHONY: unit
.NOTPARALLELS: unit
unit: $(UNIT_BUILD_FILE) | $(UNIT_TESTS_EXECUTABLE_FILE)
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:.; $(UNIT_TESTS_EXECUTABLE_FILE)

# Генерация зависимостей
$(OUT_PATH)/check_%.d: $(UNIT_TESTS_PATH)/check_%.c | $(OUT_PATH)
	$(COMPILER) -MM -I $(INCLUDE_PATH) $< > $@

$(OUT_PATH)/%.d: $(SRC_PATH)/%.c $(OUT_PATH)
	$(COMPILER) -MM -I $(INCLUDE_PATH) $< > $@

# Сборка объектных файлов
$(OUT_PATH)/check_%.o: include $(UNIT_TESTS_DEPENDENCIES)
$(OUT_PATH)/check_%.o: $(UNIT_TESTS_PATH)/check_%.c | $(OUT_PATH)
	$(COMPILER) $(COMPILER_FLAGS) -I $(INCLUDE_PATH) -c $< -o $@

$(OUT_PATH)/%.o: include $(DEPENDENCIES)
$(OUT_PATH)/%.o: $(SRC_PATH)/%.c | $(OUT_PATH)
	$(COMPILER) $(COMPILER_FLAGS) -I $(INCLUDE_PATH) -c $< -o $@

# Сборка библиотеки
$(LIBRARY_FILE): $(OBJECTS)
	$(COMPILER) $^ -o $@ -shared

# Сборка unit_tests.exe
$(UNIT_TESTS_EXECUTABLE_FILE): COMPILER_FLAGS += $(DEBUG_FLAGS)
$(UNIT_TESTS_EXECUTABLE_FILE): LINKER_FLAGS += $(CHECK_FLAGS)
$(UNIT_TESTS_EXECUTABLE_FILE): LINKER_FLAGS += $(DEBUG_FLAGS)
$(UNIT_TESTS_EXECUTABLE_FILE): $(UNIT_TESTS_OBJECTS) $(LIBRARY_FILE)
	$(COMPILER) $(UNIT_TESTS_OBJECTS) -o $@ $(LINKER_FLAGS)

# Создание папки out
.PHONY: $(OUT_PATH)
$(OUT_PATH):
	[ -d $(OUT_PATH) ] || mkdir -p $(OUT_PATH)

# Создание флажка о текущем варианте сборки
$(PWD)/.%.$(BUILD_STATE_EXT):
	rm -f $(PWD)/.*.$(BUILD_STATE_EXT)
	touch $@
	rm -rf $(OUT_PATH)/*
	rm -f *.exe
	rm -f *.so
