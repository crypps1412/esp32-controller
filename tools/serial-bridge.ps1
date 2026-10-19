# Exposes a Windows COM port to the dev container over RFC2217.
# Run on the Windows host (outside the container), keep it open while flashing/monitoring.
# Requires esptool on the host:  pip install esptool
#
#   .\tools\serial-bridge.ps1                 # auto-select the only COM port
#   .\tools\serial-bridge.ps1 -ComPort COM5   # explicit port
#   .\tools\serial-bridge.ps1 -TcpPort 4001   # also set $env:ESP_BRIDGE_PORT=4001 before opening the container
param(
    [string]$ComPort,
    [int]$TcpPort = 4000
)

if (-not $ComPort) {
    # Only USB serial adapters (CP210x, CH340, FTDI, native USB) - skips built-in COM1 etc.
    $ports = Get-CimInstance Win32_PnPEntity |
        Where-Object { $_.Name -match '\((COM\d+)\)' -and $_.PNPDeviceID -match '^(USB|FTDIBUS)' } |
        ForEach-Object { [regex]::Match($_.Name, 'COM\d+').Value } | Sort-Object -Unique
    if ($ports.Count -eq 0) { Write-Error "No COM ports found. Is the board plugged in?"; exit 1 }
    if ($ports.Count -gt 1) { Write-Error "Several COM ports found ($($ports -join ', ')). Pass -ComPort."; exit 1 }
    $ComPort = @($ports)[0]
}

Write-Host "Bridging $ComPort -> rfc2217://0.0.0.0:$TcpPort (Ctrl+C to stop)"
Write-Host "In the container: idf.py flash monitor   (ESPPORT=rfc2217://host.docker.internal:$TcpPort?ign_set_control)"

# esptool >= 5 ships esp_rfc2217_server; 4.x ships esp_rfc2217_server.py
if (Get-Command esp_rfc2217_server -ErrorAction SilentlyContinue) {
    & esp_rfc2217_server -v -p $TcpPort $ComPort
} elseif (Get-Command esp_rfc2217_server.py -ErrorAction SilentlyContinue) {
    & esp_rfc2217_server.py -v -p $TcpPort $ComPort
} else {
    & python -m esp_rfc2217_server -v -p $TcpPort $ComPort
}
exit $LASTEXITCODE
