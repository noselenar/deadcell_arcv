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
    ss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    return ss.str();
}

void backupFile(const fs::path& srcFile, const fs::path& backupDir) {
    if (fs::exists(srcFile)) {
        std::string timestamp = getCurrentTimestamp();
        fs::path backupFile = backupDir / (srcFile.stem().string() + "_" + timestamp + srcFile.extension().string());

        try {
            fs::copy(srcFile, backupFile, fs::copy_options::overwrite_existing);
            std::cout << "File updated, backup successful: " << backupFile.filename() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error during backup: " << e.what() << std::endl;
        }
    } else {
        std::cout << "File not found: " << srcFile << std::endl;
    }
}

void monitorFileUpdates(const fs::path& folder, const std::string& filename, const fs::path& backupDir) {
    fs::path srcFile = folder / filename;
    auto lastWriteTime = fs::last_write_time(srcFile);

    while (true) {
        if (fs::exists(srcFile)) {
            auto currentWriteTime = fs::last_write_time(srcFile);

            // 如果文件的修改时间发生变化，则备份文件
            if (currentWriteTime != lastWriteTime) {
                lastWriteTime = currentWriteTime;
                backupFile(srcFile, backupDir);
            }
        } else {
            std::cout << "File not found: " << srcFile << std::endl;
        }

        // 每隔 2 秒检查文件状态
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

int main(int argc, char* argv[]) {

    fs::path absolutePath = argv[1];
    fs::path savePath = argv[2];
    fs::path folder = absolutePath.parent_path();  // 目标文件夹路径
    std::string filename = absolutePath.filename().string();  // 文件名
    fs::path backupDir = savePath / "update";

    if (!fs::exists(backupDir)) {
        fs::create_directory(backupDir);
    }

    monitorFileUpdates(folder, filename, backupDir);

    return 0;
}
