## ESPlay Micro CMake 项目必须用 esp-idf-v4.2 版本

- 不要用 v4.4 编译 ESPlay Micro 的 CMake 项目，否则无法初始化sdcard，并可能导致crach！
- 目前的代码只支持 idf v4.2 而不是最新的 4.4，因为 sdcard sdmmc 和 i2s 驱动程序之间存在 DMA 冲突
- [[编译 esplay-base-firmware (esp-idf-4.2)]]
- 另外，官方的 esplay-retro-emulation 需要使用 esp-idf-3.3 编译：[[编译 esplay-retro-emulation (esp-idf-3.3)]]

参考：
> https://github.com/pebri86/esplay-retro-emulation/issues/30#issuecomment-1152776504
> pebri86 commented on Jun 11, 2022
> Actually i have it in my local repo, but it's very experimental, also the current code only support idf v4.2 not the latest one 4.4 because conflict of DMA between sdcard sdmmc and i2s driver

