# Zen
**Zen**, A mod menu for Growtopia (Android)

---

## ⭐ Screenshots

![Zen Screenshot 1](images/Zen1.jpg)

![Zen Screenshot 2](images/Zen2.jpg)

---

## 📦 How to Build

Building Zen is straightforward, and the following steps will guide you through the process. You can use **Termux** or any **Linux** command line.

### Requirements

Ensure the following are installed on your system:
- **Clang**
- **[Termux-NDK](https://github.com/lzhiyong/termux-ndk)** (for Android development)

### Step-by-Step Guide

**Important**:
   Don't forget to set the location of your Termux-NDK toolchain in this [line](https://github.com/Zaenalos/Zen/blob/main/CMakeLists.txt#L3).

1. **Set up the Build Directory**:
   Create the build directory where the project files will be compiled:
   ```bash
   cd out/build
   ```

2. **Generate the Build Files**:
   Inside the `out/build` directory, generate the build files by running:
   ```bash
   cmake ../..
   ```

3. **Build the Project**:
   Use the following command to build the project with multiple threads.
   ```bash
   cmake --build . --parallel
   ```

4. **Locate the Output**:
   After a successful build, the shared library `libZen.so` will be generated in [this](https://github.com/Zaenalos/Zen/blob/main/CMakeLists.txt#L32) directory (Feel free to modify the output location.).

---

## 💻 Credits

Zen wouldn't be possible without the help of these amazing open-source projects:

- **ShadowHook**: A fast and dynamic Android inline hook library, used in Zen for hooking system functions.  
  [GitHub Repository](https://github.com/bytedance/android-inline-hook)
  
- **ImGui**: A powerful and flexible graphical user interface library, used for creating Zen's mod menu.  
  [GitHub Repository](https://github.com/ocornut/imgui)

- **Termux-NDK**: A toolchain for developing Android native code in Termux, used for building Zen on Android devices.  
  [GitHub Repository](https://github.com/lzhiyong/termux-ndk)

---

## 🫱🏻‍🫲🏿 Contributing

Contributions are welcome! If you have ideas for new features or improvements, feel free to fork the repository and submit a pull request. I have no time to improve and continue this project so it's up to you guys!

---