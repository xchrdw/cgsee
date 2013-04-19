# CGSee

Free open source viewer/explorer for computer graphics related data, intended to become a *Deep Exploration* (product development discontinued) replacement.


## Development Notes

If you are contributing to this project, please keep the following notes in mind:
* Add your name and email to the AUTHORS file.
* Follow coding conventions according to google's [C++ Style Guide](http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml).
* Commits should provide a meaningful  message, which uses the imperative, present tense: "change", not "changed" nor "changes", AND start with a capital letter.
* Commits should always refer to an issue: use #xxx, fix #xxx, or close #xxx.
* Pull Requests are reviewed by at least one other developer on another platform.
* Use lazy initialization as often as possible for time consuming tasks.
* Member Variables always start with m_ (e.g., m_variable).
* Interface or abstract class names always start with Abstract (e.g., AbstractPainter).
* Enforce strict include sequence: gl, glew, std, glm, qt, header, cgsee (there might be few exceptions, e.g., gpuquery).

## Build Instructions

The project is currently configured, developed, and tested on linux and windws plattforms (both x64) and uses CMake, Qt5, and GLEW.

### Requirements

The following dev-libraries and programs need to be provided for correct CMake configuration:
* Qt5: http://qt-project.org/
* CMake 2.8.9 (waiting for 2.8.11): http://www.cmake.org/
* OpenGL Extension Wrangler (GLEW): http://glew.sourceforge.net/
* OpenGL Mathematics (GLM): http://glm.g-truc.net/

### Configure and CMake

* For windows plattforms, consider to use the provide batch files, and edit *_configure.bat* (with "git update-index --assume-unchanged _*.bat" your personal configuration can be ignored)
* For linux, TODO
