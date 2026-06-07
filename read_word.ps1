$word = New-Object -ComObject Word.Application
$word.Visible = $false
$filePath = "C:\Users\Admin\Downloads\(SHARE) MAU BAOCAO Mon hoc (ThamKhao).docx"
$doc = $word.Documents.Open($filePath)
$text = $doc.Content.Text
$text | Out-File -FilePath "C:\racing test\template_content.txt" -Encoding utf8
$doc.Close()
$word.Quit()
Write-Host "Done"
