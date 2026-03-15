@echo off
setlocal enabledelayedexpansion

title OpenAnimator NG - Cross-Platform Update (No Phase Durations)
echo.
echo ================================================
echo   OpenAnimator NG - Cross-Platform Update
echo   Removing phase durations from roadmap
echo   Full open-source support for Windows, macOS, and Linux
echo ================================================
echo.

cd /d D:\openanimator-ng

echo [1/5] Updating README.md with cross-platform instructions ...
(
echo # OpenAnimator NG
echo.
echo **The open raster animation workstation**
echo.
echo Fully open-source tool for raster animation, compositing, and production pipelines.
echo Built for speed and designed to run natively on Windows, macOS, and Linux.
echo.
echo ## Vision
echo - Instant pixel-accurate drawing and flipbook playback
echo - Node-based compositing pipelines
echo - Scenes, sequences, reusable objects, and animation libraries
echo - Python plugin system for automation and extensions
echo - Asset management and marketplace support
echo - **Fully cross-platform: Windows, macOS, Linux** ^(one codebase^)
echo.
echo ## Quick Start ^(All Platforms^)
echo.
echo **Windows**
echo 1. Double-click `bootstrap_vs.bat`
echo 2. Open the generated solution in Visual Studio 2022
echo 3. Build and run `ani.exe`
echo.
echo **macOS and Linux**
echo ```bash
echo mkdir build ^&^& cd build
echo cmake ..
echo cmake --build .
echo ./ani
echo ```
echo.
echo Full roadmap, architecture, and contribution guide below.
echo.
echo Licensed under BSD-3-Clause for the core engine ^+ MIT for new parts.
echo 100%% open source and free for everyone.
) > README.md

echo [2/5] Updating ROADMAP.md without phase durations ...
(
echo # OpenAnimator NG - Roadmap
echo.
echo ## Phase 0 - Foundation
echo - Repository created and branded
echo - Builds and runs on Windows, macOS, and Linux
echo.
echo ## Phase 1 - Core Modernization
echo - True-color canvas support
echo - Unlimited undo/redo with delta storage
echo - Basic layers and masks
echo - Onion skin and animation curves
echo - Cross-platform build system finalized
echo.
echo ## Phase 2 - Node System
echo - Schematic view for piping cells, colors, effects, transforms
echo - Non-destructive compositing
echo.
echo ## Phase 3 - Python Integration
echo - Python plugin loader
echo - First plugins: procedural effects and export tools
echo.
echo ## Phase 4 - Production Pipeline
echo - Scenes, sequences, and object library
echo - Animation library and presets
echo - Asset browser with sounds, images, textures
echo.
echo ## Phase 5 - Ecosystem
echo - Built-in and community plugins
echo - Marketplace for objects, effects, and presets
echo - Export pipelines for video, sprite sheets, and large formats
echo.
echo ## Phase 6 - Polish
echo - Real-time playback optimizations
echo - Community templates for TV intros, cartoons, games, broadcast graphics
echo - Official releases for Windows, macOS, and Linux
) > ROADMAP.md

echo [3/5] Updating ARCHITECTURE.md for cross-platform design ...
(
echo # OpenAnimator NG - Architecture Overview
echo.
echo ## Core Design Principles
echo - Fast raster engine with direct framebuffer access
echo - Modular CEL system for layered animation
echo - Palette engine for color cycling and effects
echo - Timeline engine for flipbook and sequencing
echo.
echo ## Cross-Platform Foundation
echo - Single codebase that compiles natively on Windows, macOS, and Linux
echo - Standard CMake build system
echo - No platform-specific code in the core
echo.
echo ## New Layers ^(added in phases^)
echo - Node graph engine: connect cells → filters → transforms → output
echo - Python API layer: plugins and automation
echo - Asset system: scenes, objects, sequences, libraries
echo - Compositor: real-time layer and effect blending
echo.
echo ## File Format
echo - Single project container ^(.openanimator^)
echo - ZIP-based with JSON metadata + binary raster data
echo.
echo Everything is designed to be open, modular, and run identically on all three platforms.
) > ARCHITECTURE.md

echo [4/5] Updating CONTRIBUTING.md ...
(
echo # Contributing to OpenAnimator NG
echo.
echo Thank you for wanting to help build the open raster animation workstation!
echo.
echo ## How to contribute
echo 1. Fork the repository
echo 2. Create a branch for your feature or fix
echo 3. Test on Windows, macOS, or Linux
echo 4. Follow the existing C style for core changes
echo 5. Add Python plugins in the plugins/ folder
echo 6. Submit a pull request with clear description
echo.
echo We welcome contributions on:
echo - Raster and compositing improvements
echo - Node system enhancements
echo - Python plugin examples
echo - Cross-platform build fixes
echo - Documentation and templates
echo.
echo Open issues and discussions are welcome on any platform.
) > CONTRIBUTING.md

echo [5/5] Committing and pushing the updated files ...
git add README.md ROADMAP.md ARCHITECTURE.md CONTRIBUTING.md
git commit -m "Update roadmap to remove phase durations - full cross-platform open-source support (Windows + macOS + Linux)" --author="OpenAnimator NG Setup <>" 2^>nul || echo (No changes to commit - files may already exist^)
git push origin main

echo.
echo ================================================
echo   Update complete!
echo.
echo   Your repo https://github.com/crazywaters/OpenAnimator-NG
echo   now has:
echo   • Clean roadmap without any time estimates
echo   • Clear open-source cross-platform messaging
echo.
echo   Refresh GitHub to see the changes.
echo   Let me know what to add next ^(Python skeleton, CMake tweaks, etc.^)
echo ================================================
pause