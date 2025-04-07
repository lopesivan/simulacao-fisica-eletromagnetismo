Allegro 5 has official Ubuntu PPAs. You can download recent releases of Allegro this way without needing to compile it yourself.

For the stable branch (5.2) run this command in your terminal:

```bash
sudo add-apt-repository ppa:allegro/5.2
```

Now, run these commands to install the development versions of Allegro:

```bash
sudo apt update
# The core library.
sudo apt install liballegro5-dev
# The addons.
sudo apt install liballegro-acodec5-dev liballegro-audio5-dev \
    liballegro-image5-dev liballegro-dialog5-dev liballegro-ttf5-dev \
    liballegro-physfs5-dev
```