#include "button.hpp"

Button::Button(const std::string& identifier, const sf::Vector2f& position, const sf::Vector2f& size,
	const sf::Color& color, const sf::Color& hover_color, const sf::Color& click_color,
	const std::string& label_text, const sf::Font& font, unsigned int font_size, std::function<void()> on_click): Component(identifier), m_color(color), m_hover_color(hover_color), m_click_color(click_color), m_on_click(on_click), m_position(position), m_size(size), m_text(std::make_shared<sf::Text>(label_text, font, font_size))
{
	m_shape = std::make_shared<sf::RectangleShape>();
	m_shape->setSize(size);
	m_shape->setPosition(position);
	m_shape->setFillColor(color);

	m_text->setCharacterSize(font_size);
	m_text->setFillColor(sf::Color::White);

	const sf::FloatRect label_bounds = m_text->getLocalBounds();
	m_text->setOrigin(label_bounds.width / 2, label_bounds.top + label_bounds.height / 2);
	m_text->setPosition(position.x + size.x / 2, position.y + size.y / 2);
}

void Button::update(float dt, const sf::RenderWindow& window)
{
	sf::Vector2f mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	if (m_shape->getGlobalBounds().contains(mouse_position))
	{
		m_is_hovered = true;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			if (!m_is_clicked)
			{
				m_is_clicked = true;
			}
		}
		else if (m_is_clicked)
		{
			m_is_clicked = false;
			if (m_on_click)
			{
				m_on_click();
			}
		}
	}
	else
	{
		m_is_hovered = false;
		m_is_clicked = false;
	}

	if (m_is_clicked)
	{
		m_shape->setFillColor(m_click_color);
	}
	else if (m_is_hovered)
	{
		m_shape->setFillColor(m_hover_color);
	}
	else
	{
		m_shape->setFillColor(m_color);
	}
}

void Button::render(sf::RenderTarget& target) {
	if (m_shape) {
		target.draw(*m_shape);
	}
	if (m_text) {
		target.draw(*m_text);
	}
}

void Button::set_position(const sf::Vector2f& position)
{
	m_position = position;
	m_shape->setPosition(position);

	m_text->setPosition(
		position.x + m_size.x / 2.f,
		position.y + (m_size.y - m_text->getCharacterSize()) / 2.f + m_text->getCharacterSize() / 2.f
	);
}

void Button::set_size(const sf::Vector2f& size)
{
	m_size = size;
	m_shape->setSize(size);

	m_text->setPosition(
		m_position.x + size.x / 2.f,
		m_position.y + (size.y - m_text->getCharacterSize()) / 2.f + m_text->getCharacterSize() / 2.f
	);
}

sf::Vector2f Button::get_position() const
{
	return m_position;
}

sf::Vector2f Button::get_size() const
{
	return m_size;
}

void Button::set_label(const std::string& label_text)
{
	m_text->setString(label_text);

	m_text->setOrigin(
		m_text->getLocalBounds().width / 2.f,
		m_text->getCharacterSize() / 2.f
	);

	m_text->setPosition(
		m_position.x + m_size.x / 2.f,
		m_position.y + m_size.y / 2.f
	);
}