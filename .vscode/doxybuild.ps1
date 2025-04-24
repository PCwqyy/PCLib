doxygen
mkdir doc -Force
moxygen --class --html-anchors --output=doc/api.md xml  
Remove-Item -Path xml -Force
echo "-----Done!------"