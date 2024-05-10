#include "generator.hpp"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "config.hpp"

namespace migrator {
  void generate_migration_folder(const Configuration &configuration,
                                 const std::vector<std::string> &args) {
    auto now =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string datetime(30, '\0');

    std::strftime(&datetime[0], datetime.size(), "%Y-%m-%dT%H-%M-%S",
                  std::localtime(&now));

    datetime.erase(std::remove(datetime.begin(), datetime.end(), '\0'),
                   datetime.end());

    std::string folder_name = datetime + "_" + args[1];
    std::string folder_path = configuration.migration_dir + "/" + folder_name;

    std::filesystem::create_directory(folder_path);

    std::ofstream up_file(folder_path + "/up.sql");

    up_file << "-- Your SQL goes here" << std::endl;

    up_file.close();

    std::ofstream down_file(folder_path + "/down.sql");

    down_file << "-- This file should undo anything in 'up.sql'" << std::endl;

    down_file.close();

    std::cout << "Created the " << folder_name << " migration!\n";
  }
}
