#include "Core/Application.hpp"
#include "AppLayer.hpp"

int main()
{
	Core::ApplicationConfig app_config;
	app_config.name = "Visual Computing";
	app_config.window_config.width = 1920;
	app_config.window_config.height = 1080;
	app_config.window_config.title = "Visual Computing";

	Core::Application application(app_config);
	application.push_layer<AppLayer>();
	application.run();

}