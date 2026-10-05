# lookcpu

Linux hardware info tool in C/C++. Only 820KB memory. Zero dependencies.

linux系统的硬件检测工具，C/C++编写，只有820KB内存占用，无任何依赖。

## Why lookcpu? / 为什么做 lookcpu?

For people who have old computers at home (typically 15 years old), the only viable path to installing a modern operating system is to use a lightweight Linux distribution. Since it's inconvenient to disassemble an old machine, the best approach is to use an extremely lightweight tool to detect hardware information — and that's exactly what this tool is for: detecting hardware on old computers. However, for computers with only 512 MB of RAM, many hardware detection tools available on the market struggle to run. So I simply wrote one in C/C++. This tool has been tested on a 2009 Celeron laptop, where it runs perfectly and outputs information without any issues. Although I didn't aim to identify some peripherals, for the hard-to-notice components — such as CPU, memory, disk, and motherboard — this tool can output information.

Of course, if you're using a new computer (say, bought within the last 3 years), this tool will still give you an extremely smooth experience.

对于家里有老电脑(一般是15年前的电脑)的人，想要装上现代操作系统，唯一可选的路就是装上轻量化的linux系统。老电脑拆机不方便，所以用一个极致轻量化的工具来检测硬件信息是最好的选择，而这个就是检测老电脑硬件的工具。但是对于只有512MB内存的电脑，世面上很多硬件信息检测工具都很难运行，于是我就干脆用C/C++写了一个这样的工具。这个工具在2009年的赛扬老电脑测试过，完全可以正常运行并输出信息。虽然一些外设，我没有想着去识别，但对于难以察觉的部分，比如CPU、内存、硬盘、主板等，这个工具都可以输出信息。

当然，如果你用的是新电脑（比如最近3年买的），这个工具依然会给你一个极致流畅的体验。

## Features / 功能


- CPU information (vendor, model, cores)
- Memory usage and module details (size, type, speed)
- Disk and partition information
- GPU information
- Motherboard and BIOS information
- Network interfaces
- Battery and power status
- Operating system information

- CPU信息(厂商、型号、核心数)
- 内存使用情况和模块详情(大小、类型、速度)
- 硬盘和分区信息
- GPU信息
- 主板和BIOS信息
- 网络接口
- 电池和电源状态
- 操作系统信息

## Usage && Buils / 使用方法 && 编译

```bash

git clone https://github.com/hehechengchengaaa/lookcpu.git

cd lookcpu

mkdir build

cd build

cmake ..

make

./lookcpu

sudo ./lookcpu help

sudo ./lookcpu all

```

## License / 许可证

MIT License. See [LICENSE](LICENSE) for details.

## Support / 赞助

If you find lookcpu useful, consider supporting its development.  
如果 lookcpu 对你有帮助，欢迎支持它的开发。

Your support helps me keep this project alive while I'm a student.  
你的支持帮助我在学生阶段维持这个项目。

Your support helps cover:
- Testing on old hardware
- Domain and hosting costs
- Development time

你的支持将用于：
- 在老硬件上测试
- 域名和托管费用
- 开发时间

Donate:

https://opencollective.com/lookcpu

https://afdian.com/a/hehechengchengaaa




