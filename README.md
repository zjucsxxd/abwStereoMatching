Real-time Stereo Matching
=========================
using Adaptive Binary Window
----------------------------

The goal of this project is to provide an implementation for generating depth maps in real-time, as outlined in this paper:

http://diglib.eg.org/EG/DL/PE/3DPVT/3DPVT2010Papers/03_paper018.pdf

Parts of OpenCV 2.4.8 are currently used for image file I/O, and pixel value access. The libraries used are pre-built (x64, vc12). Make sure to have appropriate release .dlls in the system's **%PATH%**.

The end result should output depth maps of acceptable quality, in real-time, from data aquired by a pair of consumer-grade USB cameras.

The Visual Studio files are compatible with Visual Studio 2013, just remember to add the command arguments under **Project** -> **Properties** -> **Configuration Properties** -> **Debugging**.

To use the images provided in the repository, input:

	$(SolutionDir)\shared\tsukuba1.png
	$(SolutionDir)\shared\tsukuba2.png
