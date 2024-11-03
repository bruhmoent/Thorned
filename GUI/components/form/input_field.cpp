#include "input_field.hpp"
#include <iostream>
using namespace thd;

InputField::InputField(const std::string& identifier, const sf::Vector2f& position, const sf::Vector2f& size,
	const sf::Font& font, unsigned font_size, const sf::Color& color,
	const sf::Color& text_color, const sf::Color& cursor_color, const std::string& placeholder_text,
	const std::string& text, const float screen_size_x, const float screen_size_y)
	: Component(identifier), m_color(color), m_text_color(text_color), m_cursor_color(cursor_color),
	m_font(font), m_font_size(font_size), m_cursor_position(0), m_cursor_visible(true),
	m_text_string(text), m_size(size), m_screen_size_x(screen_size_x), m_screen_size_y(screen_size_y), m_text(std::make_shared<sf::Text>(text, font, font_size)), m_placeholder_text(std::make_shared<sf::Text>(placeholder_text, font, font_size)){

	m_shape = std::make_shared<sf::RectangleShape>(size);
	m_shape->setFillColor(color);

	m_text->setFillColor(text_color);
	m_placeholder_text->setFillColor(sf::Color(text_color.r, text_color.g, text_color.b, text_color.a / 2));

	set_position(position);

	m_cursor.setSize(sf::Vector2f(2.0f, static_cast<float>(font_size)));
	m_cursor.setFillColor(cursor_color);

	m_cursor_timer.restart();
	update_view();
}

void InputField::render(sf::RenderTarget& target) {
	if (m_shape) {
		target.draw(*m_shape);
	}

	sf::View original_view = target.getView();
	target.setView(m_view);

	if (m_text_string.empty()) {
		target.draw(*m_placeholder_text);
	}
	else {
		if (m_text) {
			target.draw(*m_text);
		}
	}

	if (m_is_focused && m_cursor_visible) {
		target.draw(m_cursor);
	}

	target.setView(original_view);
}

std::shared_ptr<sf::Text> InputField::get_label() const
{
	return m_text;
}

void InputField::update(float dt, const sf::RenderWindow& window) {
	if (m_cursor_timer.getElapsedTime().asSeconds() >= 0.5f) {
		m_cursor_visible = !m_cursor_visible;
		m_cursor_timer.restart();
	}

	update_displayed_text();
}

void InputField::handle_event(const sf::Event& event, sf::RenderWindow& window) {
	switch (event.type) {
		case sf::Event::MouseButtonPressed: {
			handle_mouse_press(event, window);
			break;
		}
		case sf::Event::TextEntered: {
			if (m_is_focused) {
				handle_text_input(event);
			}
			break;
		}
		case sf::Event::KeyPressed: {
			if (m_is_focused) {
				handle_key_press(event);
			}
			break;
		}
	}
}

void InputField::handle_mouse_press(const sf::Event& event, sf::RenderWindow& window) {
	sf::Vector2i pixel_pos(event.mouseButton.x, event.mouseButton.y);
	sf::Vector2f world_pos = window.mapPixelToCoords(pixel_pos);

	bool was_focused = m_is_focused;
	m_is_focused = m_shape->getGlobalBounds().contains(world_pos);

	if (m_is_focused) {
		const float click_x = world_pos.x - m_shape->getPosition().x;

		m_cursor_position = calculate_cursor_position_from_x(click_x, window);
		update_cursor_position();
	}
}

void InputField::handle_text_input(const sf::Event& event) {
	if (event.text.unicode < 128) {
		const char unicode = static_cast<char>(event.text.unicode);

		if (unicode == '\b') {
			if (!m_text_string.empty() && m_cursor_position > 0) {
				m_text_string.erase(m_cursor_position - 1, 1);
				m_cursor_position--;
				update_displayed_text();
			}
		}
		else if (unicode != '\r' && unicode != '\n') {
			m_text_string.insert(m_cursor_position, 1, unicode);
			m_cursor_position++;
			update_displayed_text();
		}
	}
}

void InputField::handle_key_press(const sf::Event& event) {
	switch (event.key.code) {
		case sf::Keyboard::Left: {
			if (m_cursor_position > 0) {
				if (event.key.control) {
					m_cursor_position = find_previous_word();
				}
				else {
					m_cursor_position--;
				}
				update_cursor_position();
			}
			break;
		}
		case sf::Keyboard::Right: {
			if (m_cursor_position < m_text_string.size()) {
				if (event.key.control) {
					m_cursor_position = find_next_word();
				}
				else {
					m_cursor_position++;
				}
				update_cursor_position();
			}
			break;
		}
		case sf::Keyboard::Home: {
			m_cursor_position = 0;
			update_cursor_position();
			break;
		}
		case sf::Keyboard::End: {
			m_cursor_position = static_cast<unsigned>(m_text_string.size());
			update_cursor_position();
			break;
		}
		case sf::Keyboard::Delete: {
			if (!m_text_string.empty() && m_cursor_position < m_text_string.size()) {
				m_text_string.erase(m_cursor_position, 1);
				update_displayed_text();
			}
			break;
		}
	}
}

unsigned InputField::calculate_cursor_position_from_x(const float& click_x, const sf::RenderWindow& window) {
	sf::Vector2f world_pos = window.mapPixelToCoords(sf::Vector2i(static_cast<int>(click_x), 0), m_view);

	sf::Text temp_text;
	temp_text.setFont(m_font);
	temp_text.setCharacterSize(m_font_size);

	for (size_t i = 0; i <= m_text_string.length(); ++i) {
		temp_text.setString(m_text_string.substr(0, i));
		if (temp_text.getGlobalBounds().width >= world_pos.x) {
			return static_cast<unsigned>(i - 1);
		}
	}

	return static_cast<unsigned>(m_text_string.length());
}

unsigned InputField::find_previous_word() {
	if (m_cursor_position <= 1) return 0;

	unsigned pos = m_cursor_position - 1;
	while (pos > 0 && std::isspace(m_text_string[pos - 1])) pos--;
	while (pos > 0 && !std::isspace(m_text_string[pos - 1])) pos--;

	return pos;
}

unsigned InputField::find_next_word() {
	unsigned pos = m_cursor_position;
	unsigned len = static_cast<unsigned>(m_text_string.length());

	while (pos < len && !std::isspace(m_text_string[pos])) pos++;
	while (pos < len && std::isspace(m_text_string[pos])) pos++;

	return pos;
}

void InputField::update_displayed_text() {
	m_text->setString(m_text_string);
	update_cursor_position();

	float text_width = m_text->getGlobalBounds().width;
	float cursor_x = m_cursor.getPosition().x;

	// Right edge
	if (cursor_x > m_view.getCenter().x + m_size.x / 2.0f - 10.0f) {
		m_view.setCenter(cursor_x - m_size.x / 2.0f + 10.0f, m_view.getCenter().y);
	}
	// Left edge
	else if (cursor_x < m_view.getCenter().x - m_size.x / 2.0f + 10.0f) {
		m_view.setCenter(cursor_x + m_size.x / 2.0f - 10.0f, m_view.getCenter().y);
	}
}

void InputField::update_cursor_position() {
	const float cursor_offset_x = m_text->findCharacterPos(m_cursor_position).x;

	m_cursor.setPosition(cursor_offset_x, m_text->getPosition().y + 2.5f);

	// Right edge
	if (m_cursor.getPosition().x > m_view.getCenter().x + m_size.x / 2.0f - 10.0f) {
		m_view.setCenter(m_cursor.getPosition().x - m_size.x / 2.0f + 10.0f, m_view.getCenter().y);
	}
	// Left edge
	else if (m_cursor.getPosition().x < m_view.getCenter().x - m_size.x / 2.0f + 10.0f) {
		m_view.setCenter(m_cursor.getPosition().x + m_size.x / 2.0f - 10.0f, m_view.getCenter().y);
	}
}

void InputField::update_view() {
	const sf::FloatRect bounds = m_shape->getGlobalBounds();

	m_view.setCenter(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);

	m_view.setSize(bounds.width, bounds.height);

	m_view.setViewport(sf::FloatRect(
		bounds.left / m_screen_size_x,
		bounds.top / m_screen_size_y,
		bounds.width / m_screen_size_x,
		bounds.height / m_screen_size_y));
}

void InputField::set_position(const sf::Vector2f& position) {
	m_shape->setPosition(position);

	float vertical_center = position.y + m_shape->getSize().y / 2 - m_placeholder_text->getLocalBounds().height + 5.0f;

	m_text->setPosition(position.x, vertical_center);
	m_placeholder_text->setPosition(m_text->getPosition());

	update_view();
	update_cursor_position();
}

void InputField::set_size(const sf::Vector2f& size) {
	m_shape->setSize(size);
	update_view();
	update_cursor_position();
}

sf::Vector2f InputField::get_position() const {
	return m_shape->getPosition();
}

sf::Vector2f InputField::get_size() const {
	return m_shape->getSize();
}

void InputField::set_text(const std::string& text) {
	m_text_string = text;
	update_displayed_text();
}

std::string InputField::get_text() const {
	return m_text_string;
}

void InputField::set_placeholder_text(const std::string& placeholder_text) {
	m_placeholder_text->setString(placeholder_text);
}

std::string InputField::get_placeholder_text() const {
	return m_placeholder_text->getString();
}

void InputField::set_color(const sf::Color& color) {
	m_color = color;
	m_shape->setFillColor(color);
}

sf::Color InputField::get_color() const {
	return m_color;
}

void InputField::set_text_color(const sf::Color& text_color) {
	m_text_color = text_color;
	m_text->setFillColor(text_color);
}

sf::Color InputField::get_text_color() const {
	return m_text_color;
}

void InputField::set_cursor_color(const sf::Color& cursor_color) {
	m_cursor_color = cursor_color;
	m_cursor.setFillColor(cursor_color);
}

sf::Color InputField::get_cursor_color() const {
	return m_cursor_color;
}

void InputField::set_font(const sf::Font& font) {
	m_font = font;
	m_text->setFont(font);
	m_placeholder_text->setFont(font);
}

const sf::Font& InputField::get_font() const {
	return m_font;
}

void InputField::set_font_size(unsigned font_size) {
	m_font_size = font_size;
	m_text->setCharacterSize(font_size);
	m_placeholder_text->setCharacterSize(font_size);
}

unsigned InputField::get_font_size() const {
	return m_font_size;
}

void InputField::set_cursor_position(unsigned cursor_position) {
	m_cursor_position = cursor_position;
	update_cursor_position();
}

unsigned InputField::get_cursor_position() const {
	return m_cursor_position;
}

void InputField::set_is_focused(bool is_focused) {
	m_is_focused = is_focused;
}

bool InputField::get_is_focused() const {
	return m_is_focused;
}
