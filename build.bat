@echo building library...
cmake -S . -B build -G"Unix Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=1
@echo build completed.
