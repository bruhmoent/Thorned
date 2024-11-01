#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "../component.hpp"

class Image : public Component {
public:
	Image(const std::string& identifier, const std::string& path, int width, int height);

	void render(sf::RenderTarget& target) override;
	void update(float dt, const sf::RenderWindow& window) override {}

	void set_position(const sf::Vector2f& position) override;
	void set_size(const sf::Vector2f& size) override;
	sf::Vector2f get_position() const override;
	sf::Vector2f get_size() const override;

	void set_path(const std::string& path);
private:
	std::shared_ptr<sf::Texture> m_texture;
	std::shared_ptr<sf::Sprite> m_sprite;
	int m_width;
	int m_height;
	sf::Vector2f m_position;
};

#endif // IMAGE_HPP
