// ESP32 System
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include <string.h>

// 核心驱动
#include "display.h"
#include "gamepad.h"

char *VERSION = NULL;

void print_version()
{
    const char *VER_PREFIX = "Ver: ";
    size_t ver_size = strlen(VER_PREFIX) + strlen(COMPILEDATE) + 1 + strlen(GITREV) + 1;
    VERSION = malloc(ver_size);
    if (!VERSION)
        abort();

    strcpy(VERSION, VER_PREFIX);
    strcat(VERSION, COMPILEDATE);
    strcat(VERSION, "-");
    strcat(VERSION, GITREV);

    printf("esplay-can-os (%s). HEAP=%#010x\n", VERSION, esp_get_free_heap_size());
}

// 初始化系统
void hardware_init()
{

    // 启动显示屏
    display_prepare();
    display_init();
    set_display_brightness(100);

    // 按键初始化
    gamepad_init();
}

void app_main(void)
{
    print_version();
    hardware_init();

    // 主循环，防止程序退出
    while (1)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
