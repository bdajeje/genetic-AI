#include "fontmanager.hpp"

#include "utils/directory.hpp"

namespace font {

std::unique_ptr<FontManager> FontManager::_instance;

void FontManager::init(const std::string& font_directory)
{
  if( _instance )
    return;

  _instance.reset( new FontManager(font_directory) );
}

FontManager::FontManager(const std::string& font_directory)
  : _directory { utils::ensureDirectoryEnd(font_directory) }
{}

const sf::Font& FontManager::get(const std::string& font_file_path)
{
  FontManager& instance = FontManager::instance();

  // Find font
  auto found_it = instance._fonts.find(font_file_path);

  // Found, return it
  if( found_it != instance._fonts.end() )
    return found_it->second;

  // Not already loaded, so load it then return it
  return instance.loadFont(font_file_path);
}

const sf::Font& FontManager::loadFont(const std::string& font_file_path)
{
  // Load font
  sf::Font& font = _fonts[font_file_path];
  if( !font.loadFromFile( _directory + font_file_path ) )
    throw std::invalid_argument {"Can't load font: " + font_file_path};

  return font;
}

}
