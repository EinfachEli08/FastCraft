
# FastCraft

FastCraft is a personal project where I, EinfachEli08, am recreating **Minecraft: Java Edition** in C++ line by line, version for version as closely as possible. In addition, I aim to build an **open-source Minecraft like game engine**, enabling others to experiment, learn, and create their own Minecraft-inspired projects.

**Important Note:** This project does not include Minecraft's original assets (textures, sounds, etc.). You must **provide your own assets** to comply with Mojang's guidelines and copyright laws.

---

## Features
- Aiming for a 1:1 recreation of Minecraft: Java Edition in C++.
- Open-source and modular: designed to serve as a game engine for Minecraft-inspired projects.
- Built using modern libraries for performance and cross-platform support.

---

## Libraries Used
FastCraft uses the following libraries to implement core functionality:

1. **GLAD** - OpenGL Loader.
2. **GLFW** - Windowing and input handling.
3. **STB_IMAGE** - Image loading.
4. **KHRPLATFORM** - OpenGL extensions platform.
5. **LZ** - Lightweight compression/decompression.

---

## Build Instructions

### Requirements
- **MSYS2**: The project is built and tested with MSYS2. Make sure it's installed and up-to-date.
- **C++17** (or later) compiler.
- **Git**: To clone the repository.

### Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/EinfachEli08/FastCraft.git
   cd FastCraft
   ```
2. Install dependencies:
   Ensure that the above libraries are installed. You can follow the MSYS2 package manager to install relevant packages or compile them yourself.
3. Build the project:
   1. CD into /bin
   2. Run the following command:
   ```bash
   make
   ```

---

## Important Notes
1. **Assets**: You must provide your own assets (textures, sounds, etc.). Place them in the `assets/` directory. This project does not and will never include copyrighted materials.
2. **License**: The project is open-source, but is allowed to be used commercially by anyone
3. **Development**: The project is in an early stage and is not yet a fully functional Minecraft line by line clone. Features are added progressively and bugs may occur.

---

## Contributions
Contributions are welcome! Whether it's optimizing code, fixing bugs, or adding features, feel free to submit pull requests. Ensure your code adheres to Minecrafts source style works as expected.

---

## Disclaimer
FastCraft is a fan project and is not affiliated with Mojang, Microsoft, or Minecraft. It is created as a personal coding challenge.

---

## Contact
For questions or discussions, feel free to reach out via GitHub issues or pull requests. Thank you for your interest in FastCraft!
