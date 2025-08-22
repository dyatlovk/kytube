# KYouTube
YouTube QT frontend viewer. No API required.

Uses invidious instance [https://docs.piped.video/](https://docs.piped.video/) for searching and playing videos.

## Screenshots
![screen_1](https://github.com/dyatlovk/kytube/assets/4923502/f9d0775c-f808-41b2-bffc-da03b8ea20a4)

## Build from scratch
Install dependencies. For Debian based distros:
```bash
sudo apt install cmake
```

Clone third party libraries:
```bash
git submodule init
git submodule update
```

Build project
```bash
cmake --workflow=debug --preset=debug
```
## Third party libraries
* [Qt6](https://qt.io)
* [OpenSSL](https://www.openssl.org/)
* [curl](https://curl.se/)

## Viewers
* [mpv player](https://mpv.io/)
* [ytdlp](https://github.com/yt-dlp/yt-dlp)
