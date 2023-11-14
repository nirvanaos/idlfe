for prof in profiles/linux-*.profile
do
	conan create . -pr:h "$prof" -pr:b "$prof" --build=missing -s:h build_type=Debug -s:b build_type=Debug
	conan create . -pr:h "$prof" -pr:b "$prof" --build=missing -s:h build_type=Release -s:b build_type=Release
done