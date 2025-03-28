<h1 align="center">
cs2-sdk
<img src="https://img.shields.io/badge/game-CS2-yellow" />
<img src="https://img.shields.io/badge/language-C%2B%2B-%23f34b7d.svg" />
<img src="https://img.shields.io/badge/platform-Windows-blue" />
<img src="https://img.shields.io/badge/platform-Linux-purple" />
</h1>

<h2 align="center">
<strong>EDUCATIONAL PURPOSES ONLY</strong>
</h2>

## What's this?
Counter-Strike 2 SDK written in C++ to aid your development. This SDK has been written as simplified as possible and kept as close as possible to Source 2 code supporting both DirectX11 and Vulkan graphical APIs. I'll keep the project updated if I have time, project's features can be seen in the picture below.

## What's different from v1?
Now supports both Windows and Linux, main base logic has been refactored. I've gotten rid of the 'Inventory Changer' and skins altogether since I don't have the time to maintain them anymore, v1 can be found [here](https://github.com/bruhmoment21/cs2-sdk/tree/v1) and won't receive any updates from now on.

## Injection
*(⚠️ This also applies for **v1**.)*  
*(⚠️ **Manual mapping** will break unloading.)*

### Windows
- ✅ LoadLibrary - Works as expected.
- ⚠️ Manual map - `/Zc:threadSafeInit-` flag is required. C/C++ -> Command Line -> Additional Options.
### Linux
- ✅ dlopen - Works as expected. 

## Detection ❗
*(⚠️ This also applies for **v1**.)*

The base should be **DETECTED** since the base uses generic approaches such as *trampoline hooking*. Hence the detection I recommend just seeing how stuff is done/works to try to implement it in your own project. Be creative!

## Building
### Windows
- Open `cs2-sdk.sln` with Visual Studio 2022 and Build Solution or `Ctrl+Shift+B`. Select the `Release` configuration if you don't plan on debugging and also [disable logging](https://github.com/bruhmoment21/cs2-sdk/blob/v2/cs2-sdk/include/logger/logger.hpp#L3-L4) if not necessary.
### Linux
- Create a binary folder and use CMake after going in the directory, what's left is to build the library. Here's what it should look like:  
`mkdir binlinux && cd binlinux && cmake ../ && make`

## Media
![image](https://github.com/bruhmoment21/cs2-sdk/assets/53657322/d19eeea9-3309-4fa2-804c-ca5b7e84f319)
![image](https://github.com/user-attachments/assets/2a4a1dd9-88dc-4dc6-a70d-c332c3f1f32d)

## Dependencies
- [funchook](https://github.com/kubo/funchook) - hooking
- [distorm](https://github.com/gdabah/distorm/) - disassembler
- [imgui](https://github.com/ocornut/imgui) - gui
- [STB](https://github.com/cristeigabriel/STB) - signatures
### Windows
- [Vulkan](https://vulkan.lunarg.com/) - [vulkan_hooks.cpp](https://github.com/bruhmoment21/cs2-sdk/blob/v2/cs2-sdk/src/hooks/render/vulkan/vulkan_hooks.cpp#L6-L9)
### Linux
- [SDL3](https://archlinux.org/packages/extra/x86_64/sdl3/) - input
- [Vulkan](https://archlinux.org/packages/extra/any/vulkan-headers/) - rendering
