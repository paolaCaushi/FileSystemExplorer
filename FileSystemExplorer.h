#ifndef FILE_SYSTEM_EXPLORER_H
#define FILE_SYSTEM_EXPLORER_H

#include <string>
#include <vector>

class FileSystemExplorer {
public:
    void start();

private:
    void listDirectory(const std::string& path);
    void changeDirectory(const std::string& path);
    void searchFile(const std::string& filename);
    void createFile(const std::string& filename);
    void deleteFile(const std::string& filename);
    void renameFile(const std::string& oldName, const std::string& newName);
    void moveFile(const std::string& source, const std::string& destination);
    void displayPermissions(const std::string& filename);
    void displayHelp();
    std::string currentPath;

    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string getAbsolutePath(const std::string& relativePath);
};

#endif // FILE_SYSTEM_EXPLORER_H
