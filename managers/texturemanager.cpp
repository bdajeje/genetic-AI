#include "texturemanager.hpp"

#include "utils/directory.hpp"

namespace texture {

std::unique_ptr<TextureManager> TextureManager::_instance;

void TextureManager::init(const std::string& image_directory)
{
  if( _instance )
    return;

  _instance.reset( new TextureManager(image_directory) );
}

TextureManager::TextureManager(const std::string& image_directory)
  : _directory { utils::ensureDirectoryEnd(image_directory) }
{}

const sf::Texture& TextureManager::get(const std::string& texture_file_path)
{
  TextureManager& instance = TextureManager::instance();

  // Find texture
  auto found_it = instance._textures.find(texture_file_path);

  // Found, return it
  if( found_it != instance._textures.end() )
    return found_it->second;

  // Not already loaded, so load it then return it
  return instance.loadTexture(texture_file_path);
}

const sf::Texture& TextureManager::loadTexture(const std::string& texture_file_path)
{
  // Load texture
  sf::Texture& texture = _textures[texture_file_path];
  if( !texture.loadFromFile( _directory + texture_file_path ) )
    throw std::invalid_argument {"Can't load texture: " + texture_file_path};

  return texture;
}

} // texture
