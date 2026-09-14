doxygen
mkdir doc -Force
moxygen --classes --flavor github --html-anchors --output=doc/%s.md xml  
Remove-Item -Path xml -Force -Confirm
echo "-----Done!------"