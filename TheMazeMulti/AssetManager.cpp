#include "AssetManager.h"


const sf::Texture& AssetManager::GetImage(const std::string& filename)
{
	for (std::map<std::string, sf::Texture>::const_iterator it = _images.begin();
		it != _images.end();
		++it)
	{
		if (filename == it->first)
		{
			return it->second;
		}
	}

	sf::Texture image;


	if (image.loadFromFile(filename))
	{
		_images[filename] = image;
		_images[filename].setSmooth(true);
		return _images[filename];
	}

	return image;
}

const sf::SoundBuffer* AssetManager::GetSound(const std::string & filename)
{
	for (std::map<std::string, sf::SoundBuffer>::const_iterator it = _sounds.begin(); it != _sounds.end(); ++it) {
		if (filename == it->first)
		{
			return &(it->second);
		}
	}

	sf::SoundBuffer sound;


	if (sound.loadFromFile(filename))
	{
		_sounds[filename] = sound;
		return &_sounds[filename];
	}

	return &sound;

}

const sf::Font& AssetManager::GetFont(const std::string & filename)
{
	for (std::map<std::string, sf::Font>::const_iterator it = _fonts.begin(); it != _fonts.end(); ++it) {
		if (filename == it->first)
		{
			return it->second;
		}
	}

	sf::Font font;
	if (font.loadFromFile(filename))
	{
		_fonts[filename] = font;
		return _fonts[filename];
	}
	return font;
}

