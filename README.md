# opencv-godot
Godot OpenCV



this is work with `scons` without errors when I removed this line:
```python
env.Append(LINKFLAGS=["-Wl,--whole-archive", "D:/opencv-4.0.1-android-sdk/OpenCV-android-sdk/sdk/native/3rdparty/libs/arm64-v8a/liblibwebp.a", "-Wl,--no-whole-archive"])
```



but when I try run the Project from Godot on Android `remote debug` show this error:

```
Project export for platform: Android - Failed.
• [Run]: Could not install to device: Serving...
All files should be loaded. Notifying the device.
adb: failed to install c:/Users/user/AppData/Local/Godot/tmpexport. 1715077950.apk: Failure
Failed to extract native libraries, res=-2]
Performing Incremental Install
Failure Failed to extract native libraries, res=-1 1
Performing Streamed Install
```
