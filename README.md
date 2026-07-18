# 7135Z "Push Back" Programming Archive

> All original work in this archive was the work of Aidan Bossle, any
> unoriginal material is credited in-file to its respective author(s).

## Lemlib Implementation
7135Z-Lem was the competitive build we actually ran all season: it is primarily just a fork of LemLib (credit: https://github.com/LemLib/LemLib) since I didn't have much time to build out custom pathing stack (see below). Despite these time constraints, there is some additional functionality built on top of it, including a few changes to the autonomous selector and some abstraction in new header files. These can all be found directly in /include as .h headers with their implementations as .cpp listed directly in /src.

## Documentation
In compliance with new emphasis on Rule G4 in this years' Vex V5 Robotics Competition season, I threw together a explanation of LemLib's internals to satisfy the criteria of being able to explain unoriginal code included in your codebase. It was successful to this end, and, ironically, secured us a second interview for the think award at our state competition despite not having updated it since the Sugar Rush competition in January... So that was entertaining! Wasn't much use beyond that but thought it would be a useful inclusion in the archive.

This was my full fledged first personal project utilizing LaTeX for typesetting. Part of the learning process was installing and configuring the TeX live distribution in tandem with BibLaTeX & a custom recipe. VSCode remained my IDE of choice for both the development side and documentation side.

## Custom Autonomous Stack
This detail is included in the latter section of the programming notebook, and while I never brought it to fruition I thought the stack was interesting and included it as an archive for future reference. I opted to use squiggles trajectory path generation as an alternative to building my own two-dimensional motion profiling solution (as those are math-intensive and difficult to debug) but I simply ran out of time to build this stack out considering I was also responsible for all other areas of the robot development.

![Autonomous stack diagram for RAMSETE custom lib](https://raw.githubusercontent.com/atabbed/7135z_progdoc/refs/heads/main/Blank%20diagram%20%281%29.png?token=GHSAT0AAAAAAEBPQVNZ6ALZKARW7LKM4EV42S35S5Q)

> If you want to implement this be my guest I don't know if I would have
> ever got it working since I think I made it like barely halfway
> through the global ref frame stuff.