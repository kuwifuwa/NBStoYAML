# How it works? The Explanation.
This page will explain the inner workings of NBStoYAML, i.e., how it works.

## File Format
Before you go ahead, you should know of the [OpenNBS file format](https://opennbs.org/nbs) as well as the [YAML](https://yaml.org/) format that it is converted to.<br>
This part explains YAML conversion of NBS file format **4.0**.<br>
In the examples below, inside curly braces `{}` is a variable. e.g. `- "{var1},{var2}"`
<br>
### Version Number (Line 1)
`- {versionNumber}`<br>
This is the version number, which will always be the NBS file specification version that the . It is used for validating the file version. As of July 2020, if this is not 4, the song won't play.<br>
<br>
  
### Header (Line 2)
`- "{vanillaInstrumentCount},{tempo},{isLooped},{maxLoopCount},{loopStartTick}"`<br>
The header contains information about the song that needs to be determined before playing the song.
| Variable                 | Type             | Description                                                                                                                                      |
|--------------------------|------------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| Vanilla Instrument Count | Number           | The amount of default instruments when the song was saved, although this is not useful information for playing the song in-game, mostly ignored. |
| Tempo                    | Number           | The TPS (ticks per second) of the song, multiplied by 100. E.g. `5.5 tps -> 550 tempo`                                                           |
| Is Looped                | Boolean (0 or 1) | If 1, the next two variables are ignored. If 1, the song is looped.                                                                              |
| Max Loop Count           | Number           | The amount of times the song is looped, where 0 = infinite.                                                                                      |
| Loop Start Ticks         | Number           | The part (tick) of the song to start at when the song ends and loops back.                                                                       |
<br>
  
### Note Block (Line 3+)
`- "{waitTicks},{instrument},{key},{velocity}"`<br>
Each line after line 2 contains a single note block, i.e. a single sound.
| Variable   | Type   | Description                                                                                                                               |
|------------|--------|-------------------------------------------------------------------------------------------------------------------------------------------|
| Wait Ticks | Number | The amount of ticks to wait before playing the sound. This is not the actual ticks in time, it is later scaled according to the song TPS. |
| Instrument | Number | The instrument of the note block. This is 0-15 as of Minecraft 1.16, which has 16 unique note block sounds.                               |
| Key        | Number | The key of the note block. 33-57 is within the 2-octave limit of minecraft, other values will be ignored.                                 |
| Velocity   | Number | The volume of the note block, multiplied by 100. E.g. `0.5 volume -> 50 velocity`                                                         |
<br>
<hr>
  
## Playing the song in Skript using [NBSConnectionPlayer.sk](https://github.com/CreepahGIT/NBStoYAML/blob/master/Usage%20Example/NBSConnectionPlayer.sk)
This part explains how NBSConnectionPlayer plays the song by reading from a YAML file.<br>
After reading this part, you should also be able to write your own parser to play songs in Skript.
<br>
  
### NBSYAML_loadSong
This function reads from a given list in a YAML file (by using SkQuery, Skellett, skript-yaml, etc.) and stores the data in a list variable.<br>
If the song version does not match the version the player was written for, this function will return an error.<br>
Data is stored exactly how it is in the YAML list, but in a Skript list variable.<br>
  
**Example File:**
```
NoteBlockSongData:
- 4
- "16,1000,0,0,0"
- "0,1,40,100"
```
  
When loaded under name `"MySong"`,<br>
**Example Output:**
```
{-NBSYAML_SONG::MySong::1} = 4
{-NBSYAML_SONG::MySong::2} = "16,1000,0,0,0"
{-NBSYAML_SONG::MySong::3} = "0,1,40,100"
```
<br>
  
### NBSYAML_playSong
This function reads from a loaded song and plays it.<br>
First, it reads the header and uses the tempo and loop information.<br>
Then, it loops through all the note blocks, waits the appropriate time, plays the appropriate instrument at the appropriate pitch and volume.<br>
  
**Example Data:**
```
{-NBSYAML_SONG::MySong::1} = 4
{-NBSYAML_SONG::MySong::2} = "16,1000,0,0,0"
{-NBSYAML_SONG::MySong::3} = "0,1,40,100"
```
  
**Example Simplified Pseudo-code Procedure to Play:**
```
Tempo = header::2 parsed as number / 100 # The second value in the header, parsed into a number and divided by 100 to get the song TPS
WaitTimePerTick = 20 / Tempo # The wait time is multiplied by this value to get the actual tick wait time.

IsLooped = header::3 parsed as number # = 0
# Since IsLooped = 0, ignore next 2 values

Loop the loaded song list variable from third index:
  WaitTime = noteblock::1 parsed as number # The first value, wait time
  wait (WaitTime * WaitTimePerTick) ticks
  
  Instrument = noteblock::2 parsed as number # The instrument number
  Sound = getSoundFromInstrument(Instrument) # The sound name e.g. block.note.harp
  
  Key = noteblock::3 parsed as number # The key number
  Pitch = getPitchFromKey(Key) # The pitch number within minecraft's 2-octave limit (0-2)
  
  Volume = noteblock::4 parsed as number / 100 # The volume number
  
  PlaySound(Sound, Volume, Pitch) # Play the sound!
```
