 # 开发中，请勿使用

- Windows 用户直接下载 [esp-idf-tools-setup-espressif-ide-2.8.1-with-esp-idf-4.4.4.exe](https://dl.espressif.com/dl/idf-installer/esp-idf-tools-setup-espressif-ide-2.8.1-with-esp-idf-4.4.4.exe)

```cmd
#idf.py clean
#idf.py menuconfig
idf.py build
idf.py -p COM10 -b 921600 flash
```

