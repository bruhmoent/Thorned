#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include "../component.hpp"
#include <functional>

class Button : public Component {
public:
	Button(const std::string& identifier, const sf::Vector2f& position, const sf::Vector2f& size,
		const sf::Color& color, const sf::Color& hover_color, const sf::Color& click_color,
		const std::string& label_text, const sf::Font& font, unsigned int font_size,
		std::function<void()> on_click);

	void update(float dt, const sf::RenderWindow& window) override;
	void render(sf::RenderTarget& target) override;

	void set_position(const sf::Vector2f& position) override;
	void set_size(const sf::Vector2f& size) override;
	sf::Vector2f get_position() const override;
	sf::Vector2f get_size() const override;
	void set_label(const std::string& label_text);
private:
	sf::Color m_color;
	sf::Color m_hover_color;
	sf::Color m_click_color;
	std::function<void()> m_on_click;
	bool m_is_hovered = false;
	bool m_is_clicked = false;
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	std::shared_ptr<sf::Text> m_text;
	std::shared_ptr<sf::RectangleShape> m_shape;
};

#endif // BUTTON_HPP
