doxygen
mkdir doc
doxybook2 -i ./temp/xml -o ./doc
del /f /q "xml"
rmdir xml