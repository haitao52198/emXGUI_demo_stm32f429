关于本项目
------------------
本项目通过git开源
-  github仓库地址：https://github.com/Embdefire/emXGUI_demo_stm32f429

-  gitee 地址：https://gitee.com/wildfireteam/emXGUI_demo_stm32f429

emxGUI综合程序简介
------------------

野火提供的emXGUI综合示例程序，是基于emXGUI图形软件库及FreeRTOS操作系统制作的人机交互界面程序。该程序界面酷炫，性能卓越，非常适合作为嵌入式人机交互界面的参考示例，在基础教程代码中已将野火所有开发板和屏幕适配，目前综合程序已适配表格1‑1中的野火系列开发板。

表格 1‑1 emXGUI综合示例程序适配的开发板

| 开发板                | 适配情况 | 图形库 | 操作系统                 |
|-----------------------|----------|--------|-----------------------|
| F429_挑战者开发板_V1   | 已适配   | emXGUI | RT-Thread，FreeRTOS     |
| F429_挑战者开发板_V2   | 已适配   | emXGUI | RT-Thread，FreeRTOS     |
| H743_Pro开发板        | 已适配   | emXGUI | FreeRTOS               |
| H750_Pro开发板        | 已适配   | emXGUI | FreeRTOS               |
| F103_霸道开发板       | 已适配   | emXGUI  |FreeRTOS               |
| F407_霸天虎开发板     | 正在开发 | emXGUI | FreeRTOS                |
| F767_挑战者开发板_V2  | 正在开发 | emXGUI | FreeRTOS                |
| H743_挑战者开发板_V2 | 正在开发 | emXGUI | FreeRTOS                |
| i.MX RT1052系列开发板 | 正在开发 | emXGUI | FreeRTOS                |

如何运行emXGUI综合程序
----------------------

野火emXGUI综合程序提供开源代码，用户可编译并下载该程序到开发板上运行。

### 所需要的环境

#### 硬件环境

emXGUI综合程序所需要的硬件运行环境如下：

-   本仓库配套野火STM32F429_挑战者开发板_V2

-   5寸液晶屏、5寸液晶屏、或者4.3寸液晶屏

-   DAP下载器

-   SD卡及读卡器

-   OV5640摄像头（可选）

-   耳机（可选）

-   GSM模块（可选）

#### 软件环境

emXGUI综合程序所需要的电脑软件环境主要包括编译器和USB转串口驱动，关于这些具体参考野火的《STM32库开发实战指南》相关的教程，建议先通过该教程掌握STM32开发再进行实验。

-   Keil（5.23版本以上即可），用于编译和下载程序

-   USB CH340转串口驱动，用于接收开发板的串口输出

-   串口调试助手，用于查看程序输出，了解程序运行情况

### emXGUI综合程序资料

找到野火提供的emXGUI综合程序资料，资料中主要包含有各配套开发板的“emXGUI综合示例程序”源代码和“sd卡资源文件”。

### 准备SD卡资源文件

emXGUI示例程序运行需要SD卡资源文件，实验前需要准备一张SD卡，并使用读卡器在电脑上把它格式化成FAT32或FAT格式，然后把emXGUI综合程序资料中的“资源文件”（5寸屏和7寸屏使用同一个资源文件"资源文件_5寸"，4.3寸屏使用资源文件"资源文件_4.3寸"）下的所有内容拷贝至SD卡的根目录，把拷贝好资源的SD卡插入到配套开发板的SD卡槽。

### 准备开发板

请按如下步骤准备开发板：

-   给开发板插入包含“sd资源”的SD卡

-   给开发板接上配套的5寸、7寸或者4.3寸屏幕（5寸、7寸使用同一个工程代码，4.3使用单独的工程代码）

-   使用USB线连接开发板的USB转串口接口，另一端连接至电脑

-   使用DAP下载器连接开发板，另一端连接至电脑

-   打开开发板的电源开关

-   若需要听MP3或视频的声音需要接入耳机、或者接入喇叭

-   若需要使用摄像头或者二维码识别应用需要接入野火OV5640摄像头

-   若需要使用电话或者短信应用需要接入野火GSM模块

### 编译并下载程序

找到资料里配套屏幕和开发板的程序，编译并使用DAP下载器把程序下载到开发板即可，由于emXGUI综合程序非常复杂，且在持续开发中，所以程序编译后可能会提示很多“Warning”，只要编译后没有提示“Error”，忽略即可。

### 第一次运行

emXGUI综合程序在第一次运行时，会要求从SD卡中拷贝各种资源文件的界面。

根据其提示，点击“Click me to load
resources”按钮，等待烧录完成即可，烧录完成后可点击界面的复位按钮或开发板的复位按键复位开发板。

若烧录正常，复位后即可看到程序正式运行时的开机界面和APP应用界面。若出现错误，可使用串口调试助手查看开发板返回的运行日志，串口使用的波特率为115200。
