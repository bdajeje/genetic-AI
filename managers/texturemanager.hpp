#ifndef MANAGERS_TEXTURE_MANAGER_HPP
#define MANAGERS_TEXTURE_MANAGER_HPP

#include <SFML/Graphics/Texture.hpp>

#include <map>
#include <memory>

namespace texture {

class TextureManager final
{
  public:

    static void init(const std::string& image_directory);

    /* Get the unique instance */
    static TextureManager& instance() { return *_instance; }

    static const sf::Texture& get(const std::string& texture_file_path);

  private:

    TextureManager(const std::string& image_directory);

    const sf::Texture& loadTexture(const std::string& texture);

  private:

    static std::unique_ptr<TextureManager> _instance;

    /* Directory when to find resources */
    const std::string _directory;

    /* Cached textures */
    std::map<std::string, sf::Texture> _textures;
};

} // texture

#endif // MANAGERS_TEXTURE_MANAGER_HPP
