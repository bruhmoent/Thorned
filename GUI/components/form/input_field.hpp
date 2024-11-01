#ifndef INPUT_FIELD_HPP
#define INPUT_FIELD_HPP

#include "../../component.hpp"

class InputField : public Component {
public:
	InputField(const std::string& identifier, const sf::Vector2f& position, const sf::Vector2f& size,
		const sf::Font& font, unsigned font_size, const sf::Color& color = sf::Color::White,
		const sf::Color& text_color = sf::Color::Black, const sf::Color& cursor_color = sf::Color::Black, const std::string& placeholder_text = "",
		const std::string& text = "", const float screen_size_x = 800.f, const float screen_size_y = 600.f);

	void update(float dt, const sf::RenderWindow& window) override;
	void render(sf::RenderTarget& target) override;

	void set_position(const sf::Vector2f& position) override;
	void set_size(const sf::Vector2f& size) override;
	sf::Vector2f get_position() const override;
	sf::Vector2f get_size() const override;
	void set_text(const std::string& text);
	std::string get_text() const;
	void set_placeholder_text(const std::string& placeholder_text);
	std::string get_placeholder_text() const;
	void set_color(const sf::Color& color);
	sf::Color get_color() const;
	void set_text_color(const sf::Color& text_color);
	sf::Color get_text_color() const;
	void set_cursor_color(const sf::Color& cursor_color);
	sf::Color get_cursor_color() const;
	void set_font(const sf::Font& font);
	const sf::Font& get_font() const;
	void set_font_size(unsigned font_size);
	unsigned get_font_size() const;
	void set_cursor_position(unsigned cursor_position);
	unsigned get_cursor_position() const;
	void set_is_focused(bool is_focused);
	bool get_is_focused() const;
	std::shared_ptr<sf::Text> get_label() const;

	void handle_event(const sf::Event& event, sf::RenderWindow& window) override;
	void handle_mouse_press(const sf::Event& event, sf::RenderWindow& window);
	void handle_text_input(const sf::Event& event);
	void handle_key_press(const sf::Event& event);
	unsigned calculate_cursor_position_from_x(const float& click_x);
	unsigned find_previous_word();
	unsigned find_next_word();
private:
	void update_displayed_text();
	void update_cursor_position();
	void update_view();

	std::shared_ptr<sf::Text> m_text;
	std::shared_ptr<sf::Text> m_placeholder_text;
	std::shared_ptr<sf::RectangleShape> m_shape;
	sf::RectangleShape m_cursor;
	sf::Vector2f m_size;
	sf::View m_view;

	sf::Color m_color;
	sf::Color m_outline_color;
	sf::Color m_text_color;
	sf::Color m_cursor_color;
	sf::Font m_font;
	unsigned m_font_size;

	std::string m_text_string;
	unsigned m_cursor_position = 0;
	bool m_is_focused = false;
	bool m_cursor_visible = true;
	sf::Clock m_cursor_timer; 

	const float m_screen_size_x = 0.f;
	const float m_screen_size_y = 0.f;
};

#endif // INPUT_FIELD_HPP
