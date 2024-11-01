#include "label.hpp"

Label::Label(const std::string& identifier, const sf::Font& font, const std::string& text,
	unsigned int font_size, sf::Color color)
	: Component(identifier), m_text(std::make_shared<sf::Text>(text, font, font_size)), m_color(color) {
	m_text->setFillColor(color);
	m_size = calculate_text_bounds();
}

void Label::update(float dt, const sf::RenderWindow& window) {}

void Label::set_label_text(const std::string& text)
{
	m_text->setString(text);
	m_size = calculate_text_bounds();
}

void Label::set_position(const sf::Vector2f& position) {
	const sf::Vector2f adjusted_text_position = calculate_anchor_position(position);
	m_text->setPosition(adjusted_text_position);
}

void Label::set_size(const sf::Vector2f& size) {
	m_size = size;
}

sf::Vector2f Label::get_position() const {
	return  m_text->getPosition();
}

sf::Vector2f Label::get_size() const {
	return m_size;
}

void Label::render(sf::RenderTarget& target) {
	target.draw(*m_text);
}

sf::Vector2f Label::calculate_text_bounds() {
	const sf::FloatRect bounds = m_text->getLocalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}

sf::Vector2f Label::calculate_anchor_position(const sf::Vector2f& base_position) const {
	sf::Vector2f anchor_offset;
	switch (m_anchor_point) {
	case AnchorPoint::TopLeft:
		anchor_offset = sf::Vector2f(0.f, 0.f);
		break;
	case AnchorPoint::TopCenter:
		anchor_offset = sf::Vector2f((m_size.x - m_text->getLocalBounds().width) / 2.f, 0.f);
		break;
	case AnchorPoint::TopRight:
		anchor_offset = sf::Vector2f(m_size.x - m_text->getLocalBounds().width, 0.f);
		break;
	case AnchorPoint::CenterLeft:
		anchor_offset = sf::Vector2f(0.f, (m_size.y - m_text->getLocalBounds().height) / 2.f);
		break;
	case AnchorPoint::Center:
		anchor_offset = sf::Vector2f(
			(m_size.x - m_text->getGlobalBounds().width) / 2.f,
			(m_size.y - m_text->getGlobalBounds().height * 2 + m_text->getGlobalBounds().height / 4.f) / 2.f
		);
		break;
	case AnchorPoint::CenterRight:
		anchor_offset = sf::Vector2f(m_size.x - m_text->getLocalBounds().width,
			(m_size.y - m_text->getLocalBounds().height) / 2.f);
		break;
	case AnchorPoint::BottomLeft:
		anchor_offset = sf::Vector2f(0.f, m_size.y - m_text->getLocalBounds().height);
		break;
	case AnchorPoint::BottomCenter:
		anchor_offset = sf::Vector2f((m_size.x - m_text->getLocalBounds().width) / 2.f,
			m_size.y - m_text->getLocalBounds().height);
		break;
	case AnchorPoint::BottomRight:
		anchor_offset = sf::Vector2f(m_size.x - m_text->getLocalBounds().width,
			m_size.y - m_text->getLocalBounds().height);
		break;
	}
	return base_position + anchor_offset;
}
