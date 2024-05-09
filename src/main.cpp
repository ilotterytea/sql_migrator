#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "hi!" << std::endl;

  std::string folder_path = "migrations";

  bool directory_exists = std::filesystem::exists(folder_path);
  bool is_directory = std::filesystem::is_directory(folder_path);

  if (directory_exists && !is_directory) {
    std::cout << "*** The given path \"" << folder_path
              << "\" isn't a directory!\n";
    return 1;
  }

  if (!directory_exists) {
    std::cout << "Creating the \"" << folder_path << "\" directory...\n";
    std::filesystem::create_directories(folder_path);
  }

  return 0;
}
