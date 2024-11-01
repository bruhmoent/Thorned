#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

namespace thd
{ 

enum AnchorPoint {
	TopLeft,
	TopCenter,
	TopRight,
	CenterLeft,
	Center,
	CenterRight,
	BottomLeft,
	BottomCenter,
	BottomRight
};

class Component {
public:
	Component(const std::string& identifier) : m_identifier(identifier) {}
	virtual ~Component() = default;

	virtual void update(float dt, const sf::RenderWindow& window) = 0;
	virtual void render(sf::RenderTarget& target) = 0;
	virtual void handle_event(const sf::Event& event, sf::RenderWindow& window) {}

	virtual void set_position(const sf::Vector2f& position) = 0;
	virtual void set_size(const sf::Vector2f& size) = 0;
	virtual sf::Vector2f get_position() const = 0;
	virtual sf::Vector2f get_size() const = 0;

	virtual std::shared_ptr<sf::Text> get_label() const { return nullptr; }

	// Sets the label's string
	virtual void set_label_text(const std::string& text) {}
	std::string get_identifier() const { return m_identifier; }

	void set_anchor_point(AnchorPoint anchor_point) { m_anchor_point = anchor_point; }
	AnchorPoint get_anchor_point() const { return m_anchor_point; }
protected:
	AnchorPoint m_anchor_point = AnchorPoint::TopLeft;
	std::string m_identifier = "";
};

} // namespace thd
#endif // COMPONENT_HPP
