
# ===================================================
# Script tao muc luc tu dong cho BaocaoSRS-HIEPHAOQUOC.docx
# Phuong phap: Chen TOC field vao dau document
# ===================================================

$docxPath = "C:\Users\Admin\Downloads\BaocaoSRS-HIEPHAOQUOC.docx"
$outPath  = "C:\Users\Admin\Downloads\BaocaoSRS-HIEPHAOQUOC_CoMucLuc.docx"
$zipPath  = "C:\Users\Admin\Downloads\Dev-Cpp 5.7.1\racing_game\_temp_docx.zip"
$tempDir  = "C:\Users\Admin\Downloads\Dev-Cpp 5.7.1\racing_game\_docx_temp"

Copy-Item $docxPath $zipPath -Force
if (Test-Path $tempDir) { Remove-Item $tempDir -Recurse -Force }
Expand-Archive -Path $zipPath -DestinationPath $tempDir -Force

$xmlPath = "$tempDir\word\document.xml"
$xmlBytes = [System.IO.File]::ReadAllBytes($xmlPath)
$xmlText  = [System.Text.Encoding]::UTF8.GetString($xmlBytes)

# === Tao XML cho Muc Luc (TOC) su dung Word field ===
# Day la XML chuan de Word tu render muc luc khi mo file
$tocXml = @'
  <w:p xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main">
    <w:pPr>
      <w:pStyle w:val="Heading1"/>
      <w:jc w:val="center"/>
    </w:pPr>
    <w:r>
      <w:t>MỤC LỤC</w:t>
    </w:r>
  </w:p>
  <w:p xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
       xmlns:w14="http://schemas.microsoft.com/office/word/2010/wordml">
    <w:pPr>
      <w:pStyle w:val="TOCHeading"/>
    </w:pPr>
    <w:fldChar w:fldCharType="begin"/>
    <w:instrText> TOC \o "1-3" \h \z \u </w:instrText>
    <w:fldChar w:fldCharType="separate"/>
    <w:fldChar w:fldCharType="end"/>
  </w:p>
  <w:p xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main">
    <w:r>
      <w:br w:type="page"/>
    </w:r>
  </w:p>
'@

# Dung XML field chuan hon
$tocFieldXml = @'
<w:p xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"><w:pPr><w:jc w:val="center"/><w:rPr><w:b/><w:sz w:val="28"/></w:rPr></w:pPr><w:r><w:rPr><w:b/><w:sz w:val="28"/></w:rPr><w:t>MỤC LỤC</w:t></w:r></w:p><w:p xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"><w:pPr><w:jc w:val="center"/></w:pPr><w:r><w:rPr><w:i/><w:color w:val="595959"/></w:rPr><w:t>(Nhấn Ctrl+A rồi F9 trong Word để cập nhật mục lục)</w:t></w:r></w:p><w:p xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"><w:r><w:fldChar w:fldCharType="begin"/></w:r><w:r><w:instrText xml:space="preserve"> TOC \o "1-3" \h \z \u </w:instrText></w:r><w:r><w:fldChar w:fldCharType="separate"/></w:r><w:r><w:t>[ Mục lục sẽ hiển thị sau khi mở file trong Word và nhấn F9 ]</w:t></w:r><w:r><w:fldChar w:fldCharType="end"/></w:r></w:p><w:p xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"><w:r><w:br w:type="page"/></w:r></w:p>
'@

# Tim vi tri sau <w:body> de chen TOC vao dau
$insertAfter = "<w:body>"
$insertPos = $xmlText.IndexOf($insertAfter)

if ($insertPos -lt 0) {
    Write-Host "KHONG TIM THAY <w:body>!"
    exit 1
}

$insertAt = $insertPos + $insertAfter.Length
$newXmlText = $xmlText.Substring(0, $insertAt) + "`n" + $tocFieldXml + "`n" + $xmlText.Substring($insertAt)

# Ghi lai file
[System.IO.File]::WriteAllBytes($xmlPath, [System.Text.Encoding]::UTF8.GetBytes($newXmlText))

# Nen lai thanh .docx
$outZip = "C:\Users\Admin\Downloads\Dev-Cpp 5.7.1\racing_game\_out_docx.zip"
if (Test-Path $outZip) { Remove-Item $outZip -Force }
Compress-Archive -Path "$tempDir\*" -DestinationPath $outZip -Force
Copy-Item $outZip $outPath -Force

# Don dep
Remove-Item $tempDir -Recurse -Force
Remove-Item $zipPath -Force
Remove-Item $outZip -Force

Write-Host "HOAN THANH! File da luu tai: $outPath"
Write-Host "Hay mo file trong Microsoft Word, bam Ctrl+A roi F9 de cap nhat muc luc."
