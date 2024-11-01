#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "component.hpp"
#include <list>

enum AlignmentType {
	Vertical,
	Horizontal
};

enum FitType {
	Fit,
	Default
};

class Container : public Component {
public:
	Container(AlignmentType alignment_type, FitType fit_type = FitType::Default,
		const sf::Vector2f& position = sf::Vector2f(0.0f, 0.0f),
		const sf::Vector2f& size = sf::Vector2f(0.0f, 0.0f), const sf::Color& color = sf::Color::White);

	void arrange_children();

	void update(float dt, const sf::RenderWindow& window) override;
	void render(sf::RenderTarget& target) override;
	void handle_event(const sf::Event& event, sf::RenderWindow& window) override;

	void set_position(const sf::Vector2f& position) override;
	void set_size(const sf::Vector2f& size) override;
	sf::Vector2f get_position() const override;
	sf::Vector2f get_size() const override;
	void set_fit_type(FitType fit_type);
	FitType get_fit_type() const;

	void add_component(std::shared_ptr<Component> component);
	void delete_component(const std::string& identifier);
	const std::list<std::shared_ptr<Component>>& get_components() const;
	void clear_components();
	std::shared_ptr<Component> get_component(const std::string& identifier) const;
private:
	AlignmentType m_alignment_type;
	FitType m_fit_type;
	std::list<std::shared_ptr<Component>> m_components;
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	std::shared_ptr<sf::RectangleShape> m_shape;
};

#endif // CONTAINER_HPP
