
## esplay-bootloader 刷入步骤

参考：[ESPlay Micro微型开源游戏掌机（精修版2.0） - 立创开源硬件平台](https://oshwhub.com/LiiGuang/esplay-micro-V2#P6)

- 定位到【固件】文件夹，安装【CH341SER.EXE】驱动
- 运行flash_donwload_tool_v3.8.5下flash_download_tool_v3.8.5.exe
- 点击“Developer Mode”-->“ESP32 DownloadTool”
- 连接 ESPlay Micro 板到计算机USB口，按以下设置参数设置，注意COM口是设备管理器中的 `USB-SERIAL CH340`  ![[download.png]]  ![[ch340.jpg]]
- 地址文字版：
```
phy_init_data.bin=0xf000
esplay-base-firmware.bin=0x10000
partitions.bin=0x8000
bootloader.bin=0x1000
```
- 点击“ERASE”擦除原固件，点击“START”刷入新固件。
- 完成，刷机后会黑屏，拔掉USB数据线，重启设备即可进入 ESPLAY MICRO BOOTLOADER
- 复制`固件`下的`sdcard`文件夹下的所有文件夹到你的Micro SD卡的根目录
- 插入Micro SD卡，重新开机，选择固件并刷入，建议先选择 `v2.2-esplay-micro.fw` 如果没问题再刷其他固件
- ![[bootloader-ui.jpg]]
