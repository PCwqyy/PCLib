doxygen
mkdir doc
doxybook2 -i ./xml -o ./doc
del /f /q "xml"
rmdir xml