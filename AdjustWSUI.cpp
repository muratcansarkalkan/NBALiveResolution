#include <iostream>
#include <filesystem>
#include <unordered_set>

namespace fs = std::filesystem;

namespace adjustWSUI {
    void adjustWidescreenUI(float value, const std::string& pathA, const std::string& pathB) {
        if (value > 1.333334) {
            // Check if files are already in path 'b'
            std::unordered_set<std::string> filesInB;

            for (const auto& entry : fs::directory_iterator(pathB)) {
                filesInB.insert(entry.path().filename().string());
            }

            // Copy files from path 'a' to path 'b' only if they don't exist in 'b'
            for (const auto& entry : fs::directory_iterator(pathA)) {
                const auto& filename = entry.path().filename().string();
                if (filesInB.find(filename) == filesInB.end()) {
                    fs::copy(entry.path(), fs::path(pathB) / entry.path().filename());
                }
            }

            std::cout << "Files copied from path 'a' to path 'b' if they didn't exist in 'b'.\n";
        }
        else {
            // Remove files from path 'b' that have corresponding files in path 'a'
            std::unordered_set<std::string> filesInA;

            for (const auto& entry : fs::directory_iterator(pathA)) {
                filesInA.insert(entry.path().filename().string());
            }

            for (const auto& entry : fs::directory_iterator(pathB)) {
                const auto& filename = entry.path().filename().string();
                if (filesInA.find(filename) != filesInA.end()) {
                    fs::remove(entry.path());
                    std::cout << "Removed file from path 'b': " << filename << '\n';
                }
            }

            std::cout << "Files removed from path 'b' that have corresponding files in path 'a'.\n";
        }
    }
}