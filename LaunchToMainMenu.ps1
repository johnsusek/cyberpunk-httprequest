# Cyberpunk2077 launch script to skip final intro movie
# Usage: powershell -ExecutionPolicy Bypass -File "Cyberpunk 2077 - Spacebar Pusher.ps1"

# After launch this script will tap spacebar once every second
# until $stopAfter many seconds have elapsed

# Tweak this number to match your system
# If this number is too low the intro won't be skipped
# In this number is too high spacebar will keep being tapped in the main menu
$stopAfter = 25

$path = "C:\Program Files\Epic Games\Cyberpunk2077\bin\x64\Cyberpunk2077.exe"
$args = "-modded --launcher-skip -skipStartScreen --windowed --width 2048 --height 1152"

# Kill previous instances running
taskkill /im Cyberpunk2077.exe
taskkill /f /im Cyberpunk2077.exe
taskkill /im scc.exe
taskkill /f /im scc.exe

psexec -i 1 "$path $args"

Start-Sleep 1

$title = "Cyberpunk 2077 (C) 2020 by CD Projekt RED"
$shell = New-Object -ComObject wscript.shell

for ($idx = 1; $idx -lt $stopAfter; $idx++)
{
  $_ = $shell.AppActivate($title)
  $_ = $shell.SendKeys(' ')
  Start-Sleep 1
}
