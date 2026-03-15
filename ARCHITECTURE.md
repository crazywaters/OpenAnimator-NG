# OpenAnimator NG - Architecture Overview

## Core Design Principles
- Fast raster engine with direct framebuffer access
- Modular CEL system for layered animation
- Palette engine for color cycling and effects
- Timeline engine for flipbook and sequencing

## Cross-Platform Foundation
- Single codebase that compiles natively on Windows, macOS, and Linux
- Standard CMake build system
- No platform-specific code in the core

## New Layers (added in phases)
- Node graph engine: connect cells → filters → transforms → output
- Python API layer: plugins and automation
- Asset system: scenes, objects, sequences, libraries
- Compositor: real-time layer and effect blending

## File Format
- Single project container (.openanimator)
- ZIP-based with JSON metadata + binary raster data

Everything is designed to be open, modular, and run identically on all three platforms.
