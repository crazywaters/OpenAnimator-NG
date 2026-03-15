# OpenAnimator NG

**The open raster animation workstation**

Fully open-source tool for raster animation, compositing, and production pipelines.
Built for speed and designed to run natively on Windows, macOS, and Linux.

## Vision
- Instant pixel-accurate drawing and flipbook playback
- Node-based compositing pipelines
- Scenes, sequences, reusable objects, and animation libraries
- Python plugin system for automation and extensions
- Asset management and marketplace support
- **Fully cross-platform: Windows, macOS, Linux** (one codebase)

## Quick Start (All Platforms)

**Windows**
1. Double-click `bootstrap_vs.bat`
2. Open the generated solution in Visual Studio 2022
3. Build and run `ani.exe`

**macOS and Linux**
```bash
mkdir build && cd build
cmake ..
cmake --build .
./ani
```

Full roadmap, architecture, and contribution guide below.

Licensed under BSD-3-Clause for the core engine + MIT for new parts.
100% open source and free for everyone.
