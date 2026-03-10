#ifndef FLIP_WEATHER_STORAGE_H
#define FLIP_WEATHER_STORAGE_H

#include <furi.h>
#include <storage/storage.h>
#include <flip_weather.h>

#define SETTINGS_PATH STORAGE_EXT_PATH_PREFIX "/apps_data/flip_weather/settings.bin"

void save_settings(
    const char *ssid,
    const char *password,
    const char *city,
    const char *state,
    const char *country,
    bool use_fahrenheit);

bool load_settings(
    char *ssid,
    size_t ssid_size,
    char *password,
    size_t password_size,
    char *city,
    size_t city_size,
    char *state,
    size_t state_size,
    char *country,
    size_t country_size,
    bool *use_fahrenheit);
#endif // FLIP_WEATHER_STORAGE_H