
$docxPath = "C:\Users\Admin\Downloads\BaocaoSRS-HIEPHAOQUOC.docx"
$zipPath  = "C:\Users\Admin\Downloads\Dev-Cpp 5.7.1\racing_game\_temp_docx.zip"
$tempDir  = "C:\Users\Admin\Downloads\Dev-Cpp 5.7.1\racing_game\_docx_temp"

Copy-Item $docxPath $zipPath -Force
if (Test-Path $tempDir) { Remove-Item $tempDir -Recurse -Force }
Expand-Archive -Path $zipPath -DestinationPath $tempDir -Force

$xmlPath = "$tempDir\word\document.xml"
$xmlBytes = [System.IO.File]::ReadAllBytes($xmlPath)
$xmlText  = [System.Text.Encoding]::UTF8.GetString($xmlBytes)
[xml]$xml = $xmlText

$nsm = New-Object System.Xml.XmlNamespaceManager($xml.NameTable)
$nsm.AddNamespace("w", "http://schemas.openxmlformats.org/wordprocessingml/2006/main")

$headings = @()
foreach ($para in $xml.SelectNodes("//w:p", $nsm)) {
    $styleNode = $para.SelectSingleNode("w:pPr/w:pStyle", $nsm)
    if ($styleNode) {
        $styleVal = $styleNode.GetAttribute("val", "http://schemas.openxmlformats.org/wordprocessingml/2006/main")
        if ($styleVal -match "^Heading\d$") {
            $texts = $para.SelectNodes(".//w:t", $nsm)
            $text = ($texts | ForEach-Object { $_.InnerText }) -join ""
            if ($text.Trim() -ne "") {
                $level = [int]($styleVal -replace "Heading","")
                $headings += [PSCustomObject]@{ Level = $level; Text = $text.Trim() }
            }
        }
    }
}

Write-Host "=== MUC LUC ==="
foreach ($h in $headings) {
    $indent = "  " * ($h.Level - 1)
    $prefix = switch ($h.Level) {
        1 { "" }
        2 { "" }
        3 { "" }
        default { "" }
    }
    Write-Host "$indent$prefix$($h.Text)"
}

Remove-Item $tempDir -Recurse -Force
Remove-Item $zipPath -Force
