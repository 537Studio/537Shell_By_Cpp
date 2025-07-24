#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <map>
#include <set>
#include <regex>
#include <locale>
#include <codecvt>
#include <iomanip>

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

using namespace std;
namespace fs = std::filesystem;

// 全局变量
string current_path;
vector<string> command_history;
map<string, string> environment_vars;
bool chinese_mode = false;

// 辅助函数声明
vector<string> split_command(const string& cmd);
void print_prompt();
void execute_command(const vector<string>& args);
void init_locale();
string to_utf8(const string& str);

// 命令函数声明
void cmd_help();
void cmd_ls(const vector<string>& args);
void cmd_cd(const vector<string>& args);
void cmd_pwd();
void cmd_mkdir(const vector<string>& args);
void cmd_rmdir(const vector<string>& args);
void cmd_rm(const vector<string>& args);
void cmd_cp(const vector<string>& args);
void cmd_mv(const vector<string>& args);
void cmd_cat(const vector<string>& args);
void cmd_echo(const vector<string>& args);
void cmd_find(const vector<string>& args);
void cmd_grep(const vector<string>& args);
void cmd_ps();
void cmd_date();
void cmd_clear();
void cmd_history();
void cmd_touch(const vector<string>& args);
void cmd_tree(const vector<string>& args);
void cmd_wc(const vector<string>& args);
void cmd_sort(const vector<string>& args);
void cmd_uniq(const vector<string>& args);
void cmd_head(const vector<string>& args);
void cmd_tail(const vector<string>& args);
void cmd_cut(const vector<string>& args);
void cmd_sed(const vector<string>& args);
void cmd_tr(const vector<string>& args);
void cmd_which(const vector<string>& args);
void cmd_env();
void cmd_set(const vector<string>& args);
void cmd_export(const vector<string>& args);
void cmd_diff(const vector<string>& args);
void cmd_du(const vector<string>& args);
void cmd_df();
void cmd_lang(const vector<string>& args);

// 工具函数实现
void init_locale() {
    try {
        locale::global(locale(""));
        cout.imbue(locale(""));
        cin.imbue(locale(""));
    } catch (const exception& e) {
        // 如果设置locale失败，继续使用默认
    }
}

string get_current_time() {
    time_t now = time(0);
    char buffer[100];
    struct tm* timeinfo = localtime(&now);
    
    if (chinese_mode) {
        strftime(buffer, sizeof(buffer), "%Y年%m月%d日 %H:%M:%S", timeinfo);
    } else {
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    }
    
    return string(buffer);
}

void print_tree(const string& path, const string& prefix = "", bool is_last = true) {
    try {
        fs::path dir_path(path);
        if (!fs::exists(dir_path) || !fs::is_directory(dir_path)) {
            return;
        }

        vector<fs::directory_entry> entries;
        for (const auto& entry : fs::directory_iterator(dir_path)) {
            entries.push_back(entry);
        }

        sort(entries.begin(), entries.end(), [](const fs::directory_entry& a, const fs::directory_entry& b) {
            return a.path().filename().string() < b.path().filename().string();
        });

        for (size_t i = 0; i < entries.size(); ++i) {
            bool last = (i == entries.size() - 1);
            cout << prefix << (last ? "└── " : "├── ") << entries[i].path().filename().string() << endl;
            
            if (entries[i].is_directory()) {
                string new_prefix = prefix + (last ? "    " : "│   ");
                print_tree(entries[i].path().string(), new_prefix, last);
            }
        }
    } catch (const exception& e) {
        cout << (chinese_mode ? "错误: " : "Error: ") << e.what() << endl;
    }
}

vector<string> split_command(const string& cmd) {
    vector<string> tokens;
    istringstream iss(cmd);
    string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

void print_prompt() {
    string prompt_symbol = "$ ";
    cout << "537Shell:" << fs::current_path().filename().string() << prompt_symbol;
}

void execute_command(const vector<string>& args) {
    if (args.empty()) return;
    
    string cmd = args[0];
    
    if (cmd == "help" || cmd == "?" || cmd == "帮助") {
        cmd_help();
    } else if (cmd == "ls" || cmd == "dir" || cmd == "列表") {
        cmd_ls(args);
    } else if (cmd == "cd" || cmd == "进入") {
        cmd_cd(args);
    } else if (cmd == "pwd" || cmd == "路径") {
        cmd_pwd();
    } else if (cmd == "mkdir" || cmd == "创建目录") {
        cmd_mkdir(args);
    } else if (cmd == "rmdir" || cmd == "删除目录") {
        cmd_rmdir(args);
    } else if (cmd == "rm" || cmd == "del" || cmd == "删除") {
        cmd_rm(args);
    } else if (cmd == "cp" || cmd == "copy" || cmd == "复制") {
        cmd_cp(args);
    } else if (cmd == "mv" || cmd == "move" || cmd == "移动") {
        cmd_mv(args);
    } else if (cmd == "cat" || cmd == "type" || cmd == "查看") {
        cmd_cat(args);
    } else if (cmd == "echo" || cmd == "输出") {
        cmd_echo(args);
    } else if (cmd == "find" || cmd == "查找") {
        cmd_find(args);
    } else if (cmd == "grep" || cmd == "搜索") {
        cmd_grep(args);
    } else if (cmd == "ps" || cmd == "进程") {
        cmd_ps();
    } else if (cmd == "date" || cmd == "时间") {
        cmd_date();
    } else if (cmd == "clear" || cmd == "cls" || cmd == "清屏") {
        cmd_clear();
    } else if (cmd == "history" || cmd == "历史") {
        cmd_history();
    } else if (cmd == "touch" || cmd == "创建") {
        cmd_touch(args);
    } else if (cmd == "tree" || cmd == "树形") {
        cmd_tree(args);
    } else if (cmd == "wc" || cmd == "统计") {
        cmd_wc(args);
    } else if (cmd == "sort" || cmd == "排序") {
        cmd_sort(args);
    } else if (cmd == "uniq" || cmd == "去重") {
        cmd_uniq(args);
    } else if (cmd == "head" || cmd == "头部") {
        cmd_head(args);
    } else if (cmd == "tail" || cmd == "尾部") {
        cmd_tail(args);
    } else if (cmd == "cut" || cmd == "切割") {
        cmd_cut(args);
    } else if (cmd == "sed" || cmd == "替换") {
        cmd_sed(args);
    } else if (cmd == "tr" || cmd == "转换") {
        cmd_tr(args);
    } else if (cmd == "which" || cmd == "位置") {
        cmd_which(args);
    } else if (cmd == "env" || cmd == "环境") {
        cmd_env();
    } else if (cmd == "set" || cmd == "设置") {
        cmd_set(args);
    } else if (cmd == "export" || cmd == "导出") {
        cmd_export(args);
    } else if (cmd == "diff" || cmd == "比较") {
        cmd_diff(args);
    } else if (cmd == "du" || cmd == "磁盘使用") {
        cmd_du(args);
    } else if (cmd == "df" || cmd == "磁盘空间") {
        cmd_df();
    } else if (cmd == "lang" || cmd == "语言") {
        cmd_lang(args);
    } else if (cmd == "exit" || cmd == "quit" || cmd == "退出") {
        cout << (chinese_mode ? "再见！" : "Goodbye!") << endl;
        exit(0);
    } else {
        if (chinese_mode) {
            cout << "537Shell: 未找到命令: " << cmd << endl;
            cout << "输入 '帮助' 查看可用命令。" << endl;
        } else {
            cout << "537Shell: command not found: " << cmd << endl;
            cout << "Type 'help' for available commands." << endl;
        }
    }
}

// 命令实现
void cmd_help() {
    if (chinese_mode) {
        cout << "537Shell - 可用命令:" << endl;
        cout << "  帮助, help           - 显示此帮助信息" << endl;
        cout << "  列表, ls             - 列出目录内容" << endl;
        cout << "  进入, cd <路径>      - 切换目录" << endl;
        cout << "  路径, pwd            - 显示当前目录" << endl;
        cout << "  创建目录, mkdir      - 创建目录" << endl;
        cout << "  删除目录, rmdir      - 删除目录" << endl;
        cout << "  删除, rm             - 删除文件" << endl;
        cout << "  复制, cp             - 复制文件" << endl;
        cout << "  移动, mv             - 移动/重命名文件" << endl;
        cout << "  查看, cat            - 显示文件内容" << endl;
        cout << "  输出, echo           - 显示文本" << endl;
        cout << "  查找, find           - 查找文件/目录" << endl;
        cout << "  搜索, grep           - 在文件中搜索文本" << endl;
        cout << "  进程, ps             - 列出运行进程" << endl;
        cout << "  时间, date           - 显示当前日期/时间" << endl;
        cout << "  清屏, clear          - 清除屏幕" << endl;
        cout << "  历史, history        - 显示命令历史" << endl;
        cout << "  创建, touch          - 创建空文件" << endl;
        cout << "  树形, tree           - 显示目录树" << endl;
        cout << "  统计, wc             - 统计文件行数/字数" << endl;
        cout << "  排序, sort           - 排序文件内容" << endl;
        cout << "  去重, uniq           - 去除重复行" << endl;
        cout << "  头部, head           - 显示文件前几行" << endl;
        cout << "  尾部, tail           - 显示文件后几行" << endl;
        cout << "  切割, cut            - 提取文件列" << endl;
        cout << "  替换, sed            - 文本替换" << endl;
        cout << "  转换, tr             - 字符转换" << endl;
        cout << "  比较, diff           - 比较文件差异" << endl;
        cout << "  磁盘使用, du         - 显示目录大小" << endl;
        cout << "  磁盘空间, df         - 显示磁盘空间" << endl;
        cout << "  环境, env            - 显示环境变量" << endl;
        cout << "  语言, lang           - 切换语言模式" << endl;
        cout << "  退出, exit           - 退出程序" << endl;
    } else {
        cout << "537Shell - Available Commands:" << endl;
        cout << "  help, ?              - Show this help message" << endl;
        cout << "  ls, dir              - List directory contents" << endl;
        cout << "  cd <path>            - Change directory" << endl;
        cout << "  pwd                  - Print working directory" << endl;
        cout << "  mkdir <dir>          - Create directory" << endl;
        cout << "  rmdir <dir>          - Remove directory" << endl;
        cout << "  rm <file>            - Remove file" << endl;
        cout << "  cp <src> <dst>       - Copy file" << endl;
        cout << "  mv <src> <dst>       - Move/rename file" << endl;
        cout << "  cat <file>           - Display file contents" << endl;
        cout << "  echo <text>          - Display text" << endl;
        cout << "  find <name>          - Find files/directories" << endl;
        cout << "  grep <pattern>       - Search text in files" << endl;
        cout << "  ps                   - List running processes" << endl;
        cout << "  date                 - Show current date/time" << endl;
        cout << "  clear, cls           - Clear screen" << endl;
        cout << "  history              - Show command history" << endl;
        cout << "  touch <file>         - Create empty file" << endl;
        cout << "  tree                 - Show directory tree" << endl;
        cout << "  wc <file>            - Word, line, character count" << endl;
        cout << "  sort <file>          - Sort lines in file" << endl;
        cout << "  uniq <file>          - Remove duplicate lines" << endl;
        cout << "  head <file>          - Show first lines of file" << endl;
        cout << "  tail <file>          - Show last lines of file" << endl;
        cout << "  cut <options> <file> - Extract columns from file" << endl;
        cout << "  sed <pattern> <file> - Stream editor for text" << endl;
        cout << "  tr <set1> <set2>     - Translate characters" << endl;
        cout << "  diff <file1> <file2> - Compare files" << endl;
        cout << "  du [path]            - Show directory size" << endl;
        cout << "  df                   - Show disk space" << endl;
        cout << "  env                  - Show environment variables" << endl;
        cout << "  lang <en|zh>         - Switch language mode" << endl;
        cout << "  exit, quit           - Exit shell" << endl;
    }
}

void cmd_ls(const vector<string>& args) {
    string path = ".";
    bool show_all = false;
    bool long_format = false;
    bool show_size = false;
    
    for (size_t i = 1; i < args.size(); ++i) {
        if (args[i] == "-a") {
            show_all = true;
        } else if (args[i] == "-l") {
            long_format = true;
        } else if (args[i] == "-h") {
            show_size = true;
        } else if (args[i][0] != '-') {
            path = args[i];
        }
    }
    
    try {
        vector<fs::directory_entry> entries;
        for (const auto& entry : fs::directory_iterator(path)) {
            entries.push_back(entry);
        }
        
        sort(entries.begin(), entries.end(), [](const fs::directory_entry& a, const fs::directory_entry& b) {
            return a.path().filename().string() < b.path().filename().string();
        });
        
        for (const auto& entry : entries) {
            string filename = entry.path().filename().string();
            
            if (!show_all && filename[0] == '.') continue;
            
            if (long_format) {
                cout << (entry.is_directory() ? "d" : "-");
                cout << "rwxrwxrwx ";
                cout << "1 user user ";
                
                if (entry.is_regular_file()) {
                    auto size = entry.file_size();
                    if (show_size) {
                        if (size >= 1024 * 1024 * 1024) {
                            cout << fixed << setprecision(1) << (double)size / (1024 * 1024 * 1024) << "G ";
                        } else if (size >= 1024 * 1024) {
                            cout << fixed << setprecision(1) << (double)size / (1024 * 1024) << "M ";
                        } else if (size >= 1024) {
                            cout << fixed << setprecision(1) << (double)size / 1024 << "K ";
                        } else {
                            cout << size << " ";
                        }
                    } else {
                        cout << size << " ";
                    }
                } else {
                    cout << "0 ";
                }
                
                auto ftime = fs::last_write_time(entry);
                cout << (chinese_mode ? "1月01日 12:00 " : "Jan 01 12:00 ");
            }
            
            cout << filename;
            if (entry.is_directory()) cout << "/";
            cout << endl;
        }
    } catch (const exception& e) {
        cout << (chinese_mode ? "ls: " : "ls: ") << e.what() << endl;
    }
}

void cmd_cd(const vector<string>& args) {
    string new_path;
    
    if (args.size() < 2) {
        new_path = fs::current_path().root_path().string();
    } else {
        new_path = args[1];
    }
    
    try {
        fs::current_path(new_path);
        current_path = fs::current_path().string();
    } catch (const exception& e) {
        cout << (chinese_mode ? "cd: 没有此文件或目录: " : "cd: no such file or directory: ") << new_path << endl;
    }
}

void cmd_pwd() {
    cout << fs::current_path().string() << endl;
}

void cmd_mkdir(const vector<string>& args) {
    if (args.size() < 2) {
        cout << (chinese_mode ? "mkdir: 缺少操作数" : "mkdir: missing operand") << endl;
        return;
    }
    
    for (size_t i = 1; i < args.size(); ++i) {
        try {
            fs::create_directories(args[i]);
            cout << (chinese_mode ? "已创建目录: " : "Directory created: ") << args[i] << endl;
        } catch (const exception& e) {
            cout << "mkdir: " << e.what() << endl;
        }
    }
}

void cmd_rmdir(const vector<string>& args) {
    if (args.size() < 2) {
        cout << (chinese_mode ? "rmdir: 缺少操作数" : "rmdir: missing operand") << endl;
        return;
    }
    
    for (size_t i = 1; i < args.size(); ++i) {
        try {
            fs::remove_all(args[i]);
            cout << (chinese_mode ? "已删除目录: " : "Directory removed: ") << args[i] << endl;
        } catch (const exception& e) {
            cout << "rmdir: " << e.what() << endl;
        }
    }
}

void cmd_rm(const vector<string>& args) {
    if (args.size() < 2) {
        cout << (chinese_mode ? "rm: 缺少操作数" : "rm: missing operand") << endl;
        return;
    }
    
    for (size_t i = 1; i < args.size(); ++i) {
        try {
            fs::remove(args[i]);
            cout << (chinese_mode ? "已删除文件: " : "File removed: ") << args[i] << endl;
        } catch (const exception& e) {
            cout << "rm: " << e.what() << endl;
        }
    }
}

void cmd_cp(const vector<string>& args) {
    if (args.size() < 3) {
        cout << (chinese_mode ? "cp: 缺少操作数" : "cp: missing operand") << endl;
        cout << (chinese_mode ? "用法: cp <源文件> <目标文件>" : "usage: cp <source> <destination>") << endl;
        return;
    }
    
    try {
        fs::copy_file(args[1], args[2], fs::copy_options::overwrite_existing);
        cout << (chinese_mode ? "已复制文件: " : "File copied: ") << args[1] << " -> " << args[2] << endl;
    } catch (const exception& e) {
        cout << "cp: " << e.what() << endl;
    }
}

void cmd_mv(const vector<string>& args) {
    if (args.size() < 3) {
        cout << (chinese_mode ? "mv: 缺少操作数" : "mv: missing operand") << endl;
        cout << (chinese_mode ? "用法: mv <源文件> <目标文件>" : "usage: mv <source> <destination>") << endl;
        return;
    }
    
    try {
        fs::rename(args[1], args[2]);
        cout << (chinese_mode ? "已移动文件: " : "File moved: ") << args[1] << " -> " << args[2] << endl;
    } catch (const exception& e) {
        cout << "mv: " << e.what() << endl;
    }
}

void cmd_cat(const vector<string>& args) {
    if (args.size() < 2) {
        cout << (chinese_mode ? "cat: 缺少操作数" : "cat: missing operand") << endl;
        return;
    }
    
    for (size_t i = 1; i < args.size(); ++i) {
        ifstream file(args[i]);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                cout << line << endl;
            }
            file.close();
        } else {
            cout << (chinese_mode ? "cat: " : "cat: ") << args[i] << (chinese_mode ? ": 没有此文件或目录" : ": No such file or directory") << endl;
        }
    }
}

void cmd_echo(const vector<string>& args) {
    for (size_t i = 1; i < args.size(); ++i) {
        cout << args[i];
        if (i < args.size() - 1) cout << " ";
    }
    cout << endl;
}

void cmd_find(const vector<string>& args) {
    string search_path = ".";
    string pattern = "*";
    
    if (args.size() > 1) {
        pattern = args[1];
    }
    
    try {
        for (const auto& entry : fs::recursive_directory_iterator(search_path)) {
            string filename = entry.path().filename().string();
            if (pattern == "*" || filename.find(pattern) != string::npos) {
                cout << entry.path().string() << endl;
            }
        }
    } catch (const exception& e) {
        cout << "find: " << e.what() << endl;
    }
}

void cmd_grep(const vector<string>& args) {
    if (args.size() < 3) {
        cout << (chinese_mode ? "grep: 用法: grep <模式> <文件>" : "grep: usage: grep <pattern> <file>") << endl;
        return;
    }
    
    string pattern = args[1];
    string filename = args[2];
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << (chinese_mode ? "grep: " : "grep: ") << filename << (chinese_mode ? ": 没有此文件或目录" : ": No such file or directory") << endl;
        return;
    }
    
    string line;
    int line_number = 1;
    while (getline(file, line)) {
        if (line.find(pattern) != string::npos) {
            cout << line_number << ": " << line << endl;
        }
        line_number++;
    }
    file.close();
}

void cmd_ps() {
    if (chinese_mode) {
        cout << "PID  PPID 命令" << endl;
        cout << "1    0    537Shell" << endl;
    } else {
        cout << "PID  PPID CMD" << endl;
        cout << "1    0    537Shell" << endl;
    }
}

void cmd_date() {
    cout << get_current_time() << endl;
}

void cmd_clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void cmd_history() {
    for (size_t i = 0; i < command_history.size(); ++i) {
        cout << i + 1 << "  " << command_history[i] << endl;
    }
}

void cmd_touch(const vector<string>& args) {
    if (args.size() < 2) {
        cout << (chinese_mode ? "touch: 缺少操作数" : "touch: missing operand") << endl;
        return;
    }
    
    for (size_t i = 1; i < args.size(); ++i) {
        ofstream file(args[i]);
        if (file.is_open()) {
            file.close();
            cout << (chinese_mode ? "已创建文件: " : "File created: ") << args[i] << endl;
        } else {
            cout << (chinese_mode ? "touch: 无法创建 " : "touch: cannot create ") << args[i] << endl;
        }
    }
}

void cmd_tree(const vector<string>& args) {
    string path = ".";
    if (args.size() > 1) {
        path = args[1];
    }
    
    cout << path << endl;
    print_tree(path);
}

void cmd_wc(const vector<string>& args) {
    if (args.size() < 2) {
        cout << (chinese_mode ? "wc: 缺少操作数" : "wc: missing operand") << endl;
        return;
    }
    
    for (size_t i = 1; i < args.size(); ++i) {
        ifstream file(args[i]);
        if (!file.is_open()) {
            cout << (chinese_mode ? "wc: " : "wc: ") << args[i] << (chinese_mode ? ": 没有此文件或目录" : ": No such file or directory") << endl;
            continue;
        }
        
        int lines = 0, words = 0, chars = 0;
        string line;
        
        while (getline(file, line)) {
            lines++;
            chars += line.length() + 1; // +1 for newline
            
            istringstream iss(line);
            string word;
            while (iss >> word) {
                words++;
            }
        }
        
        cout << lines << " " << words << " " << chars << " " << args[i] << endl;
        file.close();
    }
}

void cmd_sort(const vector<string>& args) {
    if (args.size() < 2) {
        cout << (chinese_mode ? "sort: 缺少操作数" : "sort: missing operand") << endl;
        return;
    }
    
    ifstream file(args[1]);
    if (!file.is_open()) {
        cout << (chinese_mode ? "sort: " : "sort: ") << args[1] << (chinese_mode ? ": 没有此文件或目录" : ": No such file or directory") << endl;
        return;
    }
    
    vector<string> lines;
    string line;
    
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    
    sort(lines.begin(), lines.end());
    
    for (const auto& sorted_line : lines) {
        cout << sorted_line << endl;
    }
}

void cmd_uniq(const vector<string>& args) {
    if (args.size() < 2) {
        cout << (chinese_mode ? "uniq: 缺少操作数" : "uniq: missing operand") << endl;
        return;
    }
    
    ifstream file(args[1]);
    if (!file.is_open()) {
        cout << (chinese_mode ? "uniq: " : "uniq: ") << args[1] << (chinese_mode ? ": 没有此文件或目录" : ": No such file or directory") << endl;
        return;
    }
    
    string line, prev_line;
    bool first = true;
    
    while (getline(file, line)) {
        if (first || line != prev_line) {
            cout << line << endl;
            prev_line = line;
            first = false;
        }
    }
    file.close();
}

void cmd_head(const vector<string>& args) {
    if (args.size() < 2) {
        cout << (chinese_mode ? "head: 缺少操作数" : "head: missing operand") << endl;
        return;
    }
    
    int num_lines = 10;
    string filename;
    
    if (args.size() >= 4 && args[1] == "-n") {
        num_lines = stoi(args[2]);
        filename = args[3];
    } else {
        filename = args[1];
    }
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << (chinese_mode ? "head: " : "head: ") << filename << (chinese_mode ? ": 没有此文件或目录" : ": No such file or directory") << endl;
        return;
    }
    
    string line;
    int count = 0;
    
    while (getline(file, line) && count < num_lines) {
        cout << line << endl;
        count++;
    }
    file.close();
}

void cmd_tail(const vector<string>& args) {
    if (args.size() < 2) {
        cout << (chinese_mode ? "tail: 缺少操作数" : "tail: missing operand") << endl;
        return;
    }
    
    int num_lines = 10;
    string filename;
    
    if (args.size() >= 4 && args[1] == "-n") {
        num_lines = stoi(args[2]);
        filename = args[3];
    } else {
        filename = args[1];
    }
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << (chinese_mode ? "tail: " : "tail: ") << filename << (chinese_mode ? ": 没有此文件或目录" : ": No such file or directory") << endl;
        return;
    }
    
    vector<string> lines;
    string line;
    
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    
    int start = max(0, (int)lines.size() - num_lines);
    for (int i = start; i < (int)lines.size(); ++i) {
        cout << lines[i] << endl;
    }
}

void cmd_cut(const vector<string>& args) {
    if (args.size() < 4) {
        cout << (chinese_mode ? "cut: 用法: cut -f <字段> <文件>" : "cut: usage: cut -f <field> <file>") << endl;
        return;
    }
    
    if (args[1] != "-f") {
        cout << (chinese_mode ? "cut: 不支持的选项" : "cut: unsupported option") << endl;
        return;
    }
    
    int field = stoi(args[2]) - 1; // 转为0索引
    string filename = args[3];
    char delimiter = '\t';
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << (chinese_mode ? "cut: " : "cut: ") << filename << (chinese_mode ? ": 没有此文件或目录" : ": No such file or directory") << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string token;
        int current_field = 0;
        
        while (getline(iss, token, delimiter)) {
            if (current_field == field) {
                cout << token << endl;
                break;
            }
            current_field++;
        }
    }
    file.close();
}

void cmd_sed(const vector<string>& args) {
    if (args.size() < 3) {
        cout << (chinese_mode ? "sed: 用法: sed 's/old/new/' <文件>" : "sed: usage: sed 's/old/new/' <file>") << endl;
        return;
    }
    
    string pattern = args[1];
    string filename = args[2];
    
    // 简单的替换模式解析 s/old/new/
    if (pattern.length() < 5 || pattern[0] != 's' || pattern[1] != '/') {
        cout << (chinese_mode ? "sed: 无效的模式" : "sed: invalid pattern") << endl;
        return;
    }
    
    size_t first_slash = pattern.find('/', 2);
    size_t second_slash = pattern.find('/', first_slash + 1);
    
    if (first_slash == string::npos || second_slash == string::npos) {
        cout << (chinese_mode ? "sed: 无效的模式" : "sed: invalid pattern") << endl;
        return;
    }
    
    string old_str = pattern.substr(2, first_slash - 2);
    string new_str = pattern.substr(first_slash + 1, second_slash - first_slash - 1);
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << (chinese_mode ? "sed: " : "sed: ") << filename << (chinese_mode ? ": 没有此文件或目录" : ": No such file or directory") << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        size_t pos = line.find(old_str);
        if (pos != string::npos) {
            line.replace(pos, old_str.length(), new_str);
        }
        cout << line << endl;
    }
    file.close();
}

void cmd_tr(const vector<string>& args) {
    if (args.size() < 3) {
        cout << (chinese_mode ? "tr: 用法: tr <集合1> <集合2>" : "tr: usage: tr <set1> <set2>") << endl;
        return;
    }
    
    string set1 = args[1];
    string set2 = args[2];
    
    string line;
    while (getline(cin, line)) {
        for (char& c : line) {
            size_t pos = set1.find(c);
            if (pos != string::npos && pos < set2.length()) {
                c = set2[pos];
            }
        }
        cout << line << endl;
    }
}

void cmd_which(const vector<string>& args) {
    if (args.size() < 2) {
        cout << (chinese_mode ? "which: 缺少操作数" : "which: missing operand") << endl;
        return;
    }
    
    string command = args[1];
    
    // 简单实现：检查是否是内置命令
    vector<string> builtin_commands = {
        "help", "ls", "cd", "pwd", "mkdir", "rmdir", "rm", "cp", "mv",
        "cat", "echo", "find", "grep", "ps", "date", "clear", "history",
        "touch", "tree", "wc", "sort", "uniq", "head", "tail", "cut",
        "sed", "tr", "which", "env", "set", "export", "diff", "du", "df",
        "lang"
    };
    
    if (find(builtin_commands.begin(), builtin_commands.end(), command) != builtin_commands.end()) {
        cout << (chinese_mode ? "内置命令: " : "builtin: ") << command << endl;
    } else {
        cout << (chinese_mode ? "未找到命令: " : "command not found: ") << command << endl;
    }
}

void cmd_env() {
    for (const auto& pair : environment_vars) {
        cout << pair.first << "=" << pair.second << endl;
    }
}

void cmd_set(const vector<string>& args) {
    if (args.size() < 2) {
        cmd_env();
        return;
    }
    
    string var_assignment = args[1];
    size_t eq_pos = var_assignment.find('=');
    
    if (eq_pos != string::npos) {
        string var_name = var_assignment.substr(0, eq_pos);
        string var_value = var_assignment.substr(eq_pos + 1);
        environment_vars[var_name] = var_value;
        cout << (chinese_mode ? "已设置变量: " : "Variable set: ") << var_name << "=" << var_value << endl;
    } else {
        cout << (chinese_mode ? "set: 无效的格式" : "set: invalid format") << endl;
    }
}

void cmd_export(const vector<string>& args) {
    if (args.size() < 2) {
        cout << (chinese_mode ? "export: 缺少操作数" : "export: missing operand") << endl;
        return;
    }
    
    string var_assignment = args[1];
    size_t eq_pos = var_assignment.find('=');
    
    if (eq_pos != string::npos) {
        string var_name = var_assignment.substr(0, eq_pos);
        string var_value = var_assignment.substr(eq_pos + 1);
        environment_vars[var_name] = var_value;
        cout << (chinese_mode ? "已导出变量: " : "Variable exported: ") << var_name << "=" << var_value << endl;
    } else {
        cout << (chinese_mode ? "export: 无效的格式" : "export: invalid format") << endl;
    }
}

void cmd_diff(const vector<string>& args) {
    if (args.size() < 3) {
        cout << (chinese_mode ? "diff: 用法: diff <文件1> <文件2>" : "diff: usage: diff <file1> <file2>") << endl;
        return;
    }
    
    ifstream file1(args[1]);
    ifstream file2(args[2]);
    
    if (!file1.is_open()) {
        cout << (chinese_mode ? "diff: " : "diff: ") << args[1] << (chinese_mode ? ": 没有此文件或目录" : ": No such file or directory") << endl;
        return;
    }
    
    if (!file2.is_open()) {
        cout << (chinese_mode ? "diff: " : "diff: ") << args[2] << (chinese_mode ? ": 没有此文件或目录" : ": No such file or directory") << endl;
        return;
    }
    
    vector<string> lines1, lines2;
    string line;
    
    while (getline(file1, line)) {
        lines1.push_back(line);
    }
    
    while (getline(file2, line)) {
        lines2.push_back(line);
    }
    
    file1.close();
    file2.close();
    
    size_t max_lines = max(lines1.size(), lines2.size());
    bool found_diff = false;
    
    for (size_t i = 0; i < max_lines; ++i) {
        string line1 = (i < lines1.size()) ? lines1[i] : "";
        string line2 = (i < lines2.size()) ? lines2[i] : "";
        
        if (line1 != line2) {
            if (!found_diff) {
                cout << (chinese_mode ? "文件差异:" : "File differences:") << endl;
                found_diff = true;
            }
            cout << (chinese_mode ? "行 " : "Line ") << (i + 1) << ":" << endl;
            cout << "< " << line1 << endl;
            cout << "> " << line2 << endl;
            cout << "---" << endl;
        }
    }
    
    if (!found_diff) {
        cout << (chinese_mode ? "文件相同" : "Files are identical") << endl;
    }
}

void cmd_du(const vector<string>& args) {
    string path = ".";
    if (args.size() > 1) {
        path = args[1];
    }
    
    try {
        uintmax_t total_size = 0;
        map<string, uintmax_t> dir_sizes;
        
        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            if (entry.is_regular_file()) {
                uintmax_t size = entry.file_size();
                total_size += size;
                
                string dir = entry.path().parent_path().string();
                dir_sizes[dir] += size;
            }
        }
        
        for (const auto& pair : dir_sizes) {
            double size_kb = pair.second / 1024.0;
            cout << fixed << setprecision(1) << size_kb << "K\t" << pair.first << endl;
        }
        
        double total_kb = total_size / 1024.0;
        cout << fixed << setprecision(1) << total_kb << "K\t" << (chinese_mode ? "总计" : "total") << endl;
        
    } catch (const exception& e) {
        cout << "du: " << e.what() << endl;
    }
}

void cmd_df() {
    try {
        fs::space_info space = fs::space(".");
        
        double total_gb = space.capacity / (1024.0 * 1024.0 * 1024.0);
        double available_gb = space.available / (1024.0 * 1024.0 * 1024.0);
        double used_gb = (space.capacity - space.available) / (1024.0 * 1024.0 * 1024.0);
        double used_percent = (used_gb / total_gb) * 100.0;
        
        if (chinese_mode) {
            cout << "文件系统\t总计\t已用\t可用\t使用%\t挂载点" << endl;
            cout << "当前磁盘\t" << fixed << setprecision(1) << total_gb << "G\t"
                 << used_gb << "G\t" << available_gb << "G\t"
                 << setprecision(0) << used_percent << "%\t/" << endl;
        } else {
            cout << "Filesystem\tSize\tUsed\tAvail\tUse%\tMounted on" << endl;
            cout << "current\t" << fixed << setprecision(1) << total_gb << "G\t"
                 << used_gb << "G\t" << available_gb << "G\t"
                 << setprecision(0) << used_percent << "%\t/" << endl;
        }
        
    } catch (const exception& e) {
        cout << "df: " << e.what() << endl;
    }
}

void cmd_lang(const vector<string>& args) {
    if (args.size() < 2) {
        cout << (chinese_mode ? "当前语言模式: 中文" : "Current language mode: English") << endl;
        cout << (chinese_mode ? "用法: lang <en|zh|english|chinese|英文|中文>" : "Usage: lang <en|zh|english|chinese>") << endl;
        return;
    }
    
    string lang = args[1];
    if (lang == "zh" || lang == "中文" || lang == "chinese") {
        chinese_mode = true;
        cout << "已切换到中文模式" << endl;
    } else if (lang == "en" || lang == "english" || lang == "英文") {
        chinese_mode = false;
        cout << "Switched to English mode" << endl;
    } else {
        cout << (chinese_mode ? "不支持的语言: " : "Unsupported language: ") << lang << endl;
        cout << (chinese_mode ? "支持的选项: zh, 中文, chinese, en, english, 英文" : "Supported options: zh, chinese, en, english") << endl;
    }
}

// 主函数
int main() {
    init_locale();
    
    cout << "537Shell v2.0"<< endl;
    cout << (chinese_mode ? "输入 '帮助' 查看可用命令。" : "Type 'help' for available commands.") << endl;
    cout << (chinese_mode ? "输入 'lang zh' 切换到中文模式。" : "Type 'lang zh' to switch to Chinese mode.") << endl;
    cout << endl;
    
    // 初始化当前路径和环境变量
    current_path = fs::current_path().string();
    environment_vars["PATH"] = "/usr/local/bin:/usr/bin:/bin";
    environment_vars["HOME"] = fs::current_path().root_path().string();
    environment_vars["SHELL"] = "537Shell";
    environment_vars["USER"] = "user";
    
    string input;
    while (true) {
        print_prompt();
        getline(cin, input);
        
        if (!input.empty()) {
            command_history.push_back(input);
            // 保持历史记录在合理范围内
            if (command_history.size() > 1000) {
                command_history.erase(command_history.begin());
            }
            
            vector<string> args = split_command(input);
            execute_command(args);
        }
    }
    
    return 0;
}
