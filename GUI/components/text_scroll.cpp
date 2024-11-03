#include "text_scroll.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

std::string file_content_temp = "";

using namespace thd;

TextScroll::TextScroll(const std::string& identifier, const std::string& text, const sf::Font& font,
	const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& background_color,
	const sf::Color& font_color, unsigned font_size, const float screen_size_x, const float screen_size_y, const std::string& path, const bool& is_in_fit_container)
	: Component(identifier), m_size(size), m_position(position), m_screen_size_x(screen_size_x), m_screen_size_y(screen_size_y), 
	m_is_in_fit_container(is_in_fit_container) {
	m_text = std::make_shared<sf::Text>();
	m_text->setFont(font);
	set_text(text);
	m_text->setCharacterSize(font_size);
	m_text->setFillColor(font_color);
	m_text->setPosition(position);

	m_shape = std::make_shared<sf::RectangleShape>(size);
	m_shape->setFillColor(background_color);
	m_shape->setPosition(position);

	if (path != "none") {
		load_from_file(path);
	}

	update_view();
}

void TextScroll::load_from_file(const std::string& file_path) {
	std::ifstream file(file_path);
	if (!file.is_open()) {
		std::cerr << "Error: Could not open file " << file_path << std::endl;
		return;
	}

	std::stringstream buffer;
	std::string line;
	while (std::getline(file, line)) {
		buffer << line << '\n';
	}

	std::string file_content = buffer.str();
	if (file_content.empty()) {
		std::cerr << "File is empty or not readable." << std::endl;
	}
	else {
		if (!m_is_in_fit_container)
			set_text(file_content);
		else
			file_content_temp = file_content;
	}

	file.close();
}

void TextScroll::set_text(const std::string& text) {
	if (m_shape) { 
		std::string wrapped_text = wrap_text(text, m_shape->getSize().x);
		m_text->setString(wrapped_text);
	}
}

std::string TextScroll::wrap_text(const std::string& text, const float& width) {
	std::string result;
	std::istringstream line_stream(text);
	std::string line;

	while (std::getline(line_stream, line)) {
		m_text->setString(line);

		if (m_text->getGlobalBounds().width <= width) {
			result += line + "\n";
			continue;
		}

		std::istringstream word_stream(line);
		std::string word;
		std::string current_line;

		while (word_stream >> word) {
			std::string test_line = current_line.empty() ? word : current_line + " " + word;
			m_text->setString(test_line);

			if (m_text->getGlobalBounds().width > width) {
				result += current_line + "\n";
				current_line = word;
			}
			else {
				current_line = test_line;
			}
		}

		if (!current_line.empty()) {
			result += current_line + "\n";
		}
	}

	if (!result.empty() && result.back() == '\n') {
		result.pop_back();
	}

	return result;
}

void TextScroll::update(float dt, const sf::RenderWindow& window) {}

void TextScroll::render(sf::RenderTarget& target) {
	target.draw(*m_shape);

	sf::View previousView = target.getView();
	target.setView(m_view);

	target.draw(*m_text);

	target.setView(previousView);
}

void TextScroll::handle_event(const sf::Event& event, sf::RenderWindow& window) {
	if (event.type == sf::Event::MouseWheelScrolled) {
		sf::Vector2f world_position = window.mapPixelToCoords(sf::Mouse::getPosition(window), m_view);

		sf::FloatRect viewport = m_view.getViewport();

		sf::FloatRect view_bounds(
			viewport.left * m_screen_size_x,
			viewport.top * m_screen_size_y,
			viewport.width * m_screen_size_x,
			viewport.height * m_screen_size_y
		);

		if (view_bounds.contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
			if (event.mouseWheelScroll.delta > 0) {
				m_view.move(0, -10.f);
			}
			else {
				m_view.move(0, 10.f);
			}
		}
	}
}

void TextScroll::set_position(const sf::Vector2f& position) {
	m_position = position;
	m_shape->setPosition(position);
	m_text->setPosition(position);

	update_view();
}

void TextScroll::set_size(const sf::Vector2f& size) {
	m_size = size;
	m_shape->setSize(size);

	set_text(file_content_temp);
	update_view();
}

void TextScroll::update_view() {
	const sf::FloatRect bounds = m_shape->getGlobalBounds();

	m_view.setCenter(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	m_view.setSize(bounds.width, bounds.height);

	m_view.setViewport(sf::FloatRect(
		bounds.left / m_screen_size_x,
		bounds.top / m_screen_size_y,
		bounds.width / m_screen_size_x,
		bounds.height / m_screen_size_y));
}
