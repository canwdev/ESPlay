
> 这个是游戏机固件，支持模拟 NES、GBC 等游戏，还支持音乐播放
> 基于 Make 构建系统，必须用 idf-v3.3

## 参考

- [[ESPlay Micro 制作记#固件编译]]
- [pebri86/esplay-retro-emulation: Retro Emulation Collection for ESPlay Hardware, an ESP32 based game console (github.com)](https://github.com/pebri86/esplay-retro-emulation)
- 个人修改版：[canwdev/esplay-retro-emulation](https://github.com/canwdev/esplay-retro-emulation)

## 1. 安装 esp-idf-v3.3 环境

- [快速入门 — ESP-IDF 编程指南 v3.3 文档 (espressif.com)](https://docs.espressif.com/projects/esp-idf/zh_CN/v3.3/get-started/index.html#id2)
	- [Windows 平台工具链的标准设置 — ESP-IDF 编程指南 v3.3 文档](https://docs.espressif.com/projects/esp-idf/zh-cn/v3.3/get-started/windows-setup.html)
	- [在用户配置文件中添加 IDF_PATH — ESP-IDF 编程指南 v3.3 文档](https://docs.espressif.com/projects/esp-idf/zh-cn/v3.3/get-started/add-idf_path-to-profile.html)
- 配置环境变量 ![[idf3.3env.png]]
- 启动 `mingw32.exe` 输入 `printenv IDF_PATH` 查看环境变量是否设置
- ![[idf3.3env2.png]]
## 2. 克隆项目

```sh
git clone https://github.com/canwdev/esplay-retro-emulation.git
```
- 切换分支 `idf-v3.3`

## 3. 编译

```sh
# 进入项目目录中的 esplay-launcher，改成你的
cd /d/Project/dev-hardware/esplay-retro-emulation/esplay-launcher

# 开始编译，16线程，根据你的CPU核心数量自行设置
make -j16
```

编译成功后会在 build 文件夹生成 LauncherLite.bin
## 4. 刷入

连接设备并开机，Windows 用户打开设备管理器查找 CH340 设备，示例中的端口号为 `COM10`
![[ch340.jpg]]
运行 `make menuconfig` 配置串口的端口号，示例中的端口号为 `COM17`
![[idf3.3串口配置.png]]

```sh
make -j16 flash

# 或直接运行
$IDF_PATH/components/esptool_py/esptool/esptool.py --chip esp32 --port COM17 --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect 0xd000 ./build/ota_data_initial.bin 0x1000 ./build/bootloader/bootloader.bin 0x10000 ./build/LauncherLite.bin 0x8000 ./build/partitions.bin
```

## 5. 编译生成 .fw

- Windows 用户，先打开 ESP-IDF 环境，cd 到项目文件夹，并执行 `mkrelease.bat`
- Linux 用户，执行 `mkrelease.sh`

编译成功后会生成1个几MB 的 `001-esplay-retro-emu.fw` ，这个固件可以复制到 sd 卡的 `esplay/firmware/` 目录，并使用 bootloader 手动刷入。

---
## 旧版教程，请无视

- 需要切换为 esp-idf-v3.3： [快速入门 — ESP-IDF 编程指南 v3.3 文档 (espressif.com)](https://docs.espressif.com/projects/esp-idf/zh_CN/v3.3/get-started/index.html#id2)
- Linux 需要安装工具链：[https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-97-gc752ad5-5.2.0.tar.gz](https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-97-gc752ad5-5.2.0.tar.gz)，并解压到 `~/esp/xtensa-esp32-elf/` 目录
- 需要安装 ffmpeg 并将其放入 PATH（Manjaro 已经自带，无需配置）
- ESP-IDF 支持 Python 3.6 及以上版本
- Linux 用户，需要修改 `mkrelease.sh` 脚本，将环境变量、目录和 mkfw 位置设置正确
- Linux 需要将 `mkrelease.sh` 的 CRLF 改为 LF（使用VSCode）

设置工具链的环境变量：
```
export IDF_PATH=~/esp/esp-idf
export PATH="$HOME/esp/xtensa-esp32-elf/bin:$PATH"
```

配置好环境后，按照 esplay-retro-emulation 的文档，运行 `mkrelease.sh` 进行编译，如果项目目录下得到一个几KB的 `esplay-retro-emu.fw`，那么恭喜你编译失败了，此时需要一行行地执行脚本查找错误。

```
# 首先，设置环境变量
export IDF_PATH=~/esp/esp-idf
export ESPLAY_SDK=~/esp/esplay-retro-emulation/esplay-sdk

# 一行行地执行下面代码，观察报错，每次make成功后会在相应 build 目录下生成所需的 .bin 文件

# 说明：Esplay 启动器, new code base using UGUI library
cd esplay-launcher
make -j4

# 说明：GNUBoy for play GameBoy and GameBoy Color
cd ../esplay-gnuboy
make -j4

# 说明：Nofrendo NES Emulator for play Nintendo ROM
cd ../esplay-nofrendo
make -j4

# 说明：SMSPlusGX could play Sega Master System, Game Gear and Coleco Vision ROM
cd ../esplay-smsplusgx
make -j4

cd ..
# 运行剩余的命令生成约 3.1MB 的 esplay-retro-emu.fw 文件
```

记录一下我遇到的几个坑

- Python 依赖未安装
	- 通过控制台的提示，安装依赖：`/usr/bin/python -m pip install --user -r /home/___/esp/esp-idf/requirements.txt`
- 编译 esplay-launcher 提示 gamepad.h: No such file or directory
	- 设置环境变量 IDF_PATH, ESPLAY_SDK
- 在 `esplay-launcher/main/graphics.c` 中找不到文件 `gfxTile.inc` 
	- 手动将 `gfxTile.png` 转换为 `gfxTile.inc`
	- 或直接使用这个项目的文件 [esplay-retro-emulation/gfxTile.inc at master · FantasyGmm/esplay-retro-emulation (github.com)](https://github.com/FantasyGmm/esplay-retro-emulation/blob/master/esplay-launcher/main/gfxTile.inc) 
- `assets/Tile.png` 不存在
	- 需要手动将 `assets/Tile.xcf` 转换成 png 格式，这个图像将作为 rom 的封面
