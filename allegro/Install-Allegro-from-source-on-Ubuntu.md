1. Install general system dependencies.

    ```bash
    sudo apt install git cmake build-essential
    ```

2. Install various dependencies.

    ```bash
    sudo apt install libdumb1-dev \
                     libflac-dev \
                     libfreetype6-dev \
                     libgl1-mesa-dev \
                     libglu1-mesa-dev \
                     libgtk-3-dev \
                     libjpeg-dev \
                     libopenal-dev \
                     libphysfs-dev \
                     libpng-dev \
                     libpulse-dev \
                     libtheora-dev \
                     libvorbis-dev \
                     libwebp-dev \
                     libxext-dev \
                     libxxf86vm-dev \
                     libxrandr-dev \
                     libxinerama-dev \
                     libxpm-dev \
                     libopusfile-dev
    ```

3. Download Allegro's source from git and navigate to its directory.

    ```bash
    git clone https://github.com/liballeg/allegro5.git
    cd allegro5
    ```
    
    Naturally, you can replace the repository to something else if you want 
    grab the code from, e.g., a GitHub fork.

4. If you want MP3 support, download the header-only MiniMP3 library as follows:

   ```bash
   mkdir -p deps/include
   cd deps/include
   curl -O https://raw.githubusercontent.com/lieff/minimp3/master/minimp3.h
   curl -O https://raw.githubusercontent.com/lieff/minimp3/master/minimp3_ex.h
   cd ../..
   ```

5. Configure and build Allegro.

    ```bash
    mkdir build
    cd build
    cmake ..
    make -j$(nproc)
    ```
    
    If you're reproducing some sort of bug or developing a new feature, it can 
    be helpful to build Allegro in debug mode by issuing `cmake .. 
    -DCMAKE_BUILD_TYPE=Debug`. The default `CMAKE_BUILD_TYPE` is 
    `RelWithDebInfo`.

6. Test by running an example.

    ```bash
    cd examples
    ./ex_draw_bitmap
    ```