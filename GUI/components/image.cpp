#include "image.hpp"

Image::Image(const std::string& identifier, const std::string& path, int width, int height)
	: Component(identifier), m_width(width), m_height(height)
{
	m_texture = std::make_shared<sf::Texture>();
	if (!m_texture->loadFromFile(path)) {
		throw std::runtime_error("Failed to load image: " + path);
	}

	m_sprite = std::make_shared<sf::Sprite>();
	m_sprite->setTexture(*m_texture);
	set_size(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
}

void Image::render(sf::RenderTarget& target) {
	target.draw(*m_sprite);
}

void Image::set_position(const sf::Vector2f& position) {
	m_sprite->setPosition(position);
	m_position = position;
}

void Image::set_size(const sf::Vector2f& size) {
	m_width = static_cast<int>(size.x);
	m_height = static_cast<int>(size.y);
	m_sprite->setScale(
		size.x / m_texture->getSize().x,
		size.y / m_texture->getSize().y
	);
}

sf::Vector2f Image::get_position() const {
	return m_position;
}

sf::Vector2f Image::get_size() const {
	return sf::Vector2f(static_cast<float>(m_width), static_cast<float>(m_height));
}

void Image::set_path(const std::string& path) {
	if (!m_texture->loadFromFile(path)) {
		throw std::runtime_error("Failed to load image: " + path);
	}
	m_sprite->setTexture(*m_texture);
	set_size(sf::Vector2f(static_cast<float>(m_width), static_cast<float>(m_height)));
}