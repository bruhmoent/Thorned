#include "document.hpp"
#include "../GUI/container.hpp"
#include "../GUI/components/button.hpp"
#include "../GUI/components/image.hpp"
#include "../GUI/components/label.hpp"
#include "../GUI/components/form/input_field.hpp"
#include <sstream>
#include <iostream>

Document::Document(const char* filename, const float& screen_size_x, const float& screen_size_y, const std::string& font_path) : m_filename(filename), m_root(nullptr), m_screen_size_x(screen_size_x), m_screen_size_y(screen_size_y){
	m_document = std::make_unique<tinyxml2::XMLDocument>();
	m_main_container = std::make_shared<Container>(AlignmentType::Vertical);
	if (!m_font.loadFromFile(font_path)) {
		std::cerr << "Error loading font file: " << font_path << std::endl;
	}
}

void Document::load(const char* filename) {
	if (m_document->LoadFile(filename) == tinyxml2::XML_SUCCESS) {
		m_root = m_document->RootElement();
		if (m_root) {
			parse_components(m_root, m_main_container);
			m_main_container->arrange_children();
		}
		else {
			std::cerr << "Error: Root element not found in " << filename << std::endl;
		}
	}
	else {
		std::cerr << "Error loading file: " << filename << std::endl;
	}
}

AnchorPoint Document::parse_anchor_point(tinyxml2::XMLElement* element) {
	const char* anchor_str = element->Attribute("anchorPoint");
	if (!anchor_str) return AnchorPoint::TopLeft;

	const std::string anchor(anchor_str);
	if (anchor == "TopLeft") return AnchorPoint::TopLeft;
	if (anchor == "TopCenter") return AnchorPoint::TopCenter;
	if (anchor == "TopRight") return AnchorPoint::TopRight;
	if (anchor == "CenterLeft") return AnchorPoint::CenterLeft;
	if (anchor == "Center") return AnchorPoint::Center;
	if (anchor == "CenterRight") return AnchorPoint::CenterRight;
	if (anchor == "BottomLeft") return AnchorPoint::BottomLeft;
	if (anchor == "BottomCenter") return AnchorPoint::BottomCenter;
	if (anchor == "BottomRight") return AnchorPoint::BottomRight;

	return AnchorPoint::TopLeft;
}

void Document::parse_components(tinyxml2::XMLElement* element, std::shared_ptr<Container> parent_container) {
	sf::Vector2f parent_size = parent_container->get_size();

	for (tinyxml2::XMLElement* child = element->FirstChildElement();
		child != nullptr;
		child = child->NextSiblingElement()) {

		if (child->ToComment()) continue;

		const std::string tag = child->Value();
		auto component = create_element(tag, child, parent_container, parent_size);

		if (component) {
			component->set_position(parse_position(child));
			component->set_anchor_point(parse_anchor_point(child));
		}
	}
}

std::shared_ptr<Component> Document::create_element(const std::string& tag,
	tinyxml2::XMLElement* element,
	std::shared_ptr<Container> parent_container,
	const sf::Vector2f& parent_size) {

	if (tag == "button") {
		sf::Vector2f position = parse_position(element);
		sf::Vector2f size = parse_size(element, parent_size);
		sf::Color color = parse_color(element, "color");
		sf::Color hover_color = parse_color(element, "hoverColor");
		sf::Color click_color = parse_color(element, "clickColor");

		const char* label_text = element->Attribute("label");
		if (!label_text) label_text = "Button";

		const unsigned font_size = static_cast<unsigned>(element->IntAttribute("fontSize", 24));

		auto button = std::make_shared<Button>(
			label_text,
			position,
			size,
			color,
			hover_color,
			click_color,
			label_text,
			m_font,
			font_size,
			[]() { std::cout << "Button clicked" << std::endl; }
		);

		if (parent_container) {
			parent_container->add_component(button);
		}
		return button;
	}
	else if (tag == "container") {
		const char* alignment_str = element->Attribute("alignment");
		AlignmentType alignment = AlignmentType::Vertical;
		const char* fit_str = element->Attribute("fit");
		FitType fit = FitType::Default;
		sf::Color bg_color = parse_color(element, "color");

		if (alignment_str) {
			std::string align_str(alignment_str);
			alignment = (align_str == "horizontal") ? AlignmentType::Horizontal : AlignmentType::Vertical;
		}

		if (fit_str) {
			std::string fit_str_temp(fit_str);
			fit = (fit_str_temp == "fit") ? FitType::Fit : FitType::Default;
		}

		auto container = std::make_shared<Container>(
			alignment,
			fit,
			parse_position(element),
			parse_size(element, parent_size),
			bg_color
		);

		parse_components(element, container);

		if (parent_container) {
			parent_container->add_component(container);
		}

		container->arrange_children();

		return container;
	}
	else if (tag == "image") {
		const char* image_path = element->Attribute("path");
		if (!image_path) return nullptr;

		const char* identifier = element->Attribute("id");
		if (!identifier) identifier = "image";

		const int width = element->IntAttribute("width", 64);
		const int height = element->IntAttribute("height", 64);

		auto image = std::make_shared<Image>(
			identifier,
			image_path,
			width,
			height
		);

		if (parent_container) {
			parent_container->add_component(image);
		}
		return image;
	}
	else if (tag == "label") {
		const char* text = element->Attribute("text");
		if (!text) text = "Label";

		const char* identifier = element->Attribute("id");
		if (!identifier) identifier = "label";

		const unsigned font_size = static_cast<unsigned>(element->IntAttribute("fontSize", 24));
		sf::Color color = parse_color(element, "color");

		auto label = std::make_shared<Label>(
			identifier,
			m_font,
			text,
			font_size,
			color
		);

		const AnchorPoint anchor_point = parse_anchor_point(element);
		label->set_anchor_point(anchor_point);

		if (parent_container) {
			parent_container->add_component(label);
		}
		return label;
	}
	else if (tag == "inputField") {
		const char* identifier = element->Attribute("id");
		if (!identifier) identifier = "inputField";

		const sf::Vector2f position = parse_position(element);
		const sf::Vector2f size = parse_size(element, parent_size);
		const unsigned font_size = static_cast<unsigned>(element->IntAttribute("fontSize", 24));
		const sf::Color color = parse_color(element, "color");
		const sf::Color text_color = parse_color(element, "textColor");
		const sf::Color cursor_color = parse_color(element, "cursorColor");
		const std::string placeholder_text = element->Attribute("placeholderText") ? element->Attribute("placeholderText") : "";
		const std::string initial_text = element->Attribute("text") ? element->Attribute("text") : "";

		auto input_field = std::make_shared<InputField>(
			identifier,
			position,
			size,
			m_font,
			font_size,
			color,
			text_color,
			cursor_color,
			placeholder_text,
			initial_text,
			m_screen_size_x,
			m_screen_size_y
		);

		if (parent_container) {
			parent_container->add_component(input_field);
		}
		return input_field;
		}

	return nullptr;
}

sf::Vector2f Document::parse_position(tinyxml2::XMLElement* element) {
	float x = 0.0f, y = 0.0f;
	element->QueryFloatAttribute("x", &x);
	element->QueryFloatAttribute("y", &y);
	return sf::Vector2f(x, y);
}

sf::Vector2f Document::parse_size(tinyxml2::XMLElement* element, const sf::Vector2f& parent_size) {
	float width = 100.0f, height = 50.0f;
	const char* width_attr = element->Attribute("width");
	const char* height_attr = element->Attribute("height");

	if (width_attr) {
		const std::string width_str(width_attr);
		if (width_str == "SCREEN_SIZE_X") {
			width = m_screen_size_x;
		}
		else if (width_str.back() == '%') {
			const float percent = std::stof(width_str.substr(0, width_str.size() - 1));
			width = (percent / 100.0f) * parent_size.x;
		}
		else {
			width = std::stof(width_str);
		}
	}

	if (height_attr) {
		const std::string height_str(height_attr);
		if (height_str == "SCREEN_SIZE_Y") {
			height = m_screen_size_y;
		}
		else if (height_str.back() == '%') {
			const float percent = std::stof(height_str.substr(0, height_str.size() - 1));
			height = (percent / 100.0f) * parent_size.y;
		}
		else {
			height = std::stof(height_str);
		}
	}

	return sf::Vector2f(width, height);
}

sf::Color Document::parse_color(tinyxml2::XMLElement* element, const std::string& attr_name) {
	const char* color_str = element->Attribute(attr_name.c_str());
	if (!color_str) return sf::Color::White;

	int r = 255, g = 255, b = 255, a = 255;
	std::istringstream color_stream(color_str);
	char separator;

	if (!(color_stream >> r >> separator >> g >> separator >> b)) {
		return sf::Color::White;
	}

	if (color_stream >> separator >> a) {}

	return sf::Color(
		static_cast<uint8_t>(r),
		static_cast<uint8_t>(g),
		static_cast<uint8_t>(b),
		static_cast<uint8_t>(a)
	);
}

const std::shared_ptr<Container> Document::get_main_container() const {
	return m_main_container;
}