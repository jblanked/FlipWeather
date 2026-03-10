#include <alloc/flip_weather_alloc.h>

// Function to allocate resources for the FlipWeatherApp
FlipWeatherApp *flip_weather_app_alloc()
{
    FlipWeatherApp *app = (FlipWeatherApp *)malloc(sizeof(FlipWeatherApp));

    Gui *gui = furi_record_open(RECORD_GUI);

    // initialize uart
    if (!flipper_http_init(flipper_http_rx_callback, app))
    {
        FURI_LOG_E(TAG, "Failed to initialize flipper http");
        return NULL;
    }

    // Allocate the text input buffer
    app->uart_text_input_buffer_size_ssid = 64;
    app->uart_text_input_buffer_size_password = 64;
    app->uart_text_input_buffer_size_city = 64;
    app->uart_text_input_buffer_size_state = 64;
    app->uart_text_input_buffer_size_country = 64;
    if (!easy_flipper_set_buffer(&app->uart_text_input_buffer_ssid, app->uart_text_input_buffer_size_ssid))
    {
        return NULL;
    }
    if (!easy_flipper_set_buffer(&app->uart_text_input_temp_buffer_ssid, app->uart_text_input_buffer_size_ssid))
    {
        return NULL;
    }
    if (!easy_flipper_set_buffer(&app->uart_text_input_buffer_password, app->uart_text_input_buffer_size_password))
    {
        return NULL;
    }
    if (!easy_flipper_set_buffer(&app->uart_text_input_temp_buffer_password, app->uart_text_input_buffer_size_password))
    {
        return NULL;
    }
    if (!easy_flipper_set_buffer(&app->uart_text_input_buffer_city, app->uart_text_input_buffer_size_city))
    {
        return NULL;
    }
    if (!easy_flipper_set_buffer(&app->uart_text_input_temp_buffer_city, app->uart_text_input_buffer_size_city))
    {
        return NULL;
    }
    if (!easy_flipper_set_buffer(&app->uart_text_input_buffer_state, app->uart_text_input_buffer_size_state))
    {
        return NULL;
    }
    if (!easy_flipper_set_buffer(&app->uart_text_input_temp_buffer_state, app->uart_text_input_buffer_size_state))
    {
        return NULL;
    }
    if (!easy_flipper_set_buffer(&app->uart_text_input_buffer_country, app->uart_text_input_buffer_size_country))
    {
        return NULL;
    }
    if (!easy_flipper_set_buffer(&app->uart_text_input_temp_buffer_country, app->uart_text_input_buffer_size_country))
    {
        return NULL;
    }

    // Allocate ViewDispatcher
    if (!easy_flipper_set_view_dispatcher(&app->view_dispatcher, gui, app))
    {
        return NULL;
    }
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, flip_weather_custom_event_callback);
    // Main view
    if (!easy_flipper_set_view(&app->view_loader, FlipWeatherViewLoader, flip_weather_loader_draw_callback, NULL, callback_to_submenu, &app->view_dispatcher, app))
    {
        return NULL;
    }
    flip_weather_loader_init(app->view_loader);

    // Widget
    if (!easy_flipper_set_widget(&app->widget, FlipWeatherViewAbout, "FlipWeather v1.3\n-----\nUse WiFi to get GPS and \nWeather information.\n-----\nwww.github.com/jblanked", callback_to_submenu, &app->view_dispatcher))
    {
        return NULL;
    }
    if (!easy_flipper_set_widget(&app->widget_result, FlipWeatherViewWidgetResult, "Error, try again.", callback_to_submenu, &app->view_dispatcher))
    {
        return NULL;
    }

    // Text Input
    if (!easy_flipper_set_uart_text_input(&app->uart_text_input_ssid, FlipWeatherViewTextInputSSID, "Enter SSID", app->uart_text_input_temp_buffer_ssid, app->uart_text_input_buffer_size_ssid, text_updated_ssid, callback_to_wifi_settings, &app->view_dispatcher, app))
    {
        return NULL;
    }
    if (!easy_flipper_set_uart_text_input(&app->uart_text_input_password, FlipWeatherViewTextInputPassword, "Enter Password", app->uart_text_input_temp_buffer_password, app->uart_text_input_buffer_size_password, text_updated_password, callback_to_wifi_settings, &app->view_dispatcher, app))
    {
        return NULL;
    }
    if (!easy_flipper_set_uart_text_input(&app->uart_text_input_city, FlipWeatherViewTextInputCity, "Enter City", app->uart_text_input_temp_buffer_city, app->uart_text_input_buffer_size_city, text_updated_city, callback_to_wifi_settings, &app->view_dispatcher, app))
    {
        return NULL;
    }
    if (!easy_flipper_set_uart_text_input(&app->uart_text_input_state, FlipWeatherViewTextInputState, "Enter State/Region", app->uart_text_input_temp_buffer_state, app->uart_text_input_buffer_size_state, text_updated_state, callback_to_wifi_settings, &app->view_dispatcher, app))
    {
        return NULL;
    }
    if (!easy_flipper_set_uart_text_input(&app->uart_text_input_country, FlipWeatherViewTextInputCountry, "Enter Country", app->uart_text_input_temp_buffer_country, app->uart_text_input_buffer_size_country, text_updated_country, callback_to_wifi_settings, &app->view_dispatcher, app))
    {
        return NULL;
    }

    // Variable Item List
    if (!easy_flipper_set_variable_item_list(&app->variable_item_list, FlipWeatherViewSettings, settings_item_selected, callback_to_submenu, &app->view_dispatcher, app))
    {
        return NULL;
    }
    app->variable_item_ssid = variable_item_list_add(app->variable_item_list, "SSID", 0, NULL, NULL);
    app->variable_item_password = variable_item_list_add(app->variable_item_list, "Password", 0, NULL, NULL);
    app->variable_item_city = variable_item_list_add(app->variable_item_list, "City", 0, NULL, NULL);
    app->variable_item_state = variable_item_list_add(app->variable_item_list, "State", 0, NULL, NULL);
    app->variable_item_country = variable_item_list_add(app->variable_item_list, "Country", 0, NULL, NULL);
    app->variable_item_temperature_unit = variable_item_list_add(app->variable_item_list, "Temperature", 2, temperature_unit_change, app);
    variable_item_set_current_value_text(app->variable_item_ssid, "");
    variable_item_set_current_value_text(app->variable_item_password, "");
    variable_item_set_current_value_text(app->variable_item_city, "");
    variable_item_set_current_value_text(app->variable_item_state, "");
    variable_item_set_current_value_text(app->variable_item_country, "");
    variable_item_set_current_value_index(app->variable_item_temperature_unit, 0);
    variable_item_set_current_value_text(app->variable_item_temperature_unit, "Celsius");

    // Submenu
    if (!easy_flipper_set_submenu(&app->submenu, FlipWeatherViewSubmenu, "FlipWeather v1.3", callback_exit_app, &app->view_dispatcher))
    {
        return NULL;
    }
    submenu_add_item(app->submenu, "Weather", FlipWeatherSubmenuIndexWeather, callback_submenu_choices, app);
    submenu_add_item(app->submenu, "GPS", FlipWeatherSubmenuIndexGPS, callback_submenu_choices, app);
    submenu_add_item(app->submenu, "About", FlipWeatherSubmenuIndexAbout, callback_submenu_choices, app);
    submenu_add_item(app->submenu, "Settings", FlipWeatherSubmenuIndexSettings, callback_submenu_choices, app);

    // load settings
    if (load_settings(
            app->uart_text_input_buffer_ssid, app->uart_text_input_buffer_size_ssid,
            app->uart_text_input_buffer_password, app->uart_text_input_buffer_size_password,
            app->uart_text_input_buffer_city, app->uart_text_input_buffer_size_city,
            app->uart_text_input_buffer_state, app->uart_text_input_buffer_size_state,
            app->uart_text_input_buffer_country, app->uart_text_input_buffer_size_country,
            &use_fahrenheit))
    {
        // Update variable items
        if (app->variable_item_ssid)
            variable_item_set_current_value_text(app->variable_item_ssid, app->uart_text_input_buffer_ssid);
        // dont show password

        // Copy items into their temp buffers with safety checks
        if (app->uart_text_input_buffer_ssid && app->uart_text_input_temp_buffer_ssid)
        {
            strncpy(app->uart_text_input_temp_buffer_ssid, app->uart_text_input_buffer_ssid, app->uart_text_input_buffer_size_ssid - 1);
            app->uart_text_input_temp_buffer_ssid[app->uart_text_input_buffer_size_ssid - 1] = '\0';
        }
        if (app->uart_text_input_buffer_password && app->uart_text_input_temp_buffer_password)
        {
            strncpy(app->uart_text_input_temp_buffer_password, app->uart_text_input_buffer_password, app->uart_text_input_buffer_size_password - 1);
            app->uart_text_input_temp_buffer_password[app->uart_text_input_buffer_size_password - 1] = '\0';
        }
        if (app->uart_text_input_buffer_city && app->uart_text_input_temp_buffer_city)
        {
            strncpy(app->uart_text_input_temp_buffer_city, app->uart_text_input_buffer_city, app->uart_text_input_buffer_size_city - 1);
            app->uart_text_input_temp_buffer_city[app->uart_text_input_buffer_size_city - 1] = '\0';
            strncpy(custom_city, app->uart_text_input_buffer_city, sizeof(custom_city) - 1);
            custom_city[sizeof(custom_city) - 1] = '\0';
            if (app->variable_item_city)
                variable_item_set_current_value_text(app->variable_item_city, app->uart_text_input_buffer_city);
        }
        if (app->uart_text_input_buffer_state && app->uart_text_input_temp_buffer_state)
        {
            strncpy(app->uart_text_input_temp_buffer_state, app->uart_text_input_buffer_state, app->uart_text_input_buffer_size_state - 1);
            app->uart_text_input_temp_buffer_state[app->uart_text_input_buffer_size_state - 1] = '\0';
            strncpy(custom_state, app->uart_text_input_buffer_state, sizeof(custom_state) - 1);
            custom_state[sizeof(custom_state) - 1] = '\0';
            if (app->variable_item_state)
                variable_item_set_current_value_text(app->variable_item_state, app->uart_text_input_buffer_state);
        }
        if (app->uart_text_input_buffer_country && app->uart_text_input_temp_buffer_country)
        {
            strncpy(app->uart_text_input_temp_buffer_country, app->uart_text_input_buffer_country, app->uart_text_input_buffer_size_country - 1);
            app->uart_text_input_temp_buffer_country[app->uart_text_input_buffer_size_country - 1] = '\0';
            strncpy(custom_country, app->uart_text_input_buffer_country, sizeof(custom_country) - 1);
            custom_country[sizeof(custom_country) - 1] = '\0';
            if (app->variable_item_country)
                variable_item_set_current_value_text(app->variable_item_country, app->uart_text_input_buffer_country);
        }

        // Apply loaded temperature unit
        if (app->variable_item_temperature_unit)
        {
            variable_item_set_current_value_index(app->variable_item_temperature_unit, use_fahrenheit ? 1 : 0);
            variable_item_set_current_value_text(app->variable_item_temperature_unit, use_fahrenheit ? "Fahrenheit" : "Celsius");
        }
    }

    // Switch to the main view
    view_dispatcher_switch_to_view(app->view_dispatcher, FlipWeatherViewSubmenu);

    return app;
}