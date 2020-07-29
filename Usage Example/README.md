# Default Parser Usage

## Options

**DELIM:** The delimiter/separator for generated YAML data. Don't change this unless you know what you're doing. <br />
Default: `","` (e.g. `1,2,3`) <br />
  
**SUPPORTED_VERSIONS:** The NBS file format versions supported by this parser/player script, separated by ` or `. Don't change this unless you know what you're doing. <br />
Default: `4` <br />
  
**INSTRUMENT_SOUND_PREFIX:** The prefix for instrument sounds. <br />
Default: `"block.note."` (e.g. `block.note.harp`) <br />
As of writing this, in [Minecraft 1.16.1](https://www.minecraft.net/en-us/article/minecraft-java-edition-1-16-1), the prefix is `block.note_block.`. Please change this option according to your minecraft version.
  
**INSTRUMENT_SOUND_LIST:** A list of sound names for instruments in minecraft, separated by commas (`,`). <br />
Default: `"harp,bass,basedrum,snare,hat,guitar,flute,bell,chime,xylophone,iron_xylophone,cow_bell,didgeridoo,bit,banjo,pling"` <br />
As of writing this, in [Minecraft 1.16.1](https://www.minecraft.net/en-us/article/minecraft-java-edition-1-16-1), there are 16 sounds. You do not need to change this for older versions unless the instrument ID/order changes. If you are in an older version, the newer sounds will simply not play for you. <br />
  
## Functions

### loadConstants() <br />
  
**Definition:**
```
function NBSYAML_loadConstants()
```
**Usage:** Loads instrument sounds and key pitches. Must be called before attempting to play songs. <br />
<br />
  
### unload() <br />
  
**Definition:**
```
function NBSYAML_unload()
```
**Usage:** Unloads all NBSYAML data, including constants, loaded songs, and connections. <br />
<br />
  
### loadSong() <br />
  
**Definition:**
```
function NBSYAML_loadSong(songName: string, file: string, listName: string = "NoteBlockSongData", log: boolean = true) :: string
```
**Usage:** Loads a specific song from a YAML file. A song must be loaded before attempting to play it. <br />
**Parameters:**
* *songName*: The name that the loaded song will be saved under. Must be a valid variable name.
* *file*: The path to the file to load, starting from the server directory. Must be a valid file path. E.g. `plugins/Skript/song/mysong.yml`
* *listName*: Name of the YAML list to load. Must be a valid YAML key.
* *log*: Whether to log to console or not. Must be a boolean. <br />

**Return:** Returns the result as a string message, indicating whether the function was successful or not. <br />
<br />
  
### playSong() <br />
  
**Definition:**
```
function NBSYAML_playSong(songName: string, players: players, location: location, connectionReturn: string, setVolume: number = -1, setTempo: number = -1, loopInfo: numbers = -1, startIndex: number = 1) :: string
```
**Usage:** Starts a connection and plays a song at a given location, to a given selection of players. <br />
**Parameters:**
* *songName*: The name of the song to play.
* *players*: A list of players to play to.
* *location*: A location to play the song at. If given `<none>`, defaults to location of `{_players::1}`.
* *connectionReturn*: Name of the variable to save the connection ID to. Must be a valid variable name.
* *setVolume*: If `>=0`, plays song at a set volume, ignoring the volume in the song data.
* *setTempo*: If `>=0`, plays song at a set tempo, ignoring the tempo in the song data.
* *loopInfo*: If `!=-1`, plays song with specific loop details, ignoring loop details in the song data. E.g. `1, 3 and 5` would play the song 3 times, starting at tick 5 on the second and third loop.
* *startIndex*: The tick index to start the song at on the first iteration. Can cause unexpected results if `<1`. <br />

**Return:** Returns nothing if the function was successful. If something went wrong, an error message is returned. <br />
<br />
  
### stopSong() <br />
  
**Definition:**
```
function NBSYAML_stopSong(connectionId: string)
```
**Usage:** Stops a connection/song, given the ID returned by the `playSong()` function. <br />
**Parameters:**
* *connectionId*: The ID of the connection to disconnect. <br />
<br />
  
## Examples

**Some commands utilizing the functions:** [CommandExample.sk](https://github.com/CreepahGIT/NBStoYAML/blob/master/Usage%20Example/CommandExample)
```
command loadsong <string> <string>:
    usage: /loadsong <songname> <songpath>
    # e.g. /loadsong MySong plugins/Skript/song/MySong.yml
    permission: skript.nbsyaml
    trigger:
        set {_result} to NBSYAML_loadSong(arg-1, arg-2)
        send "&aLoad Result: &f%{_result}%" to executor

command playsong <string>:
    usage: /playsong <songname>
    # e.g. /playsong MySong
    permission: skript.nbsyaml
    executable by: players
    trigger:
        if {-NBSYAML_CONNECTION::%{-songConnection::%executor's uuid%}%} is set:
            send "&cPlease stop the current song before playing another one &7(/stopsong)" to executor
            exit trigger
        set {_error} to NBSYAML_playSong(arg-1, executor, {_}, "-songConnection::%executor's uuid%")
        if {_error} is set:
            send "&cFailed to play song %arg-1%: &f%{_error}%" to executor
            exit trigger
        send "&aPlaying song %arg-1%" to executor

command stopsong:
    permission: skript.nbsyaml
    executable by: players
    trigger:
        if {-NBSYAML_CONNECTION::%{-songConnection::%executor's uuid%}%} is not set:
            send "&cNo songs are currently playing" to executor
            exit trigger
        delete {-NBSYAML_CONNECTION::%{-songConnection::%executor's uuid%}%}
        delete {-songConnection::%executor's uuid%}
        send "&aStopped playing current song" to executor
```
<br />
  
**A small snippet playing and stopping a song:**
```
# Play MySong to event-player
set {_error} to NBSYAML_playSong("MySong", event-player, {_}, "-MyConnection")
if {_error} is set:
    broadcast "oh no, it failed!"
    exit trigger
wait 5 seconds
# stop song after 5 seconds
NBSYAML_stopSong({-MyConnection})
```
