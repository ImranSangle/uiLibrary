@echo building library in debug mode..
cmake -S . -B build -G"Unix Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug
@echo build completed.
