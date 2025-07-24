# 537Shell

一个跨平台的类Linux终端程序，支持中英文双语，使用传统C++风格编写。无Windows.h依赖，纯标准库实现。

## 🌟 特性

- **跨平台支持**: Windows、Linux、macOS完全兼容
- **双语界面**: 支持中英文无缝切换
- **丰富命令集**: 30+个Linux风格命令
- **零外部依赖**: 仅使用C++17标准库
- **轻量级**: 单文件实现，易于编译和分发
- **完整功能**: 文本处理、文件管理、系统信息等

## 🚀 支持的命令

### 文件和目录操作
- `ls` / `dir` / `列表` - 列出目录内容（支持 -a, -l, -h 参数）
- `cd` / `进入` - 切换目录
- `pwd` / `路径` - 显示当前工作目录
- `mkdir` / `创建目录` - 创建目录
- `rmdir` / `删除目录` - 删除目录
- `rm` / `del` / `删除` - 删除文件
- `cp` / `copy` / `复制` - 复制文件
- `mv` / `move` / `移动` - 移动/重命名文件
- `touch` / `创建` - 创建空文件
- `tree` / `树形` - 显示目录树结构

### 文件内容处理
- `cat` / `type` / `查看` - 显示文件内容
- `echo` / `输出` - 输出文本
- `grep` / `搜索` - 在文件中搜索文本模式
- `wc` / `统计` - 统计文件行数、字数、字符数
- `sort` / `排序` - 排序文件内容
- `uniq` / `去重` - 去除重复行
- `head` / `头部` - 显示文件前N行
- `tail` / `尾部` - 显示文件后N行
- `cut` / `切割` - 提取文件指定列
- `sed` / `替换` - 流编辑器，文本替换
- `tr` / `转换` - 字符转换工具
- `diff` / `比较` - 比较两个文件的差异

### 系统和工具命令
- `find` / `查找` - 查找文件和目录
- `which` / `位置` - 查找命令位置
- `ps` / `进程` - 显示进程信息
- `date` / `时间` - 显示当前日期和时间
- `du` / `磁盘使用` - 显示目录磁盘使用情况
- `df` / `磁盘空间` - 显示磁盘空间使用情况
- `env` / `环境` - 显示环境变量
- `set` / `设置` - 设置环境变量
- `export` / `导出` - 导出环境变量

### 工具命令
- `clear` / `cls` / `清屏` - 清除屏幕
- `history` / `历史` - 显示命令历史
- `help` / `?` / `帮助` - 显示帮助信息
- `lang` / `语言` - 切换语言模式
- `exit` / `quit` / `退出` - 退出程序

## 🛠️ 编译要求

### 系统要求
- **操作系统**: Windows 7+, Linux (任意发行版), macOS 10.12+
- **编译器**: 支持C++17标准的编译器
  - GCC 7.0+
  - Clang 5.0+
  - Visual Studio 2017+
  - MinGW-w64
- **依赖**: 无外部依赖，仅使用C++17标准库

### 编译命令

#### Linux/macOS
```bash
g++ -std=c++17 -o 537shell 537shell.cpp
```

#### Windows (MinGW)  
```bash
g++ -std=c++17 -o 537shell.exe 537shell.cpp
```

#### Windows (Visual Studio)
```cmd
cl /EHsc /std:c++17 537shell.cpp
```

#### macOS (Clang)
```bash
clang++ -std=c++17 -o 537shell 537shell.cpp
```

## 🎯 使用方法

### 基本使用
```bash
# 编译后运行
./537shell        # Linux/macOS
537shell.exe      # Windows

# 查看帮助
help

# 切换到中文模式
lang zh

# 文件操作示例
ls -l
mkdir test
cd test
touch hello.txt
echo "Hello World" > hello.txt
cat hello.txt
```

### 双语支持示例
```bash
# 英文模式
537Shell:Documents$ help
537Shell:Documents$ ls -l
537Shell:Documents$ find test.txt

# 切换到中文模式
537Shell:Documents$ lang zh
已切换到中文模式

# 中文模式
537Shell:Documents$ 帮助
537Shell:Documents$ 列表 -l  
537Shell:Documents$ 查找 test.txt

# 切换回英文模式
537Shell:Documents$ lang en
Switched to English mode
```

### 文本处理示例
```bash
# 统计文件信息
wc file.txt

# 排序文件内容
sort data.txt

# 查找重复行
sort data.txt | uniq

# 搜索文本
grep "pattern" file.txt

# 文本替换
sed 's/old/new/' file.txt

# 比较文件
diff file1.txt file2.txt
```

## 🎨 设计理念

### 编程风格
- **函数式设计**: 避免使用类，采用纯函数实现
- **代码简洁**: 结构清晰，易于理解和维护
- **标准库优先**: 仅使用C++17标准库，无外部依赖

### 用户体验
- **双语支持**: 中文用户友好界面
- **跨平台**: 一次编写，到处运行
- **轻量级**: 单文件部署，快速启动

## 📁 项目结构

```
537Shell/
├── 537shell.cpp          # 主程序源代码
├── README.md             # 项目说明文档
└── LICENSE               # MIT许可证
    
```

## 🔧 平台特性

### Windows
- 兼容Windows 7及以上版本
- 支持MinGW和Visual Studio编译
- 自动适配Windows路径分隔符
- 完美支持中文显示

### Linux
- 支持所有主流Linux发行版
- 使用标准POSIX接口
- 完整的终端颜色支持
- 原生UTF-8编码

### macOS
- 支持macOS 10.12及以上版本
- 使用Clang编译器优化
- 完美支持Retina显示
- 原生终端集成

## 📈 路线图

- [x] 基础文件操作命令
- [x] 文本处理工具集
- [x] 双语支持
- [x] 跨平台兼容性
- [ ] 管道操作支持 (`|`)
- [ ] 重定向操作 (`>`, `>>`, `<`)
- [ ] 通配符匹配 (`*`, `?`)
- [ ] 脚本执行功能
- [ ] 颜色输出支持
- [ ] Tab自动补全
- [ ] 配置文件支持
- [ ] 插件系统

## 🤝 贡献指南

我们欢迎各种形式的贡献！

### 如何贡献
1. Fork 本项目
2. 创建功能分支 
3. 提交更改 
4. 推送到分支
5. 创建 Pull Request

### 贡献类型
- 🐛 Bug修复
- ✨ 新功能开发
- 📝 文档改进
- 🌐 国际化翻译
- 🔧 性能优化
- 🎨 界面改进

## 📊 兼容性矩阵

| 平台 | 编译器 | 状态 |
|------|-------|------|
| Windows 10/11 | MinGW-w64 | ✅ |
| Windows 10/11 | Visual Studio | ✅ |
| Ubuntu | GCC | ✅ | 
| CentOS/RHEL | GCC | ✅ |
| Debian | GCC | ✅ | 
| macOS | Clang | ✅ |

## 📄 许可证

本项目采用MIT许可证 - 查看 [LICENSE](LICENSE) 文件了解详情
