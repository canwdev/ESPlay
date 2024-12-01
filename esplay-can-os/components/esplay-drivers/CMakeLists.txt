idf_component_register(
    SRC_DIRS "common/src" "display/src"
    INCLUDE_DIRS "common/include" "display/include"
    REQUIRES sdmmc vfs fatfs nvs_flash esp_adc_cal app_update spiffs
)