# miv - A viewer for massive images

## Introduction

There are plenty of very capable image viewers around, but for very
large images (several hundred megapixels), most start to struggle. Miv
is a viewer that can readily handle grayscale images up to a gigapixel
or so on contemporary hardware.

## Installation

To build from source in Linux, simply type “make” followed by “sudo
make install”. To build from source in Windows or Mac, please load
“src/miv.pro” in Qt Creator and compile from there.

## Usage

To run on an 8-bit grayscale image, type “miv <imagefile>” or just
“miv” to open a dialog to select the image.

Miv can also handle 16-bit grayscale images. In that case, “contrast
stretching” is usually recommended. To clip 0.5% of pixels on the
black side of the histogram and 0.2% of pixels on the white side, type
“miv -b 0.5 -w 0.2 <imagefile>”.

Miv currently converts color images to grayscale.

Miv works by loading the image file, than precalculating reduced-size
versions of the image by successively averaging over 2x2-pixel
areas. When miv starts, it zooms out to show the entire image in its
window. Press “+” to zoom in and “-” to zoom out. Press “/” to enable
a navigator inset that indicates what part of the full image is being
shown.

Drag the mouse to slowly move around the image, use the scroll wheel
to move more rapidly around the image. Control + scroll zooms in and
out.

## In closing

miv is very new. Your thoughts for features are very welcome.
