SAM4L Skeleton Project
======================

An Atmel SAM4L skeleton project for GCC with ASF.


Programming
-------------
Due to differences in the SAM4L flash controller, need to use OpenOCD to program.
OpenOCD can be installed via `brew install openocd`.

`openocd -f atmel_sam4l8_xplained_pro.cfg -c "program sam4l-skeleton-flash.elf verify reset exit"`

