# lesson-controller
Tool to monitor and manage static timetables, like in school

![Header](https://dangertechhub.000webhostapp.com/lesson-controller/images/monday.png)

## What.
lesson-controller is basically your **timetable in digital format** with a few features that
make your life easier (or if you're too lazy to remember your timetable, like me). You **simply
copy your timetable** into two files, which takes about **4 Minutes** and are set and ready to go
to recall it from your terminal, in **mere miliseconds**. No more pulling up an image or even worse,
looking into your planner that you don't always have with you.

Also, lesson-controller is designed to be scriptable. All output can be turned into terse mode
with the `-t` flag. Then, all important data is separated by slashes which can be easily read 
by scripts through tools like `cut`.

The "few features" include **lookup of specific days, days from today, lessons from now, the next
lesson containing the given search query, the current lesson, and more**.

There are some switches which modify output, an example of which can be seen here:

![next](https://dangertechhub.000webhostapp.com/lesson-controller/images/-nMaths.png)
![next-terse](https://dangertechhub.000webhostapp.com/lesson-controller/images/-t--nextMaths.png)
![next-nocolor](https://dangertechhub.000webhostapp.com/lesson-controller/images/-C--nextMaths.png)
![next-notitle-nocolor](https://dangertechhub.000webhostapp.com/lesson-controller/images/-T-C--nextMaths.png)

Also, lesson-controller is themable! You can configure the theme in the config file.

## Install and config
`cd` whereever you keep your git clones and execute the following (explicitely DO NOT make with sudo):


    git clone https://github.com/Dangertech/lesson-controller 
    make install


This adds an executable called `lesson` to `/usr/local/bin` and creates the director `~/.config/lesson-controller`.
Move into this directory:

    cd ~/.config/lesson-controller

There, you should be able to see three files: `config.conf`, `timeframes.dat`, `lessondata.dat`. If you
peek into these files, you can see a tutorial on what they do and how you have to customize each of them. 
Just follow them and pluck your own timetable in there.

If you do have problems with this process or have ideas on how to improve it, 
please [open an issue](https://github.com/dangertech/lesson-controller/issues/new).

To get information on how to use lesson-controller in detail, call `lesson --help`.
