#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <windows.h>
#include <direct.h>
#include <io.h>

using namespace std;
namespace fs = std::filesystem;

// 全局变量
string current_path;
vector<string> command_history;

// 辅助函数声明
vector<string> split_command(const string& cmd);
void print_prompt();
void execute_command(const vector<string>& args);
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

// 工具函数
string get_current_time() {
    time_t now = time(0);
    char* time_str = ctime(&now);
    string result(time_str);
    result.pop_back(); // 移除换行符
    return result;
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

        for (size_t i = 0; i < entries.size(); ++i) {
            bool last = (i == entries.size() - 1);
            cout << prefix << (last ? "└── " : "├── ") << entries[i].path().filename().string() << endl;
            
            if (entries[i].is_directory()) {
                string new_prefix = prefix + (last ? "    " : "│   ");
                print_tree(entries[i].path().string(), new_prefix, last);
            }
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

// 分割命令行参数
vector<string> split_command(const string& cmd) {
    vector<string> tokens;
    istringstream iss(cmd);
    string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

// 打印提示符
void print_prompt() {
    cout << "537Shell:" << current_path << "$ ";
}

// 主命令执行函数
void execute_command(const vector<string>& args) {
    if (args.empty()) return;
    
    string cmd = args[0];
    
    if (cmd == "help" || cmd == "?") {
        cmd_help();
    } else if (cmd == "ls" || cmd == "dir") {
        cmd_ls(args);
    } else if (cmd == "cd") {
        cmd_cd(args);
    } else if (cmd == "pwd") {
        cmd_pwd();
    } else if (cmd == "mkdir") {
        cmd_mkdir(args);
    } else if (cmd == "rmdir") {
        cmd_rmdir(args);
    } else if (cmd == "rm" || cmd == "del") {
        cmd_rm(args);
    } else if (cmd == "cp" || cmd == "copy") {
        cmd_cp(args);
    } else if (cmd == "mv" || cmd == "move") {
        cmd_mv(args);
    } else if (cmd == "cat" || cmd == "type") {
        cmd_cat(args);
    } else if (cmd == "echo") {
        cmd_echo(args);
    } else if (cmd == "find") {
        cmd_find(args);
    } else if (cmd == "grep") {
        cmd_grep(args);
    } else if (cmd == "ps") {
        cmd_ps();
    } else if (cmd == "date") {
        cmd_date();
    } else if (cmd == "clear" || cmd == "cls") {
        cmd_clear();
    } else if (cmd == "history") {
        cmd_history();
    } else if (cmd == "touch") {
        cmd_touch(args);
    } else if (cmd == "tree") {
        cmd_tree(args);
    } else if (cmd == "exit" || cmd == "quit") {
        cout << "Goodbye!" << endl;
        exit(0);
    } else {
        cout << "537Shell: command not found: " << cmd << endl;
        cout << "Type 'help' for available commands." << endl;
    }
}

// 命令实现
void cmd_help() {
    cout << "537Shell - Available Commands:" << endl;
    cout << "  help, ?          - Show this help message" << endl;
    cout << "  ls, dir          - List directory contents" << endl;
    cout << "  cd <path>        - Change directory" << endl;
    cout << "  pwd              - Print working directory" << endl;
    cout << "  mkdir <dir>      - Create directory" << endl;
    cout << "  rmdir <dir>      - Remove directory" << endl;
    cout << "  rm <file>        - Remove file" << endl;
    cout << "  cp <src> <dst>   - Copy file" << endl;
    cout << "  mv <src> <dst>   - Move/rename file" << endl;
    cout << "  cat <file>       - Display file contents" << endl;
    cout << "  echo <text>      - Display text" << endl;
    cout << "  find <name>      - Find files/directories" << endl;
    cout << "  grep <pattern>   - Search text in files" << endl;
    cout << "  ps               - List running processes" << endl;
    cout << "  date             - Show current date/time" << endl;
    cout << "  clear, cls       - Clear screen" << endl;
    cout << "  history          - Show command history" << endl;
    cout << "  touch <file>     - Create empty file" << endl;
    cout << "  tree             - Show directory tree" << endl;
    cout << "  exit, quit       - Exit shell" << endl;
}

void cmd_ls(const vector<string>& args) {
    string path = current_path;
    bool show_all = false;
    bool long_format = false;
    
    // 解析参数
    for (size_t i = 1; i < args.size(); ++i) {
        if (args[i] == "-a") {
            show_all = true;
        } else if (args[i] == "-l") {
            long_format = true;
        } else if (args[i][0] != '-') {
            path = args[i];
        }
    }
    
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            string filename = entry.path().filename().string();
            
            if (!show_all && filename[0] == '.') continue;
            
            if (long_format) {
                cout << (entry.is_directory() ? "d" : "-");
                cout << "rwxrwxrwx ";
                cout << "1 user user ";
                
                if (entry.is_regular_file()) {
                    cout << entry.file_size() << " ";
                } else {
                    cout << "0 ";
                }
                
                auto time = fs::last_write_time(entry);
                cout << "Jan 01 12:00 ";
            }
            
            cout << filename;
            if (entry.is_directory()) cout << "/";
            cout << endl;
        }
    } catch (const exception& e) {
        cout << "ls: " << e.what() << endl;
    }
}

void cmd_cd(const vector<string>& args) {
    string new_path;
    
    if (args.size() < 2) {
        // Windows下回到用户主目录
        new_path = getenv("USERPROFILE");
    } else {
        new_path = args[1];
    }
    
    if (_chdir(new_path.c_str()) == 0) {
        current_path = fs::current_path().string();
    } else {
        cout << "cd: no such file or directory: " << new_path << endl;
    }
}

void cmd_pwd() {
    cout << current_path << endl;
}

void cmd_mkdir(const vector<string>& args) {
    if (args.size() < 2) {
        cout << "mkdir: missing operand" << endl;
        return;
    }
    
    for (size_t i = 1; i < args.size(); ++i) {
        try {
            fs::create_directory(args[i]);
            cout << "Directory created: " << args[i] << endl;
        } catch (const exception& e) {
            cout << "mkdir: " << e.what() << endl;
        }
    }
}

void cmd_rmdir(const vector<string>& args) {
    if (args.size() < 2) {
        cout << "rmdir: missing operand" << endl;
        return;
    }
    
    for (size_t i = 1; i < args.size(); ++i) {
        try {
            fs::remove_all(args[i]);
            cout << "Directory removed: " << args[i] << endl;
        } catch (const exception& e) {
            cout << "rmdir: " << e.what() << endl;
        }
    }
}

void cmd_rm(const vector<string>& args) {
    if (args.size() < 2) {
        cout << "rm: missing operand" << endl;
        return;
    }
    
    for (size_t i = 1; i < args.size(); ++i) {
        try {
            fs::remove(args[i]);
            cout << "File removed: " << args[i] << endl;
        } catch (const exception& e) {
            cout << "rm: " << e.what() << endl;
        }
    }
}

void cmd_cp(const vector<string>& args) {
    if (args.size() < 3) {
        cout << "cp: missing operand" << endl;
        cout << "usage: cp <source> <destination>" << endl;
        return;
    }
    
    try {
        fs::copy_file(args[1], args[2]);
        cout << "File copied: " << args[1] << " -> " << args[2] << endl;
    } catch (const exception& e) {
        cout << "cp: " << e.what() << endl;
    }
}

void cmd_mv(const vector<string>& args) {
    if (args.size() < 3) {
        cout << "mv: missing operand" << endl;
        cout << "usage: mv <source> <destination>" << endl;
        return;
    }
    
    try {
        fs::rename(args[1], args[2]);
        cout << "File moved: " << args[1] << " -> " << args[2] << endl;
    } catch (const exception& e) {
        cout << "mv: " << e.what() << endl;
    }
}

void cmd_cat(const vector<string>& args) {
    if (args.size() < 2) {
        cout << "cat: missing operand" << endl;
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
            cout << "cat: " << args[i] << ": No such file or directory" << endl;
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
    string search_path = current_path;
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
        cout << "grep: usage: grep <pattern> <file>" << endl;
        return;
    }
    
    string pattern = args[1];
    string filename = args[2];
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "grep: " << filename << ": No such file or directory" << endl;
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
    cout << "PID  PPID CMD" << endl;
    cout << "1    0    537Shell" << endl;
    cout << GetCurrentProcessId() << "   1    537Shell" << endl;
}

void cmd_date() {
    cout << get_current_time() << endl;
}

void cmd_clear() {
    system("cls");
}

void cmd_history() {
    for (size_t i = 0; i < command_history.size(); ++i) {
        cout << i + 1 << "  " << command_history[i] << endl;
    }
}

void cmd_touch(const vector<string>& args) {
    if (args.size() < 2) {
        cout << "touch: missing operand" << endl;
        return;
    }
    
    for (size_t i = 1; i < args.size(); ++i) {
        ofstream file(args[i]);
        if (file.is_open()) {
            file.close();
            cout << "File created: " << args[i] << endl;
        } else {
            cout << "touch: cannot create " << args[i] << endl;
        }
    }
}

void cmd_tree(const vector<string>& args) {
    string path = current_path;
    if (args.size() > 1) {
        path = args[1];
    }
    
    cout << path << endl;
    print_tree(path);
}

// 主函数
int main() {
    cout << "537Shell For Windows v1.0" << endl;
    cout << "Type 'help' for available commands." << endl;
    cout << endl;
    
    // 初始化当前路径
    current_path = fs::current_path().string();
    
    string input;
    while (true) {
        print_prompt();
        getline(cin, input);
        
        if (!input.empty()) {
            command_history.push_back(input);
            vector<string> args = split_command(input);
            execute_command(args);
        }
    }
    
    return 0;
}
