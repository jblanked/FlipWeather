#include "flip_weather.h"

char lat_data[32];
char lon_data[32];

char *total_data = NULL;
char *weather_data = NULL;
bool use_fahrenheit = false;
char custom_city[64] = "";
char custom_state[64] = "";
char custom_country[64] = "";

FlipWeatherApp *app_instance = NULL;
void flip_weather_loader_free_model(View *view);

// Function to free the resources used by FlipWeatherApp
void flip_weather_app_free(FlipWeatherApp *app)
{
    if (!app)
    {
        FURI_LOG_E(TAG, "FlipWeatherApp is NULL");
        return;
    }

    // Free View(s)
    if (app->view_loader)
    {
        view_dispatcher_remove_view(app->view_dispatcher, FlipWeatherViewLoader);
        flip_weather_loader_free_model(app->view_loader);
        view_free(app->view_loader);
    }

    // Free Submenu(s)
    if (app->submenu)
    {
        view_dispatcher_remove_view(app->view_dispatcher, FlipWeatherViewSubmenu);
        submenu_free(app->submenu);
    }

    // Free Widget(s)
    if (app->widget)
    {
        view_dispatcher_remove_view(app->view_dispatcher, FlipWeatherViewAbout);
        widget_free(app->widget);
    }
    if (app->widget_result)
    {
        view_dispatcher_remove_view(app->view_dispatcher, FlipWeatherViewWidgetResult);
        widget_free(app->widget_result);
    }

    // Free Variable Item List(s)
    if (app->variable_item_list)
    {
        view_dispatcher_remove_view(app->view_dispatcher, FlipWeatherViewSettings);
        variable_item_list_free(app->variable_item_list);
    }

    // Free Text Input(s)
    if (app->uart_text_input_ssid)
    {
        view_dispatcher_remove_view(app->view_dispatcher, FlipWeatherViewTextInputSSID);
        uart_text_input_free(app->uart_text_input_ssid);
    }
    if (app->uart_text_input_password)
    {
        view_dispatcher_remove_view(app->view_dispatcher, FlipWeatherViewTextInputPassword);
        uart_text_input_free(app->uart_text_input_password);
    }
    if (app->uart_text_input_city)
    {
        view_dispatcher_remove_view(app->view_dispatcher, FlipWeatherViewTextInputCity);
        uart_text_input_free(app->uart_text_input_city);
    }
    if (app->uart_text_input_state)
    {
        view_dispatcher_remove_view(app->view_dispatcher, FlipWeatherViewTextInputState);
        uart_text_input_free(app->uart_text_input_state);
    }
    if (app->uart_text_input_country)
    {
        view_dispatcher_remove_view(app->view_dispatcher, FlipWeatherViewTextInputCountry);
        uart_text_input_free(app->uart_text_input_country);
    }

    // Free the text input buffer
    if (app->uart_text_input_buffer_ssid)
        free(app->uart_text_input_buffer_ssid);
    if (app->uart_text_input_temp_buffer_ssid)
        free(app->uart_text_input_temp_buffer_ssid);
    if (app->uart_text_input_buffer_password)
        free(app->uart_text_input_buffer_password);
    if (app->uart_text_input_temp_buffer_password)
        free(app->uart_text_input_temp_buffer_password);
    if (app->uart_text_input_buffer_city)
        free(app->uart_text_input_buffer_city);
    if (app->uart_text_input_temp_buffer_city)
        free(app->uart_text_input_temp_buffer_city);
    if (app->uart_text_input_buffer_state)
        free(app->uart_text_input_buffer_state);
    if (app->uart_text_input_temp_buffer_state)
        free(app->uart_text_input_temp_buffer_state);
    if (app->uart_text_input_buffer_country)
        free(app->uart_text_input_buffer_country);
    if (app->uart_text_input_temp_buffer_country)
        free(app->uart_text_input_temp_buffer_country);

    // deinitalize flipper http
    flipper_http_deinit();

    // free the view dispatcher
    if (app->view_dispatcher)
        view_dispatcher_free(app->view_dispatcher);

    // close the gui
    furi_record_close(RECORD_GUI);

    if (total_data)
        free(total_data);

    // free the app
    if (app)
        free(app);
}