#ifndef BENLIB_FILESYSTEM_EXPERIMENTATION_HPP
#define BENLIB_FILESYSTEM_EXPERIMENTATION_HPP

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace benlib
{

namespace filesystem
{

namespace experimentation
{

void list_all_files(std::vector<std::string>& list, std::string_view path = "")
{
  for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
    list.emplace_back(entry.path().string());
  }
}

}  // namespace experimentation

}  // namespace filesystem

}  // namespace benlib

#endif  // BENLIB_FILESYSTEM_EXPERIMENTATION_HPP
