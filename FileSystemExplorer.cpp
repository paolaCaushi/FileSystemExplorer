#include "FileSystemExplorer.h"
#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void FileSystemExplorer::start() {
    currentPath = fs::current_path().string();
    std::string command;
    while (true) {
        std::cout << currentPath << " > ";
        std::getline(std::cin, command);
        std::vector<std::string> tokens = split(command, ' ');

        if (tokens.empty()) continue;

        if (tokens[0] == "ls") {
            listDirectory(tokens.size() > 1 ? tokens[1] : ".");
        } else if (tokens[0] == "cd") {
            if (tokens.size() > 1) changeDirectory(tokens[1]);
            else std::cout << "Usage: cd <directory>\n";
        } else if (tokens[0] == "search") {
            if (tokens.size() > 1) searchFile(tokens[1]);
            else std::cout << "Usage: search <filename>\n";
        } else if (tokens[0] == "create") {
            if (tokens.size() > 1) createFile(tokens[1]);
            else std::cout << "Usage: create <filename>\n";
        } else if (tokens[0] == "delete") {
            if (tokens.size() > 1) deleteFile(tokens[1]);
            else std::cout << "Usage: delete <filename>\n";
        } else if (tokens[0] == "rename") {
            if (tokens.size() > 2) renameFile(tokens[1], tokens[2]);
            else std::cout << "Usage: rename <oldName> <newName>\n";
        } else if (tokens[0] == "move") {
            if (tokens.size() > 2) moveFile(tokens[1], tokens[2]);
            else std::cout << "Usage: move <source> <destination>\n";
        } else if (tokens[0] == "permissions") {
            if (tokens.size() > 1) displayPermissions(tokens[1]);
            else std::cout << "Usage: permissions <filename>\n";
        } else if (tokens[0] == "help") {
            displayHelp();
        } else if (tokens[0] == "exit") {
            break;
        } else {
            std::cout << "Unknown command. Type 'help' for a list of commands.\n";
        }
    }
}

void FileSystemExplorer::listDirectory(const std::string& path) {
    std::string absPath = getAbsolutePath(path);
    for (const auto& entry : fs::directory_iterator(absPath)) {
        std::cout << entry.path().filename().string() << "\n";
    }
}

void FileSystemExplorer::changeDirectory(const std::string& path) {
    std::string absPath = getAbsolutePath(path);
    if (fs::exists(absPath) && fs::is_directory(absPath)) {
        currentPath = absPath;
    } else {
        std::cout << "Directory does not exist: " << path << "\n";
    }
}

void FileSystemExplorer::searchFile(const std::string& filename) {
    for (const auto& entry : fs::recursive_directory_iterator(currentPath)) {
        if (entry.path().filename() == filename) {
            std::cout << "Found: " << entry.path().string() << "\n";
            return;
        }
    }
    std::cout << "File not found: " << filename << "\n";
}

void FileSystemExplorer::createFile(const std::string& filename) {
    std::ofstream ofs(getAbsolutePath(filename));
    if (ofs) {
        std::cout << "File created: " << filename << "\n";
    } else {
        std::cout << "Failed to create file: " << filename << "\n";
    }
}

void FileSystemExplorer::deleteFile(const std::string& filename) {
    std::string absPath = getAbsolutePath(filename);
    if (fs::exists(absPath)) {
        fs::remove(absPath);
        std::cout << "File deleted: " << filename << "\n";
    } else {
        std::cout << "File not found: " << filename << "\n";
    }
}

void FileSystemExplorer::renameFile(const std::string& oldName, const std::string& newName) {
    std::string oldPath = getAbsolutePath(oldName);
    std::string newPath = getAbsolutePath(newName);
    if (fs::exists(oldPath)) {
        fs::rename(oldPath, newPath);
        std::cout << "File renamed from " << oldName << " to " << newName << "\n";
    } else {
        std::cout << "File not found: " << oldName << "\n";
    }
}

void FileSystemExplorer::moveFile(const std::string& source, const std::string& destination) {
    std::string srcPath = getAbsolutePath(source);
    std::string destPath = getAbsolutePath(destination);
    if (fs::exists(srcPath)) {
        fs::rename(srcPath, destPath);
        std::cout << "File moved from " << source << " to " << destination << "\n";
    } else {
        std::cout << "File not found: " << source << "\n";
    }
}

void FileSystemExplorer::displayPermissions(const std::string& filename) {
    std::string filePath = getAbsolutePath(filename);
    if (fs::exists(filePath)) {
        fs::perms p = fs::status(filePath).permissions();
        std::cout << "Permissions for " << filename << ":\n";
        std::cout << ((p & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
                  << ((p & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
                  << ((p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
                  << ((p & fs::perms::group_read) != fs::perms::none ? "r" : "-")
                  << ((p & fs::perms::group_write) != fs::perms::none ? "w" : "-")
                  << ((p & fs::perms::group_exec) != fs::perms::none ? "x" : "-")
                  << ((p & fs::perms::others_read) != fs::perms::none ? "r" : "-")
                  << ((p & fs::perms::others_write) != fs::perms::none ? "w" : "-")
                  << ((p & fs::perms::others_exec) != fs::perms::none ? "x" : "-")
                  << "\n";
    } else {
        std::cout << "File not found: " << filename << "\n";
    }
}

void FileSystemExplorer::displayHelp() {
    std::cout << "Available commands:\n";
    std::cout << "  ls [directory]     - List files in the current or specified directory\n";
    std::cout << "  cd <directory>     - Change to specified directory\n";
    std::cout << "  search <file>      - Search for a file in the current directory and subdirectories\n";
    std::cout << "  create <file>      - Create a new file\n";
    std::cout << "  delete <file>      - Delete a file\n";
    std::cout << "  rename <old> <new> - Rename a file\n";
    std::cout << "  move <src> <dest>  - Move a file\n";
    std::cout << "  permissions <file> - Display file permissions\n";
    std::cout << "  help               - Display this help message\n";
    std::cout << "  exit               - Exit the application\n";
}

std::vector<std::string> FileSystemExplorer::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string FileSystemExplorer::getAbsolutePath(const std::string& relativePath) {
    return fs::absolute(fs::path(currentPath) / relativePath).string();
}
