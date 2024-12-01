
> 这个固件其实就是 bootloader，是一个sd卡固件刷入工具。
![[esplay-base-firmware.jpg]]
## 参考

- [[ESPlay Micro 制作记#固件编译]]
- 原版：[pebri86/esplay-base-firmware: Bootloader or sdcard flasher for ESPlay HW ESP32 based device, based on odroid go base firmware (github.com)](https://github.com/pebri86/esplay-base-firmware)
- 个人修改版：[canwdev/esplay-base-firmware: esplay-base-firmware 修改版](https://github.com/canwdev/esplay-base-firmware)

## 1. 安装 esp-idf-v4.4 环境

- [快速入门 - ESP32 - — ESP-IDF 编程指南 v4.4.4 文档](https://docs.espressif.com/projects/esp-idf/zh_CN/v4.4.4/esp32/get-started/index.html)
- Windows 用户直接下载 [esp-idf-tools-setup-espressif-ide-2.8.1-with-esp-idf-4.4.4.exe](https://dl.espressif.com/dl/idf-installer/esp-idf-tools-setup-espressif-ide-2.8.1-with-esp-idf-4.4.4.exe)

## 2. 执行命令开始编译：

克隆项目到本地：`git clone https://github.com/canwdev/esplay-base-firmware.git`，并进入项目目录

原始方式：
```sh
#get_idf
mkdir build
cd build
cmake ..
make -j4
```

idf 方式：
- 推荐Windows 用户使用这种方式
- Windows 用户需要打开 ESP-IDF 环境的终端才可使用 `idf.py` 命令
```sh
#get_idf
idf.py build
```

编译成功会输出：
```sh
[46/47] Generating binary image from built executable
esptool.py v3.3.2
Creating esp32 image...
Merged 2 ELF sections
Successfully created esp32 image.
Generated D:/Project/dev-hardware/ESPlay/esplay-base-firmware-master/build/esplay_base_firmware.bin
[47/47] cmd.exe /C "cd /D D:\Project\dev-hardware\ESPlay\esplay-bas.../ESPlay/esplay-base-firmware-master/build/esplay_base_firmware.bin"
esplay_base_firmware.bin binary size 0x46220 bytes. Smallest app partition is 0x50000 bytes. 0x9de0 bytes (12%) free.

Project build complete. To flash, run this command:
D:\Espressif\python_env\idf4.4_py3.8_env\Scripts\python.exe ..\..\..\..\Espressif\frameworks\esp-idf-v4.4.4\components\esptool_py\esptool\esptool.py -p (PORT) -b 460800 --before default_reset --after hard_reset --chip esp32  write_flash --flash_mode dio --flash_size detect --flash_freq 80m 0x1000 build\bootloader\bootloader.bin 0x8000 build\partition_table\partition-table.bin 0xd000 build\ota_data_initial.bin 0x10000 build\esplay_base_firmware.bin
or run 'idf.py -p (PORT) flash'
```

此时固件存在于 `build` 下，此时可以继续刷入。
## 3. 刷入

连接设备并开机，Windows 用户打开设备管理器查找 CH340 设备
![[ch340.jpg]]
- Linux 用户刷入
```sh
idf.py -p /dev/ttyUSB0 -b 921600 flash 
```
- Windows 用户刷入，注意 COM 必须和设备管理器中的一致
```cmd
idf.py -p COM10 -b 921600 flash 
```

## 其他

### Linux 刷入权限问题

刷入权限问题：输入以下命令，重启解决（[与 ESP32 创建串口连接](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/get-started/establish-serial-connection.html#linux-dialout-group) ）
- Ubuntu:  `sudo usermod -a -G dialout $USER`
- Arch Linux: `sudo usermod -a -G uucp $USER`

### 编译时报警告

> [!WARNING]
> 编译过程中会因为警告导致编译失败，禁用 make 警告重新编译：

- 搜索 `-Werror=all` 替换为 ` `（空字符串）
- 搜索 `-Werror` 替换为 `-Wno-error`
- 然后再次 `make`

成功编译：
```
[100%] Linking C static library libmain.a
[100%] Built target __idf_main
[100%] Generating ld/sections.ld
[100%] Built target __ldgen_output_sections.ld
[100%] Building C object CMakeFiles/esplay_base_firmware.elf.dir/project_elf_src_esp32.c.obj
[100%] Linking CXX executable esplay_base_firmware.elf
[100%] Built target esplay_base_firmware.elf
[100%] Generating binary image from built executable
esptool.py v3.3.2
Creating esp32 image...
Merged 2 ELF sections
Successfully created esp32 image.
Generated /home/___/esp/esplay-base-firmware/esplay_base_firmware.bin
[100%] Built target gen_project_binary
esplay_base_firmware.bin binary size 0x49570 bytes. Smallest app partition is 0x50000 bytes. 0x6a90 bytes (8%) free.
[100%] Built target app_check_size
[100%] Built target app
[100%] Generating ../../ota_data_initial.bin
[100%] Built target blank_ota_data
```
