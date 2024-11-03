#ifndef TEXT_SCROLL_HPP
#define TEXT_SCROLL_HPP

#include "../component.hpp"

namespace thd
{

class TextScroll : public Component
{
public:
	TextScroll(const std::string& identifier, const std::string& text, const sf::Font& font,
		const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& background_color,
		const sf::Color& font_color, unsigned font_size = 30,
		const float screen_size_x = 800.f, const float screen_size_y = 600.f, const std::string& path = "none", const bool& is_in_fit_container = false);

	void load_from_file(const std::string& file_path);
	void update(float dt, const sf::RenderWindow& window) override;
	void render(sf::RenderTarget& target) override;
	void handle_event(const sf::Event& event, sf::RenderWindow& window) override;

	void set_position(const sf::Vector2f& position) override;
	void set_size(const sf::Vector2f& size) override;
	sf::Vector2f get_position() const override { return m_position; }
	sf::Vector2f get_size() const override { return m_size; }
	void set_text(const std::string& text);

private:
	sf::View m_view;
	std::shared_ptr<sf::Text> m_text;
	sf::Vector2f m_size;
	sf::Vector2f m_position;
	std::shared_ptr<sf::RectangleShape> m_shape;
	const float m_screen_size_x;
	const float m_screen_size_y;
	float m_scroll_offset;
	bool m_is_in_fit_container;

	void update_view();
	std::string wrap_text(const std::string& text, const float& width);
};

} // namespace thd
#endif // TEXT_SCROLL_HPP
