#ifndef LABEL_HPP
#define LABEL_HPP

#include "../component.hpp"

class Label : public Component {
public:
	Label(const std::string& identifier, const sf::Font& font, const std::string& text,
		unsigned int font_size, sf::Color color);

	void render(sf::RenderTarget& target) override;
	void update(float dt, const sf::RenderWindow& window) override;

	void set_position(const sf::Vector2f& position) override;
	void set_size(const sf::Vector2f& size) override;
	sf::Vector2f get_position() const override;
	sf::Vector2f get_size() const override;
	void set_label_text(const std::string& text) override;
private:
	sf::Vector2f calculate_text_bounds();
	sf::Vector2f calculate_anchor_position(const sf::Vector2f& base_position) const;

	std::shared_ptr<sf::Text> m_text;
	sf::Color m_color;
	sf::Vector2f m_size;
};

#endif // LABEL_HPP
