for prof in profiles/linux-*.profile
do
	conan create . -pr:h "$prof" -pr:b "$prof" --build=missing -s build_type=Debug
	conan create . -pr:h "$prof" -pr:b "$prof" --build=missing -s build_type=Release
done