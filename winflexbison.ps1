$folder = $args[0]
$winflex_version = "2.5.25"
$uri = "https://github.com/lexxmark/winflexbison/releases/download/v${winflex_version}/win_flex_bison-${winflex_version}.zip"
$archive = $env:TEMP + "\winflexbison.zip"

Invoke-WebRequest -Uri $uri -OutFile $archive
Expand-Archive -Path $archive -DestinationPath $folder -Force
