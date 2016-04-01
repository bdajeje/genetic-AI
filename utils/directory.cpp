#include "directory.hpp"

#include <boost/algorithm/string.hpp>

namespace utils {

std::string ensureDirectoryEnd(const std::string& path)
{
  if(!boost::algorithm::ends_with(path, "/"))
    return path + '/';

  return path;
}

} // utils
