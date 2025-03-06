## Cmd Fight

A PClib demo.

### How to play
- **The program will execute command with `system()`, so don't play this game with strangers!**
- Use `$Attack` to send command to others!
- If you lost connection you lose.
- Sending message without `$` will just be like a chat room.
- Program used some inside command begin with `&`, so don't send a message start with `&`.
- Input `Ctrl C` will **end** the program (and also, lose the game), be caution.
- Inputting wide-characters is **undefined**, may cause some exceptions.
- Caution some command like `shutdown`, `curl`..., they can execute without administrator permissions.

### `$` Commands

|command|subcommand|effect|
|-|-|-|
|`$Alias [player]`||Query `[player]`'s alias.|
|`$Attack [target] [command]`||Send `[command]` to target. `[target]` can be alias or display name.|
|`$MyRule`||View your rules.|
|`$Name [alias]`||Set your current display name to `[alias]`, and add a new **alias** to you. The old one **won't** be replaced.|
|`$Rule`|`[regex]`|Add `[regex]` to your rule list.|
||`[number] [regex]`|modify your No.`[number]` rule.|