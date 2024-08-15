Use `.pcml` file.

## `pcml` file syntax
use xml grammar.
```xml
<pcml>
	<text id="Welcome" pos=3,2 size=2,20 col=0x70>
		Hello!
	</text>
	<rect id="Paint" pos=5,5 size=30,30>
</pcml>
```

## Lable table
|lable|target|Param|Content|
|:-:|:-:|:-:|:-:|
|`pcml`|Whole screen|`pos` `size` `col`|Children nodes
|`text`|Textbox|`id` `pos` `size` `col`|Text content|
|`button`|Button|`id` `pos` `size` `col` `col_hover`|Display text
|`rect`|Char graph|`id` `pos` `size`|NULL|