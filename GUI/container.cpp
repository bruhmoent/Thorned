#include "container.hpp"
#include <random>

Container::Container(AlignmentType alignment_type, FitType fit_type,
	const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& color)
	: Component("container"), m_alignment_type(alignment_type),
	m_fit_type(fit_type), m_position(position), m_size(size)
{
	m_shape = std::make_shared<sf::RectangleShape>(m_size);
	m_shape->setPosition(m_position);
	m_shape->setFillColor(color);
}

void Container::update(float dt, const sf::RenderWindow& window) {
	for (const auto& component : m_components) {
		component->update(dt, window);
	}
}

void Container::handle_event(const sf::Event& event, sf::RenderWindow& window) {
	for (const auto& component : m_components) {
		component->handle_event(event, window);
	}
}

std::shared_ptr<Component> Container::get_component(const std::string& identifier) const {
	for (const auto& component : m_components) {
		if (component->get_identifier() == identifier) {
			return component;
		}
	}
	return nullptr;
}

void Container::render(sf::RenderTarget& target) {
	if (m_shape) {
		target.draw(*m_shape);
	}
	for (const auto& component : m_components) {
		if (component) {
			component->render(target);
		}
	}
}

void Container::set_position(const sf::Vector2f& position) {
	const sf::Vector2f offset = position - m_position;
	m_position = position;
	m_shape->setPosition(m_shape->getPosition() + offset);

	arrange_children();
}

void Container::set_size(const sf::Vector2f& size) {
	m_size = size;

	if(m_shape){ 
		m_shape->setSize(size);
	}

	arrange_children();
}

sf::Vector2f Container::get_position() const {
	return m_position;
}

sf::Vector2f Container::get_size() const {
	return m_size;
}

void Container::add_component(std::shared_ptr<Component> component) {
	m_components.push_back(component);
	arrange_children();
}

void Container::delete_component(const std::string& identifier) {
	m_components.remove_if([identifier](const std::shared_ptr<Component>& component) {
		return component->get_identifier() == identifier;
		});
	arrange_children();
}

void Container::arrange_children() {
	if (m_components.empty()) return;

	if (m_alignment_type == AlignmentType::Vertical) {
		float current_y = m_position.y;
		float component_height = m_fit_type == FitType::Fit ?
			m_size.y / m_components.size() : 0.0f;

		for (const auto& component : m_components) {
			sf::Vector2f size = component->get_size();
			if (m_fit_type == FitType::Fit) {
				size = sf::Vector2f(m_size.x, component_height);
			}
			float component_x = m_position.x;
			float component_y = current_y;

			switch (component->get_anchor_point()) {
			case AnchorPoint::Center:
			case AnchorPoint::TopCenter:
			case AnchorPoint::BottomCenter:
				component_x = m_position.x + (m_size.x - size.x) / 2;
				break;
			case AnchorPoint::CenterRight:
			case AnchorPoint::TopRight:
			case AnchorPoint::BottomRight:
				component_x = m_position.x + m_size.x - size.x;
				break;
			default:
				component_x = m_position.x;
				break;
			}
			component->set_position(sf::Vector2f(component_x, component_y));
			if (m_fit_type == FitType::Fit) {
				component->set_size(size);
			}
			current_y += size.y;
		}
	}
	else if (m_alignment_type == AlignmentType::Horizontal) {
		float current_x = m_position.x;
		float component_width = m_fit_type == FitType::Fit ?
			m_size.x / m_components.size() : 0.0f;

		for (const auto& component : m_components) {
			sf::Vector2f size = component->get_size();
			if (m_fit_type == FitType::Fit) {
				size = sf::Vector2f(component_width, m_size.y);
			}
			float component_x = current_x;
			float component_y = m_position.y;

			switch (component->get_anchor_point()) {
			case AnchorPoint::Center:
			case AnchorPoint::CenterLeft:
			case AnchorPoint::CenterRight:
				component_y = m_position.y + (m_size.y - size.y) / 2;
				break;
			case AnchorPoint::BottomLeft:
			case AnchorPoint::BottomCenter:
			case AnchorPoint::BottomRight:
				component_y = m_position.y + m_size.y - size.y;
				break;
			default:
				component_y = m_position.y;
				break;
			}
			component->set_position(sf::Vector2f(component_x, component_y));
			if (m_fit_type == FitType::Fit) {
				component->set_size(size);
			}
			current_x += size.x;
		}
	}
}

const std::list<std::shared_ptr<Component>>& Container::get_components() const {
	return m_components;
}

void Container::clear_components() {
	m_components.clear();
}

void Container::set_fit_type(FitType fit_type) {
	m_fit_type = fit_type;
	arrange_children();
}

FitType Container::get_fit_type() const {
	return m_fit_type;
}