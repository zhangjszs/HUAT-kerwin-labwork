# 要编译的内核模块对象
MODULE_NAME := Dev_frame
obj-m += $(MODULE_NAME).o

# 内核源代码目录
KDIR := /lib/modules/$(shell uname -r)/build
# 当前工作目录
PWD := $(shell pwd)

# 默认目标：编译内核模块
default:
	@echo "开始编译内核模块..."
	$(MAKE) -C $(KDIR) M=$(PWD) modules || { echo "编译内核模块失败！"; exit 1; }
	@echo "内核模块编译成功！"

# 清理目标：清理编译生成的文件
clean:
	@echo "开始清理编译生成的文件..."
	$(MAKE) -C $(KDIR) M=$(PWD) clean || { echo "清理编译生成的文件失败！"; exit 1; }
	@echo "编译生成的文件清理成功！"    