#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

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

  // validating directories
  std::vector<std::string> valid_directory_paths;
  for (const auto &entry : std::filesystem::directory_iterator(folder_path)) {
    std::string path = entry.path();

    bool up_exists = std::filesystem::exists(path + "/up.sql");
    bool down_exists = std::filesystem::exists(path + "/down.sql");

    if (up_exists && down_exists) {
      valid_directory_paths.push_back(path);
    } else {
      std::cout << "Ignoring the \"" << path
                << "\" folder because it doesn't have either up.sql, or "
                   "down.sql...\n";
    }
  }

  return 0;
}
