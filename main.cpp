#include "XML/document.hpp"
#include <iostream>

constexpr float SCREEN_WIDTH = 1080.0f;
constexpr float SCREEN_HEIGHT = 720.0f;

std::shared_ptr<thd::Component> find_component_recursive(const std::shared_ptr<thd::Container>& container, const std::string& id) {
	auto component = container->get_component(id);
	if (component) {
		return component;
	}

	for (const auto& sub_component : container->get_components()) {
		auto sub_container = std::dynamic_pointer_cast<thd::Container>(sub_component);
		if (sub_container) {
			component = find_component_recursive(sub_container, id);
			if (component) {
				return component;
			}
		}
	}
	return nullptr;
}

int main() {
	auto window = std::make_unique<sf::RenderWindow>(
		sf::VideoMode(static_cast<unsigned>(SCREEN_WIDTH), static_cast<unsigned>(SCREEN_HEIGHT)),
		"Thorned",
		sf::Style::Resize | sf::Style::Close | sf::Style::Titlebar
	);

	window->setFramerateLimit(60);
	 
	thd::Document doc("Assets/example/page.xml", SCREEN_WIDTH, SCREEN_HEIGHT, "Assets/hHachimaki.ttf");
	doc.load("Assets/example/page.xml");

	auto main_container = doc.get_main_container();
	if (!main_container) {
		std::cerr << "Failed to load main container" << std::endl;
		return -1;
	}

	auto name_input = find_component_recursive(main_container, "name");
	auto output = find_component_recursive(main_container, "output");

	while (window->isOpen()) {
		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window->close();
			}

			for (const auto& component : main_container->get_components()) {
				component->handle_event(event, *window);
			}   
		}

		output->set_label_text(name_input->get_label()->getString().toAnsiString());
		for (const auto& component : main_container->get_components()) {
			component->update(0.f, *window);
		}

		window->clear(sf::Color(30, 30, 30));
		main_container->render(*window);
		window->display();
	}

	return 0;
}
