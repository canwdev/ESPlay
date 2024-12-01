// ESP32 System
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

// 核心驱动
#include "display.h"


// 初始化系统
void canos_init()
{
    printf("Hello world! CanOS! 你好世界！\n");

    // 启动显示屏
    display_prepare();
    display_init();
    set_display_brightness(30);
    printf("Display start success.\n");
    display_show_hourglass();
}

void app_main(void)
{
    canos_init();

    // 主循环，防止程序退出
    while (1)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
