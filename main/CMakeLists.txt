idf_component_register(SRCS "app_main.cpp" "loopThrottle.cpp" "kalman.cpp" "simulation.cpp" "utilities.cpp" "debug.cpp"
                    PRIV_REQUIRES spi_flash
                    PRIV_REQUIRES esp_timer
                    PRIV_REQUIRES esp_wifi
                    INCLUDE_DIRS ".")
