# Generate a minimal 32x32 32bpp ICO file
$width = 32
$height = 32
$bpp = 32

$header = [byte[]]@(
    0x00, 0x00, # Reserved
    0x01, 0x00, # ICO type
    0x01, 0x00  # 1 image
)

# XOR mask: 32bpp BGRA pixel data
$xorMask = [byte[]]::new($width * $height * 4)
for ($y = 0; $y -lt $height; $y++) {
    for ($x = 0; $x -lt $width; $x++) {
        $idx = ($y * $width + $x) * 4
        $cx = $x - $width/2
        $cy = $y - $height/2
        $dist = [Math]::Sqrt($cx*$cx + $cy*$cy)
        if ($dist -lt 12) {
            # Blue circle
            $xorMask[$idx + 0] = 0xE8       # B
            $xorMask[$idx + 1] = 0x9C       # G
            $xorMask[$idx + 2] = 0x00       # R
            $xorMask[$idx + 3] = 0xFF       # A
        } elseif ($dist -lt 14) {
            # White border
            $xorMask[$idx + 0] = 0xFF
            $xorMask[$idx + 1] = 0xFF
            $xorMask[$idx + 2] = 0xFF
            $xorMask[$idx + 3] = 0xFF
        } else {
            # Transparent
            $xorMask[$idx + 0] = 0x00
            $xorMask[$idx + 1] = 0x00
            $xorMask[$idx + 2] = 0x00
            $xorMask[$idx + 3] = 0x00
        }
    }
}

# AND mask: 1bpp (all zeros for 32bpp)
$andRowSize = [Math]::Floor(($width + 31) / 32) * 4
$andMask = [byte[]]::new($andRowSize * $height)

# BITMAPINFOHEADER
$bmpHeader = [byte[]]::new(40)
[BitConverter]::GetBytes(40) | ForEach-Object { $bmpHeader[$i++] = $_ }; $i = 0
[BitConverter]::GetBytes($width * 2) | ForEach-Object { $bmpHeader[$i++] = $_ }; $i = 4
[BitConverter]::GetBytes($height * 2) | ForEach-Object { $bmpHeader[$i++] = $_ }; $i = 8
[BitConverter]::GetBytes(1) | ForEach-Object { $bmpHeader[$i] = $_; $i++ }; $i = 12
[BitConverter]::GetBytes($bpp) | ForEach-Object { $bmpHeader[$i] = $_; $i++ }; $i = 14
[BitConverter]::GetBytes(0) | ForEach-Object { $bmpHeader[$i] = $_; $i++ }; $i = 16

$imageSize = $bmpHeader.Length + $xorMask.Length + $andMask.Length
$imageOffset = 6 + 16

$dirEntry = [byte[]]::new(16)
$dirEntry[0] = $width
$dirEntry[1] = $height
$dirEntry[2] = 0
$dirEntry[3] = 0
$dirEntry[4] = 1
$dirEntry[5] = $bpp
[BitConverter]::GetBytes($imageSize) | ForEach-Object { $dirEntry[$i] = $_; $i++ }; $i = 8
[BitConverter]::GetBytes($imageOffset) | ForEach-Object { $dirEntry[$i] = $_; $i++ }; $i = 12

$icoBytes = $header + $dirEntry + $bmpHeader + $xorMask + $andMask
[System.IO.File]::WriteAllBytes("$PWD\resources\app.ico", $icoBytes)
Write-Host "Icon created: $($icoBytes.Length) bytes"
