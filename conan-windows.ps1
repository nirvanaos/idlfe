foreach ($prof in get-childitem profiles\windows-*.profile)
{
  conan create . -pr:h $prof -pr:b $prof --build=missing -s build_type=Debug
  conan create . -pr:h $prof -pr:b $prof --build=missing -s build_type=Release
}