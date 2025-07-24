# 537Shell

一个类似Linux的终端程序，使用C++编写。

## 🚀 特性
- **丰富的命令支持**: 包含20+个常用命令
- **轻量级设计**: 单文件实现，易于编译和部署
- **命令历史**: 支持命令历史记录功能

## 📦 支持的命令

### 文件和目录操作
- `ls` / `dir` - 列出目录内容（支持 -a, -l 参数）
- `cd <path>` - 切换目录
- `pwd` - 显示当前工作目录
- `mkdir <dir>` - 创建目录
- `rmdir <dir>` - 删除目录
- `rm <file>` / `del` - 删除文件
- `cp <src> <dst>` / `copy` - 复制文件
- `mv <src> <dst>` / `move` - 移动/重命名文件
- `touch <file>` - 创建空文件
- `tree [path]` - 显示目录树结构

### 文件内容操作
- `cat <file>` / `type` - 显示文件内容
- `echo <text>` - 输出文本
- `grep <pattern> <file>` - 在文件中搜索文本

### 系统和工具命令
- `find <name>` - 查找文件和目录
- `ps` - 显示进程信息
- `date` - 显示当前日期和时间
- `clear` / `cls` - 清屏
- `history` - 显示命令历史
- `help` / `?` - 显示帮助信息
- `exit` / `quit` - 退出程序

## 🛠️ 编译要求

- **编译器**: 支持C++17标准的编译器
  - MinGW-w64 (推荐)
  - Visual Studio 2017或更高版本
  - Clang++
- **操作系统**: Windows 7或更高版本
- **依赖库**: 无外部依赖，仅使用标准库

## 📖 编译方法

### 使用MinGW
```bash
g++ -std=c++17 -o 537shell.exe 537shell.cpp
使用Visual Studio
bash
cl /EHsc /std:c++17 537shell.cpp
使用Clang
bash
clang++ -std=c++17 -o 537shell.exe 537shell.cpp
🎯 使用方法
编译程序后运行 537shell.exe
输入 help 查看所有可用命令
使用类似Linux的命令语法进行操作
示例
bash
537Shell:C:\Users\Username$ ls -l
537Shell:C:\Users\Username$ cd Documents
537Shell:C:\Users\Username\Documents$ mkdir test_folder
537Shell:C:\Users\Username\Documents$ touch hello.txt
537Shell:C:\Users\Username\Documents$ echo "Hello World" > hello.txt
537Shell:C:\Users\Username\Documents$ cat hello.txt
537Shell:C:\Users\Username\Documents$ tree
🔧 项目结构
code
537Shell/
├── 537shell.cpp          # 主程序源代码
├── README.md             # 项目说明
└── LICENSE          # 许可证
📝 TODO
[ ] 添加更多文本处理命令 (sort, uniq, wc)
[ ] 实现简单的脚本执行功能
[ ] 添加颜色输出支持
[ ] 支持tab自动补全
🤝 贡献
欢迎提交Issue和Pull Request！
Fork本项目-创建功能分支-提交更改-推送到分支-创建Pull Request
📄 许可证
本项目采用MIT许可证 - 查看 LICENSE 文件了解详情
