$files = Get-Item *.jpg,*.png

foreach($f in $files){
    Start-Process -FilePath TextureConverter.exe -ArgumentList "$f -ml 3 -bc 7" -Wait
}
pause