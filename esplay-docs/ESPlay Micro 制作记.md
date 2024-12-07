> 建议用 Obsidian 打开 `esplay-docs` 文件夹查看此文档，否则图片可能展示不正常。
## 参考

![[esplay_LiiGuang.jpg]]

- [ESPlay Micro微型开源游戏掌机（精修版2.0） - 嘉立创EDA开源硬件平台 (oshwhub.com)](https://oshwhub.com/LiiGuang/esplay-micro-V2#P6)
	- 推荐使用此资料制作，以下内容全部根据此资料编写
- [开源掌机 ESPLAY V0.3 游戏机 - 嘉立创EDA开源硬件平台 (oshwhub.com)](https://oshwhub.com/zxp1107/esplay_copy_copy_copy#P6)

## 购买原材料

- 主板
	- [用【嘉立创下单助手】PC客户端领优惠券](https://www.bilibili.com/opus/775210888117354521)，每个月可以领1张PCB板20元免费券
	- 目前加入了考试系统，必须通过考试（60分以上）才可领券，多考几次记住答案就能过
	- 免费打样5个PCB板即可，具体下单流程自行搜索教程
- 元器件（BOM）
	- 查看本项目的【资料】文件夹，包含BOM修订版，根据内容购买元件即可
	- 可以在嘉立创商城下单大部分电容、电阻、按键、芯片等元件
	- ESP32推荐购买 `ESP32-WROVER-IE` 或 `ESP32-WROVER-E` 型号，内存越大越好反正也不贵
		- IE 是外置天线，E 是内置天线。
	- 部分找不到的芯片可以在淘宝搜索购买
- 外壳
	- 无需外壳，用M3铜柱和螺母固定
	- 3D打印外壳：TODO
## 经验总结

- 对于贴片原件，可使用锡膏+热风枪来快速焊接
- 对于 ESP32 Wrover 主控，不要使用锡膏的方式，容易出现芯片底部连锡，或者虚焊，应该用电烙铁
- 对于开关、按钮等原件，应该用电烙铁，如果使用热风枪加热可能导致原件烫坏
- 使用万用表确认电路连接正确，以及检测连锡导致的短路
- Type-C 引脚密集，可以在PCB板子触点上涂上【少量】锡膏，然后再用热风枪加热管脚，如果连锡则需要用热风枪拆下重新焊接
- 常规的焊接技巧参考这个系列视频：[【13节一分钟焊接小技巧系列】](https://www.bilibili.com/video/BV1wJ411B73v)
- 电池的焊接需要小心，因为焊接时若不小心造成正负极短路可能造成火灾，建议焊接电池插座
- JP1 跳线直接一坨锡，并且在刷机结束后不需要移除
- ESD芯片应该使用0603尺寸，而我用的是0402，这么小的尺寸非常难以焊接（经测试，不焊接也不会有任何影响）
- 屏幕排线的FPC连接器不需要购买，在触点上少量锡，对齐屏幕后再用电烙铁焊接

## 制作过程记录

- ESP32主控芯片连锡
	- 应该使用助焊剂和电烙铁加热，这样锡融化后会自动紧贴引脚
	- 如果锡太多，可以使用吸锡带和吸锡器
	- 如果锡太少，可以使用锡膏+电烙铁融化
	- 如果无论如何都无法解决短路，可加少量锡膏和助焊剂，再用电烙铁加热，新加热的锡会带动旧锡融化
- 开机白屏，无内容显示
	- ESP32主控底部有残留的锡膏导致短路
	- 用万用表测出短路的引脚，加上助焊剂，使用电烙铁加热引脚解决短路
- 无法读取SD卡、喇叭输出没声音、耳机只有右边有声音
	- 通过查看电路图和用万用表测试没有发现问题
	- 猜测是SD卡座的问题，拆除卡座用新的替换就解决了这个问题
	- 由于是第一次焊，可能烫坏了某些原件，重做了一版解决
- 开机后屏幕花屏，并一致闪烁不显示内容
	- 原因：系统尝试读取SD卡的保存状态，但是读取失败
	- 插入SD卡或重新刷入固件即可修复
- 固件不支持中文
	- 使用另一个项目【[开源掌机 ESPLAY V0.3 游戏机](https://oshwhub.com/zxp1107/esplay_copy_copy_copy)】评论提到的固件【[ESP32游戏机](https://oshwhub.com/ywqprogram/esp32_game_copy)】刷入，白屏，此固件不支持。
	- 使用 [DIY掌机，自制ESP32游戏机，超详细教程，全是干货，看完你也会做_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1H3411T7x5/) 中的资料里面的rom，成功！
	- 魔改 [esplay: 将github上的esplay项目继续完善](https://gitee.com/yvany/esplay/tree/master) 也可以实现中文展示
	- 我在上面仓库的基础上修改了一个版本，支持大多数功能：[canwdev/esplay-retro-emulation at idf-v3.3 (github.com)](https://github.com/canwdev/esplay-retro-emulation/tree/idf-v3.3)
- 外放喇叭游戏有声音，音乐播放器没声音
	- 原因：我使用的20211004版本LR肩键物理层有bug，导致L键无法触发
	- 通过修改系统代码使用Menu键开启了外放功能
- 外放喇叭有底噪
	- 不知道是硬件设计问题还是电容问题

## 固件编译

以下项目，我使用 ArchLinux（Manjaro）于 20221113 编译成功。

- 如果使用 Linux 则不需要下载上述工具链，按照教程安装环境即可
- ESP-IDF 下载：
	- 编译 esplay-retro-emulation 需要 esp-idf-v3.3 版本 https://dl.espressif.com/dl/esp-idf/releases/esp-idf-v3.3.zip
		- 如果使用 Windows，ESP-IDF 3.3工具链下载： [https://dl.espressif.com/dl/esp32_win32_msys2_environment_and_toolchain-20181001.zip](https://dl.espressif.com/dl/esp32_win32_msys2_environment_and_toolchain-20181001.zip)
	- 编译 esplay-base-firmware 需要 esp-idf-v4.4 版本 https://dl.espressif.com/github_assets/espressif/esp-idf/releases/download/v4.4.3/esp-idf-v4.4.3.zip

- [[编译 esplay-base-firmware (esp-idf-4.4)]]
- [[编译 esplay-retro-emulation (esp-idf-3.3)]]

### 其他

- ESP32 WROVER IE / ESP32 WROVER E 的区别是：IE是外置天线，E是内置天线。通过文档 [2201121630_Espressif-Systems-ESP32-WROVER-IE-8MB_C2934565.pdf (lcsc.com)](https://datasheet.lcsc.com/lcsc/2201121630_Espressif-Systems-ESP32-WROVER-IE-8MB_C2934565.pdf) 可以得知。
- [ESP32选型一文就够，ESP32-WROOM-32、ESP32-WROVER、ESP32-S衍生模组、ESP32-PICO差异 - 哔哩哔哩 (bilibili.com)](https://www.bilibili.com/read/cv15539489)