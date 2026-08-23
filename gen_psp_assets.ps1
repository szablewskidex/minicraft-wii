Add-Type -AssemblyName System.Drawing

# 1. ICON0.PNG (144x80)
$icon = New-Object System.Drawing.Bitmap 144, 80
$g = [System.Drawing.Graphics]::FromImage($icon)
$g.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::NearestNeighbor
$g.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::None
$g.PixelOffsetMode = [System.Drawing.Drawing2D.PixelOffsetMode]::Half

# Background
$bgBrush = New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::FromArgb(255, 20, 28, 20))
$g.FillRectangle($bgBrush, 0, 0, 144, 80)

# Border
$borderPen = New-Object System.Drawing.Pen ([System.Drawing.Color]::FromArgb(255, 60, 120, 60)), 2
$g.DrawRectangle($borderPen, 1, 1, 142, 78)

# Title: MINICRAFT
$fontTitle = New-Object System.Drawing.Font ("Courier New", 13, [System.Drawing.FontStyle]::Bold)
$shadowBrush = New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::FromArgb(255, 10, 10, 10))
$titleBrush = New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::FromArgb(255, 90, 230, 90))
$g.DrawString("MINICRAFT", $fontTitle, $shadowBrush, 21, 16)
$g.DrawString("MINICRAFT", $fontTitle, $titleBrush, 20, 15)

# Subtitle: PSP EDITION
$fontSub = New-Object System.Drawing.Font ("Courier New", 9, [System.Drawing.FontStyle]::Bold)
$subBrush = New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::FromArgb(255, 230, 220, 120))
$g.DrawString("PSP EDITION", $fontSub, $shadowBrush, 29, 41)
$g.DrawString("PSP EDITION", $fontSub, $subBrush, 28, 40)

# Footer
$fontFoot = New-Object System.Drawing.Font ("Courier New", 7, [System.Drawing.FontStyle]::Regular)
$footBrush = New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::FromArgb(255, 140, 160, 140))
$g.DrawString("Homebrew Port", $fontFoot, $footBrush, 32, 60)

$g.Dispose()
$icon.Save("ICON0.PNG", [System.Drawing.Imaging.ImageFormat]::Png)
$icon.Dispose()

# 2. PIC1.PNG (480x272)
$pic = New-Object System.Drawing.Bitmap 480, 272
$g2 = [System.Drawing.Graphics]::FromImage($pic)
$g2.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic

# Background dark gradient
$rect = New-Object System.Drawing.Rectangle 0, 0, 480, 272
$gradBrush = New-Object System.Drawing.Drawing2D.LinearGradientBrush ($rect, 
    [System.Drawing.Color]::FromArgb(255, 15, 25, 20), 
    [System.Drawing.Color]::FromArgb(255, 30, 45, 35), 
    [System.Drawing.Drawing2D.LinearGradientMode]::Vertical)
$g2.FillRectangle($gradBrush, $rect)

# Decorative grid pattern
$gridPen = New-Object System.Drawing.Pen ([System.Drawing.Color]::FromArgb(35, 80, 140, 80)), 1
for ($x = 0; $x -lt 480; $x += 16) {
    $g2.DrawLine($gridPen, $x, 0, $x, 272)
}
for ($y = 0; $y -lt 272; $y += 16) {
    $g2.DrawLine($gridPen, 0, $y, 480, $y)
}

# Main Title Banner
$fontBig = New-Object System.Drawing.Font ("Courier New", 26, [System.Drawing.FontStyle]::Bold)
$fontMed = New-Object System.Drawing.Font ("Courier New", 14, [System.Drawing.FontStyle]::Bold)
$fontSmall = New-Object System.Drawing.Font ("Courier New", 10, [System.Drawing.FontStyle]::Regular)

$g2.DrawString("MINICRAFT", $fontBig, $shadowBrush, 43, 63)
$g2.DrawString("MINICRAFT", $fontBig, $titleBrush, 40, 60)

$g2.DrawString("PlayStation Portable Edition", $fontMed, $shadowBrush, 42, 112)
$g2.DrawString("PlayStation Portable Edition", $fontMed, $subBrush, 40, 110)

$descText = "Classic 2D Sandbox Action-Adventure`nNative 480x272 60 FPS | Controls: D-Pad/Analog, [X] Attack, [[]] Inv, [R] Item"
$g2.DrawString($descText, $fontSmall, $footBrush, 40, 150)

$g2.Dispose()
$pic.Save("PIC1.PNG", [System.Drawing.Imaging.ImageFormat]::Png)
$pic.Dispose()

Write-Host "ICON0.PNG and PIC1.PNG created successfully."
