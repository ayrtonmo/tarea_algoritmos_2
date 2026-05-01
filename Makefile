GRUPO := 1
NOMBRE_TAREA := algorithm_analyzer

CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -Iinclude
LDFLAGS :=

SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
BUILD_DIR := build
DB_DIR := db
DOCS_DIR := docs
PLOTS_DIR := plots

TARGET := $(BUILD_DIR)/$(NOMBRE_TAREA).out

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all folders clean run plot send

all: folders $(TARGET)

folders:
	@mkdir -p $(SRC_DIR) $(INC_DIR) $(OBJ_DIR) $(BUILD_DIR) $(DB_DIR) $(DOCS_DIR) $(PLOTS_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | folders
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)/*

send: all
	tar -czvf $(NOMBRE_TAREA)_G$(GRUPO).tar.gz $(SRC_DIR) $(INC_DIR) Makefile

plot:
	@echo "Generando graficos..." && gnuplot ./plots/plot.gp
