# opencv-godot
Godot OpenCV



this is work with `scons` without errors when I removed this line:
```python
env.Append(LINKFLAGS=["-Wl,--whole-archive", "D:/opencv-4.0.1-android-sdk/OpenCV-android-sdk/sdk/native/3rdparty/libs/arm64-v8a/liblibwebp.a", "-Wl,--no-whole-archive"])
```



