# dualmonfix

In Windows 10, if you have multiple monitors, and they are at different resolutions, the operating system will not let the mouse pass from one monitor
to the next, if one of the axes is not within the range of the next monitor.

This program will modify that behavior, so  that the mouse may always pass freely to the next monitor.  For instance,
if the current Y position of the mouse is too high to be in range of the next monitor when moving to the left, the program will simply
snap the Y position to the minimum y value of the next monior.

# todo

Currently this functions by polling, which works fine and has no measurable CPU use, but it would be better if it were driven by mouse movement events.  I do not know how to get mouse movement events globally, when you are not the program in focus. Is it possible?  


