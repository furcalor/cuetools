# cuetools
This repository provides modernized, dependency‑free implementations of cuebreakpoints and cuetag — lightweight command‑line tools for splitting and tagging FLAC files using CUE sheets. Both tools are ANSI‑C, compile cleanly with MinGW or MSVC, and are designed to integrate with shntool‑based workflows (e.g., cuebreakpoints | shntool split -o flac). Ideal for automated media pipelines, Lidarr post‑processing, and environments where the original Windows binaries are no longer available.

Note cuetag requires metaflac.exe
