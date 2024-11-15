# 定义目标对象文件列表
OBJ=main.o add_int.o add_float.o sub_int.o sub_float.o

# 定义编译器
CC=gcc

# 定义编译选项
CFLAGS=-Wall -g

# 默认目标：生成可执行文件 main
main: $(OBJ)
	# 使用 gcc 编译链接所有目标对象文件，生成可执行文件 main
	$(CC) -o $@ $^

# 清理目标：删除所有编译生成的中间文件和可执行文件
clean:
	# 删除所有 .o 文件和 main 可执行文件
	rm -f *.o main

# 自动生成依赖关系
%.o: %.c
	# 编译源文件生成对象文件
	$(CC) $(CFLAGS) -c $< -o $@

# 包含自动生成的依赖文件（可选）
-include $(OBJ:.o=.d)

# 自动生成依赖文件的规则（可选）
%.d: %.c
	@set -e; $(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$