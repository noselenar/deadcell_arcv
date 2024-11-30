#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&in_time_t);
    
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");  // 格式化为 "YYYY-MM-DD_HH-MM-SS"
    return ss.str();
}

void backupFile(const fs::path& srcFile, const fs::path& backupDir) {
    if (fs::exists(srcFile)) {
        std::string timestamp = getCurrentTimestamp();
        fs::path backupFile = backupDir / (srcFile.stem().string() + "_" + timestamp + srcFile.extension().string());

        try {
            fs::copy(srcFile, backupFile, fs::copy_options::overwrite_existing);
            std::cout << "Backup successful: " << backupFile.filename() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error during backup: " << e.what() << std::endl;
        }
    } else {
        std::cout << "File not found: " << srcFile << std::endl;
    }
}

void backupEveryMinute(const fs::path& folder, const std::string& filename, const fs::path& backupDir, int interval) {
    while (true) {
        fs::path srcFile = folder / filename;

        // Call backup function
        backupFile(srcFile, backupDir);

        // Wait for interval second(s)
        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
}

int main(int argc, char* argv[]) {

    fs::path absolutePath = argv[1];
    fs::path savePath = argv[2];
    std::string times = argv[3];

    fs::path folder = absolutePath.parent_path();  // 输入目标文件夹路径
    std::string filename = absolutePath.filename().string();  // 输入需要备份的文件名
    fs::path backupDir = savePath / "freq";  // 输入备份目录路径

    int interval = std::stoi(times);
    // 确保备份目录存在
    if (!fs::exists(backupDir)) {
        fs::create_directory(backupDir);
    }

    // 启动每分钟备份一次的程序
    backupEveryMinute(folder, filename, backupDir, interval);

    return 0;
}
