set GLEW_HOME=<REPLACE>
set QT_DIR=<REPLACE>
set GLM_HOME=<REPLACE>
set ASSIMP_HOME=<REPLACE>
set LIBZEUG_HOME=<REPLACE>
set GTEST_HOME=<REPLACE>
set GMOCK_HOME=<REPLACE>
set QT_QPA_PLATFORM_PLUGIN_PATH=%QT_DIR%/plugins/platforms
set WINDOWS_SDK=C:/Program Files (x86)/Windows Kits/8.1/
set CMAKE_PREFIX_PATH=C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64;%LIBZEUG_HOME%;
set PATH=%WINDOWS_SDK%/bin/x64;%LIBZEUG_HOME%/build/Debug;%LIBZEUG_HOME%/build/Release;%GLEW_HOME%/bin;%ASSIMP_HOME%/bin/x64;%QT_DIR%/bin;%PATH%;