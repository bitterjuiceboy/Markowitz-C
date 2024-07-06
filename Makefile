CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm
TARGET = main
SRC_DIR = .
BUILD_DIR = build

# 获取所有的源文件
SRCS := $(wildcard $(SRC_DIR)/*.c)
# 生成对应的目标文件列表
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
# 获取头文件列表
DEPS := $(wildcard $(SRC_DIR)/*.h)

.PHONY: all clean

all: $(BUILD_DIR)/$(TARGET)

# 生成目标文件和可执行文件
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# 生成每个源文件对应的目标文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@
	
# 创建build目录
$(OBJS): | $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

