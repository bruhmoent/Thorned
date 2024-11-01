#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include "../GUI/container.hpp"
#include "tinyxml2.h"

class Document {
public:
	Document(const char* filename, const float& screen_size_x, const float& screen_size_y, const std::string& font_path);

	void load(const char* filename);
	const std::shared_ptr<Container> get_main_container() const;
private:
	void parse_components(tinyxml2::XMLElement* element, std::shared_ptr<Container> parent_container);

	std::shared_ptr<Component> create_element(const std::string& tag,
		tinyxml2::XMLElement* element,
		std::shared_ptr<Container> parent_container,
		const sf::Vector2f& parent_size);

	sf::Vector2f parse_position(tinyxml2::XMLElement* element);
	sf::Vector2f parse_size(tinyxml2::XMLElement* element, const sf::Vector2f& parent_size);
	sf::Color parse_color(tinyxml2::XMLElement* element, const std::string& attribute_name);
	AnchorPoint parse_anchor_point(tinyxml2::XMLElement* element);
private:
	float m_screen_size_x, m_screen_size_y;
	std::unique_ptr<tinyxml2::XMLDocument> m_document;
	tinyxml2::XMLElement* m_root;
	std::shared_ptr<Container> m_main_container;
	sf::Font m_font;
	const char* m_filename;
};

#endif // DOCUMENT_HPP