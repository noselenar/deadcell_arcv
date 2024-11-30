#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

    fs::path absolutePath = argv[1];
    fs::path savePath = argv[2];
    fs::path backupDir = savePath / "update";

    fs::path folder = absolutePath.parent_path();  // 输入目标文件夹路径
    std::string filename = absolutePath.filename().string();  // 输入需要备份的文件名

    std::cout << "Folder Path: " << folder << "\n";
    std::cout << "File Name: " << filename << "\n";
    std::cout << "backup folder: " << backupDir << "\n";

    if (!fs::exists(backupDir)) {
        fs::create_directory(backupDir);
    }
    return 0;
}