# Sonic Adventure Blast 2

A sequel to [Sonic Adventure Blast](https://github.com/TurtleMan64/Sonic3DGamev3), my older 3D Sonic fangame. SAB2 will have many improvements and other new features. 

**Currently still a work in progress.**

## Screenshots

<p align="center">
  <img src="https://cdn.discordapp.com/attachments/480149390758248448/847292405686206514/pic6.png?raw=true">
  <img src="https://cdn.discordapp.com/attachments/480149390758248448/847292443522760734/pic1.png?raw=true">
  <img src="https://cdn.discordapp.com/attachments/480149390758248448/847292464083763210/pic3.png?raw=true">
  <img src="https://cdn.discordapp.com/attachments/480149390758248448/847292501710864404/pic4.png?raw=true">
  <img src="https://cdn.discordapp.com/attachments/480149390758248448/847292520970059796/pic2.png?raw=true">
  <img src="https://cdn.discordapp.com/attachments/480149390758248448/847292551823753216/pic5.png?raw=true">
  <img src="https://cdn.discordapp.com/attachments/480149390758248448/847292563336462415/pic7.png?raw=true">
</p>

# How to compile

## Windows

1. [Download](https://github.com/TurtleMan64/SAB2/archive/refs/heads/master.zip) source code as zip.
2. Extract zip.
3. Open the `NewSonicThing.sln` in MicrosoftVisualStudio
4. Hit `Build->Build NewSonicThing`. This will generate the final exe.
   * Make sure to be in `Release` and `x64` mode
   * You might need to be in a similar version of Visual Studio to what I have generated the dependencies on (Visual Studio Community 2017, Version 15.6.7)

## Linux

1. Install Dependancies
   * `sudo apt install libglfw3-dev libogg-dev libvorbis-dev libopenal-dev libsoil-dev libsdl2-dev pkg-config`
2. Build
   * `make`
