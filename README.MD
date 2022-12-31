<div id="top"></div>

<p align="center">
    <!--
    <a href="https://github.com/mnfitz-ASU/cse340redux/pulse" alt="Activity">
        <img src="https://img.shields.io/github/commit-activity/mnfitz-ASU/compsci/cse325/project4" /></a>
    <a href="https://discord.gg/sKXHzHpq">
        <img src="https://img.shields.io/discord/308323056592486420?logo=discord"
            alt="chat on Discord"></a>
    -->
    <a href="https://github.com/mnfitz-ASU/mnfitz-ASU/graphs/contributors" alt="Contributors">
        <img src="https://img.shields.io/github/contributors/mnfitz-ASU/mnfitz-ASU" /></a>    
    <a href="https://www.linkedin.com/in/matthew-n-fitzgerald/">
        <img src="https://img.shields.io/badge/-matthewf-blue?style=flat-square&logo=Linkedin&logoColor=white&link=https://www.linkedin.com/in/matthew-n-fitzgerald/"
            alt="follow on Linkedin"></a>
</p>


# CSE325 Simon Project
## Description:
CSE 325 project to demonstrate proficiency in understanding how to program a FPGA board
- Idea: Program a FRDM-KE16Z FPGA board to simulate a playable game of Simon.
This project modifies the instructor provided CSE 325 "skeleton code" used as the basis for all CSE 325 programming labs and implements the software on a FRDM-KE16Z FPGA board. During a game of Simon, there are four LED lights that can light in a sequence. The player must then recreate that sequence by pushing each LED's corresponging button. If the player remembers correctly, all the lights will flash twice and after a biref pause next sequence will begin to play. If the player remembers the sequence wrong, the correct sequence will repeat, and the game will end.

## The Board
<p align="center">
<img width="593" alt="arrow" src="https://user-images.githubusercontent.com/79553911/210030701-7f396a60-6a4e-4025-ac87-09cc59af6805.png">
<p/>

## Prerequisites
- Download [Visual Studio Code](https://code.visualstudio.com/)
##### Windows
- Download GNU build tools from [mingw](https://github.com/niXman/mingw-builds-binaries/releases)
- (Use: `x86_64-XX.Y.Y-release-posix-seh-rt_v10-rev0.7z`)
- Extract to `C:\Program Files\mingw64`
- Add to `$PATH` envvar:  Settings > System Properties > Environment Variables > `PATH`
- |`PATH`|`C:\Program Files\mingw64\bin`|

##### Mac
- Install [Xcode](https://apps.apple.com/us/app/xcode/id497799835?mt=12) to get build tools

##### Linux
- Install build tools: 
```
$ sudo apt update 
$ sudo apt install build-essential
```

<!-- LICENSE -->
### License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/github_username/repo_name.svg?style=for-the-badge
[contributors-url]: https://github.com/github_username/repo_name/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/github_username/repo_name.svg?style=for-the-badge
[forks-url]: https://github.com/github_username/repo_name/network/members
[stars-shield]: https://img.shields.io/github/stars/github_username/repo_name.svg?style=for-the-badge
[stars-url]: https://github.com/github_username/repo_name/stargazers
[issues-shield]: https://img.shields.io/github/issues/github_username/repo_name.svg?style=for-the-badge
[issues-url]: https://github.com/github_username/repo_name/issues
[license-shield]: https://img.shields.io/github/license/github_username/repo_name.svg?style=for-the-badge
[license-url]: https://github.com/github_username/repo_name/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/linkedin_username
[product-screenshot]: images/screenshot.png
