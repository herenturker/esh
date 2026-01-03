all: configure build

configure:
	cmake -S . -B build -G Ninja \
	-DCMAKE_CXX_COMPILER=clang-cl \
	-DCMAKE_LINKER=lld-link

compile:
	cmake --build build --verbose

run: compile
	powershell -Command "& '.\\build\\esh.exe'"

clean:
	powershell -Command "Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue"
