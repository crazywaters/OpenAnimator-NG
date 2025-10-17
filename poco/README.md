# Poco subproject

## Building standalone

From the repository root:

```
pixi run cmake -B _build -S . -G Ninja -DWITH_ANI=0 -DWITH_POCO=1
pixi run cmake --build _build --config debug --target install
```

This builds `libpoco` and the `poco` CLI in `_build/poco` and installs to `_install/`.

## Using as a subproject

`CMakeLists.txt` at the root adds this subproject when `WITH_POCO=ON`:

```
add_subdirectory(poco)
```

Animator Pro links against `libpoco` only. Public headers are exposed via `poco/include`.


