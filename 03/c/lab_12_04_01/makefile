# Компилятор
COMPILER := gcc

# Текущая директория
PWD := .

# Стандарт языка
STANDARD := c99

# Флаги компиляции
COMPILER_FLAGS := -std=$(STANDARD) -Wall -Wextra -Werror

# Отладочные флаги
DEBUG_FLAGS := -g3

# Релизные флаги
RELEASE_FLAGS := -g0

# Флаги компоновки
LINKER_FLAGS := -lm

# Путь к заголовочникам
INCLUDE_PATH := $(PWD)/inc

# Путь к файлам исходного кода
SRC_PATH := $(PWD)/src

# Путь к выходной папке
OUT_PATH := $(PWD)/out

# Файлы исходного кода
SOURCES := $(wildcard $(SRC_PATH)/*.c)

# Файлы с зависимостями
DEPENDENCIES := $(subst $(SRC_PATH),$(OUT_PATH),$(SOURCES:.c=.d))

# Объектные файлы
OBJECTS := $(DEPENDENCIES:.d=.o)

# Файлы gcno
GCNO_FILES := $(OBJECTS:.o=.gcno)

# Расширение для флажка сборки
BUILD_STATE_EXT := buildstate

# main-файл для компоновки
MAIN_LINK := main

# main-файл для загрузки
MAIN_LOAD := main_load

# Текущий main-файл
MAIN := $(MAIN_LINK)

# Флажки релизной сборки
RELEASE_STATIC_LINK_BUILD_FILE := $(PWD)/.release_static_link.$(BUILD_STATE_EXT)
RELEASE_DYNAMIC_LINK_BUILD_FILE := $(PWD)/.release_dynamic_link.$(BUILD_STATE_EXT)
RELEASE_DYNAMIC_LOAD_BUILD_FILE := $(PWD)/.release_dynamic_load.$(BUILD_STATE_EXT)

# Флажки отладочной сборки
DEBUG_STATIC_LINK_BUILD_FILE := $(PWD)/.debug_static_link.$(BUILD_STATE_EXT)
DEBUG_DYNAMIC_LINK_BUILD_FILE := $(PWD)/.debug_dynamic_link.$(BUILD_STATE_EXT)
DEBUG_DYNAMIC_LOAD_BUILD_FILE := $(PWD)/.debug_dynamic_load.$(BUILD_STATE_EXT)

# Тип сборки
DEBUG_BUILD := debug
RELEASE_BUILD := release

# Текущий тип сборки
BUILD_TYPE := $(DEBUG_BUILD)

# Библиотеки
LIBS := $(PWD)/lib

# Названия папок библиотек
LIB_STATIC := static
LIB_DYNAMIC := dynamic

# Названия папок библиотек
LIB_STATIC_FILE := libarray.a
LIB_DYNAMIC_FILE := libarray.so

# Папка с библиотекой
LIB := $(LIB_STATIC)

# Файл библиотеки
LIB_FILE := $(LIB_STATIC_FILE)

# Название исполняемого файла
EXECUTABLE_FILE := $(PWD)/app.exe

# Функциональное тестирование
.PHONY: func
.NOTPARALLELS: func
func:
	@[ -f $(EXECUTABLE_FILE) ] || (echo "You must create app.exe file"; exit 1)
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:.; ./func_tests/all_test.sh

# Сборки
.PHONY: static/link
static/link: MAIN = $(MAIN_LINK)
static/link: LIB = $(LIB_STATIC)
static/link: LINKER_FLAGS += $(LIB_STATIC_FILE)
static/link: INCLUDE_PATH = $(PWD)/$(LIBS)/$(LIB)/inc
static/link: $(OUT_PATH)/$(MAIN_LINK).o $(LIB_STATIC_FILE) | $(EXECUTABLE_FILE)

.PHONY: dynamic/link
dynamic/link: MAIN = $(MAIN_LINK)
dynamic/link: LIB = $(LIB_DYNAMIC)
dynamic/link: LINKER_FLAGS += -L . -larray
dynamic/link: INCLUDE_PATH = $(PWD)/$(LIBS)/$(LIB)/inc
dynamic/link: $(OUT_PATH)/$(MAIN_LINK).o $(LIB_DYNAMIC_FILE) | $(EXECUTABLE_FILE)

.PHONY: dynamic/load
dynamic/load: MAIN = $(MAIN_LOAD)
dynamic/load: LIB = $(LIB_DYNAMIC)
dynamic/load: LINKER_FLAGS += -ldl
dynamic/load: INCLUDE_PATH = $(PWD)/$(LIBS)/$(LIB)/inc
dynamic/load: $(OUT_PATH)/$(MAIN_LOAD).o $(LIB_DYNAMIC_FILE) | $(EXECUTABLE_FILE)

# Релизные сборки
.PHONY: release/%
release/%: COMPILER_FLAGS += $(RELEASE_FLAGS)
release/%: BUILD_TYPE = $(RELEASE_BUILD)

release/static/link: $(RELEASE_STATIC_LINK_BUILD_FILE) | static/link
release/dynamic/link: $(RELEASE_DYNAMIC_LINK_BUILD_FILE) | dynamic/link
release/dynamic/load: $(RELEASE_DYNAMIC_LOAD_BUILD_FILE) | dynamic/load

# Отладочные сборки
.PHONY: debug/%
debug/%: COMPILER_FLAGS += $(DEBUG_FLAGS)
debug/%: BUILD_TYPE = $(DEBUG_BUILD)

debug/static/link: $(DEBUG_STATIC_LINK_BUILD_FILE) | static/link
debug/dynamic/link: $(DEBUG_DYNAMIC_LINK_BUILD_FILE) | dynamic/link
debug/dynamic/load: $(DEBUG_DYNAMIC_LOAD_BUILD_FILE) | dynamic/load

# Очистка
.PHONY: clean
.NOTPARALLELS: clean
clean:
	rm -f $(PWD)/.*.$(BUILD_STATE_EXT)
	rm -rf $(OUT_PATH)
	rm -f $(PWD)/*.exe
	rm -f *.so
	rm -f *.a

.PHONY: clean-all
.NOTPARALLELS: clean-all
clean-all: clean
	cd $(LIBS)/$(LIB_STATIC) && make clean
	cd $(LIBS)/$(LIB_DYNAMIC) && make clean

# Отладочная сборка
.PHONY: debug
debug: COMPILER_FLAGS += $(DEBUG_FLAGS)
debug: LINKER_FLAGS += $(DEBUG_FLAGS)
debug: $(DEBUG_BUILD_FILE) | $(EXECUTABLE_FILE)

# Генерация зависимостей
$(OUT_PATH)/%.d: $(SRC_PATH)/%.c $(OUT_PATH)
	$(COMPILER) -MM -I $(INCLUDE_PATH) $< > $@

# Сборка объектных файлов
$(OUT_PATH)/%.o: include $(DEPENDENCIES)
$(OUT_PATH)/%.o: $(SRC_PATH)/%.c | $(OUT_PATH)
	$(COMPILER) $(COMPILER_FLAGS) -I $(INCLUDE_PATH) -c $< -o $@

# Сборка app.exe
$(EXECUTABLE_FILE):
	$(COMPILER) $(OUT_PATH)/$(MAIN).o -o $@ $(LINKER_FLAGS)

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
	rm -f *.a

# Файл статической библиотеки
$(LIB_STATIC_FILE):
	cd $(LIBS)/$(LIB_STATIC) && make $(BUILD_TYPE)
	cp $(LIBS)/$(LIB_STATIC)/$(LIB_STATIC_FILE) ./

# Файл динамической библиотеки
$(LIB_DYNAMIC_FILE):
	cd $(LIBS)/$(LIB_DYNAMIC) && make $(BUILD_TYPE)
	cp $(LIBS)/$(LIB_DYNAMIC)/$(LIB_DYNAMIC_FILE) ./
