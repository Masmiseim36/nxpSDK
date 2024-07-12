/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Maestro Audio Framework", "index.html", [
    [ "Getting started with Maestro framework", "index.html", "index" ],
    [ "Maestro Audio Framework Programmer's Guide", "md_ProgrammersGuide.html", [
      [ "Introduction", "md_ProgrammersGuide.html#autotoc_md5", null ],
      [ "Architecture overview", "md_ProgrammersGuide.html#autotoc_md6", null ],
      [ "Pipeline", "md_ProgrammersGuide.html#autotoc_md7", null ],
      [ "Elements", "md_ProgrammersGuide.html#autotoc_md8", [
        [ "Add a new element type", "md_ProgrammersGuide.html#autotoc_md9", null ],
        [ "Add a new element index", "md_ProgrammersGuide.html#autotoc_md10", null ]
      ] ],
      [ "Pads", "md_ProgrammersGuide.html#autotoc_md11", null ],
      [ "Internal communication", "md_ProgrammersGuide.html#autotoc_md12", null ],
      [ "Decoders and encoders", "md_ProgrammersGuide.html#autotoc_md13", [
        [ "Common codec interface", "md_ProgrammersGuide.html#autotoc_md14", null ],
        [ "Using the CCI to interface with Metadata", "md_ProgrammersGuide.html#autotoc_md15", null ],
        [ "Using the CCI to interface with Decoders", "md_ProgrammersGuide.html#autotoc_md16", null ],
        [ "Adding new decoders to the CCI", "md_ProgrammersGuide.html#autotoc_md17", null ],
        [ "Common encoder interface", "md_ProgrammersGuide.html#autotoc_md18", null ]
      ] ],
      [ "Maestro performance", "md_ProgrammersGuide.html#autotoc_md19", [
        [ "Memory information", "md_ProgrammersGuide.html#autotoc_md20", null ],
        [ "CPU usage", "md_ProgrammersGuide.html#autotoc_md21", null ]
      ] ],
      [ "Opus codec", "md_ProgrammersGuide.html#autotoc_md22", null ],
      [ "Opus file", "md_ProgrammersGuide.html#autotoc_md23", null ]
    ] ],
    [ "CEI encoder", "md_CeiEncoder.html", [
      [ "Header files", "md_CeiEncoder.html#autotoc_md25", null ],
      [ "Instantiating the element", "md_CeiEncoder.html#autotoc_md26", null ],
      [ "Element properties", "md_CeiEncoder.html#autotoc_md27", null ],
      [ "CEI definition - implementing your own encoder", "md_CeiEncoder.html#autotoc_md28", null ]
    ] ],
    [ "AAC decoder", "aacdec_start.html", [
      [ "AAC decoder features", "aacdec_start.html#autotoc_md29", null ],
      [ "Specification and reference", "aacdec_start.html#autotoc_md30", null ],
      [ "Performance", "aacdec_start.html#autotoc_md31", [
        [ "Memory information", "aacdec_start.html#autotoc_md32", null ],
        [ "CPU usage", "aacdec_start.html#autotoc_md33", null ]
      ] ]
    ] ],
    [ "FLAC decoder", "flacdec_start.html", [
      [ "FLAC decoder features", "flacdec_start.html#autotoc_md34", null ],
      [ "Specification and reference", "flacdec_start.html#autotoc_md35", [
        [ "Official website", "flacdec_start.html#autotoc_md36", null ],
        [ "Inbound licensing", "flacdec_start.html#autotoc_md37", null ]
      ] ],
      [ "Performance", "flacdec_start.html#autotoc_md38", [
        [ "Memory information", "flacdec_start.html#autotoc_md39", null ],
        [ "CPU usage", "flacdec_start.html#autotoc_md40", null ],
        [ "Following test cases are performed:", "flacdec_start.html#autotoc_md41", null ]
      ] ]
    ] ],
    [ "MP3 decoder", "mp3dec_start.html", [
      [ "MP3 decoder features", "mp3dec_start.html#autotoc_md42", null ],
      [ "Performance", "mp3dec_start.html#autotoc_md43", [
        [ "Memory information", "mp3dec_start.html#autotoc_md44", null ],
        [ "CPU usage", "mp3dec_start.html#autotoc_md45", null ]
      ] ]
    ] ],
    [ "WAV decoder", "md_wavdec.html", [
      [ "WAV decoder features", "md_wavdec.html#autotoc_md47", null ],
      [ "Performance", "md_wavdec.html#autotoc_md48", [
        [ "Memory information", "md_wavdec.html#autotoc_md49", null ],
        [ "CPU usage", "md_wavdec.html#autotoc_md50", null ],
        [ "Following test cases were performed:", "md_wavdec.html#autotoc_md51", null ]
      ] ]
    ] ],
    [ "Maestro playback example", "md_maestro__playback.html", [
      [ "Table of content", "md_maestro__playback.html#autotoc_md53", null ],
      [ "Overview", "md_maestro__playback.html#maestro-playback-overview", null ],
      [ "Hardware requirements", "md_maestro__playback.html#maestro-playback-HW-requirements", null ],
      [ "Hardware modifications", "md_maestro__playback.html#maestro-playback-HW-modifications", null ],
      [ "Preparation", "md_maestro__playback.html#maestro-playback-preparation", null ],
      [ "Running the demo", "md_maestro__playback.html#maestro-playback-running-the-demo", null ],
      [ "Example configuration", "md_maestro__playback.html#maestro-playback-example-configuration", null ],
      [ "Functionality", "md_maestro__playback.html#maestro-playback-functionality", null ],
      [ "States", "md_maestro__playback.html#maestro-playback-states", null ],
      [ "Commands in detail", "md_maestro__playback.html#maestro-playback-commands-in-detail", [
        [ "help, version", "md_maestro__playback.html#maestro-playback-help-version", null ],
        [ "file stop", "md_maestro__playback.html#file-stop", null ],
        [ "file pause", "md_maestro__playback.html#file-pause", null ],
        [ "file volume <volume>", "md_maestro__playback.html#file-volume-volume", null ],
        [ "file seek <seek_time>", "md_maestro__playback.html#file-seek-seek_time", null ],
        [ "file play <filename>", "md_maestro__playback.html#file-play-filename", null ],
        [ "file list", "md_maestro__playback.html#file-list", null ],
        [ "file info", "md_maestro__playback.html#file-info", null ]
      ] ],
      [ "Processing Time", "md_maestro__playback.html#maestro-playback-processing-time", null ]
    ] ],
    [ "Maestro record example", "md_maestro__record.html", [
      [ "Table of content", "md_maestro__record.html#autotoc_md55", null ],
      [ "Overview", "md_maestro__record.html#maestro-record-overview", null ],
      [ "Hardware requirements", "md_maestro__record.html#maestro-record-HW-requirements", null ],
      [ "Hardware modifications", "md_maestro__record.html#maestro-record-HW-modifications", null ],
      [ "Preparation", "md_maestro__record.html#maestro-record-preparation", null ],
      [ "Running the demo", "md_maestro__record.html#maestro-record-running-the-demo", null ],
      [ "Example configuration", "md_maestro__record.html#maestro-record-example-configuration", null ],
      [ "Functionality", "md_maestro__record.html#maestro-record-functionality", null ],
      [ "States", "md_maestro__record.html#maestro-record-states", null ],
      [ "Commands in detail", "md_maestro__record.html#maestro-record-commands-in-detail", [
        [ "help, version", "md_maestro__record.html#maestro-record-help-version", null ],
        [ "record_mic audio <time>", "md_maestro__record.html#maestro-record-record-mic-audio", null ],
        [ "record_mic file <time> ; record_mic <file_name> <time>", "md_maestro__record.html#maestro-record-record-mic-file", null ],
        [ "record_mic vit <time> <language>", "md_maestro__record.html#maestro-record-record-mic-vit", null ],
        [ "opus_encode", "md_maestro__record.html#maestro-record-opus_encode", null ]
      ] ],
      [ "Processing Time", "md_maestro__record.html#maestro-record-processing-time", null ]
    ] ],
    [ "Maestro USB microphone example", "md_maestro__usb__mic.html", [
      [ "Table of content", "md_maestro__usb__mic.html#autotoc_md57", null ],
      [ "Overview", "md_maestro__usb__mic.html#maestro-usb-mic-overview", null ],
      [ "Hardware requirements", "md_maestro__usb__mic.html#maestro-usb-mic-HW-requirements", null ],
      [ "Hardware modifications", "md_maestro__usb__mic.html#maestro-usb-mic-HW-modifications", null ],
      [ "Preparation", "md_maestro__usb__mic.html#maestro-usb-mic-preparation", null ],
      [ "Running the demo", "md_maestro__usb__mic.html#maestro-usb-mic-running-the-demo", null ],
      [ "Example configuration", "md_maestro__usb__mic.html#maestro-usb-mic-example-configuration", null ],
      [ "Functionality", "md_maestro__usb__mic.html#maestro-usb-mic-functionality", null ],
      [ "States", "md_maestro__usb__mic.html#maestro-usb-mic-states", null ],
      [ "Commands in detail", "md_maestro__usb__mic.html#maestro-usb-mic-commands-in-detail", [
        [ "help, version", "md_maestro__usb__mic.html#maestro-usb-mic-help-version", null ],
        [ "usb_mic <seconds>", "md_maestro__usb__mic.html#maestro-usb-mic-usb-mic", null ]
      ] ]
    ] ],
    [ "Maestro USB speaker example", "md_maestro__usb__speaker.html", [
      [ "Table of content", "md_maestro__usb__speaker.html#autotoc_md59", null ],
      [ "Overview", "md_maestro__usb__speaker.html#maestro-usb-speaker-overview", null ],
      [ "Hardware requirements", "md_maestro__usb__speaker.html#maestro-usb-speaker-HW-requirements", null ],
      [ "Hardware modifications", "md_maestro__usb__speaker.html#maestro-usb-speaker-HW-modifications", null ],
      [ "Preparation", "md_maestro__usb__speaker.html#maestro-usb-speaker-preparation", null ],
      [ "Running the demo", "md_maestro__usb__speaker.html#maestro-usb-speaker-running-the-demo", null ],
      [ "Example configuration", "md_maestro__usb__speaker.html#maestro-usb-speaker-example-configuration", null ],
      [ "Functionality", "md_maestro__usb__speaker.html#maestro-usb-speaker-functionality", null ],
      [ "States", "md_maestro__usb__speaker.html#maestro-usb-speaker-states", null ],
      [ "Commands in detail", "md_maestro__usb__speaker.html#maestro-usb-speaker-commands-in-detail", [
        [ "help, version", "md_maestro__usb__speaker.html#maestro-usb-speaker-help-version", null ],
        [ "usb_speaker <seconds>", "md_maestro__usb__speaker.html#maestro-usb-speaker-usb-speaker", null ]
      ] ]
    ] ],
    [ "Maestro sync example", "md_maestro__sync.html", [
      [ "Table of content", "md_maestro__sync.html#autotoc_md61", null ],
      [ "Overview", "md_maestro__sync.html#maestro-sync-overview", null ],
      [ "Hardware requirements", "md_maestro__sync.html#maestro-sync-HW-requirements", null ],
      [ "Hardware modifications", "md_maestro__sync.html#maestro-sync-HW-modifications", null ],
      [ "Preparation", "md_maestro__sync.html#maestro-sync-preparation", null ],
      [ "Running the demo", "md_maestro__sync.html#maestro-sync-running-the-demo", null ],
      [ "Example configuration", "md_maestro__sync.html#maestro-sync-example-configuration", null ],
      [ "Functionality", "md_maestro__sync.html#maestro-sync-functionality", null ],
      [ "States", "md_maestro__sync.html#maestro-sync-states", null ],
      [ "Commands in detail", "md_maestro__sync.html#maestro-sync-commands-in-detail", [
        [ "help, version", "md_maestro__sync.html#maestro-sync-help-version", null ],
        [ "start [nosdcard]", "md_maestro__sync.html#maestro-sync-start", null ],
        [ "stop", "md_maestro__sync.html#maestro-sync-stop", null ],
        [ "debug [on|off]", "md_maestro__sync.html#maestro-sync-debug-on-off", null ]
      ] ]
    ] ],
    [ "Supported features", "md_supported__features.html", [
      [ "Decoders", "md_supported__features.html#autotoc_md63", null ],
      [ "Encoders", "md_supported__features.html#autotoc_md64", null ],
      [ "Sample rate converters", "md_supported__features.html#autotoc_md65", null ],
      [ "Additional libraries", "md_supported__features.html#autotoc_md66", null ]
    ] ],
    [ "Processing Time", "md_processing__time.html", [
      [ "Table of content", "md_processing__time.html#autotoc_md68", null ],
      [ "Maestro playback example", "md_processing__time.html#maestro-playback-times", [
        [ "Time on each element", "md_processing__time.html#autotoc_md69", null ]
      ] ],
      [ "Maestro record example", "md_processing__time.html#maestro-record-times", [
        [ "Pipeline Microphone -> Speaker", "md_processing__time.html#autotoc_md70", null ],
        [ "Pipeline Microphone -> VIT", "md_processing__time.html#autotoc_md71", null ],
        [ "Pipeline Microphone -> Voice seeker -> VIT", "md_processing__time.html#autotoc_md72", null ]
      ] ]
    ] ],
    [ "Topics", "topics.html", "topics" ],
    [ "Data Structures", "annotated.html", [
      [ "Data Structures", "annotated.html", "annotated_dup" ],
      [ "Data Fields", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", null ],
        [ "Variables", "functions_vars.html", "functions_vars" ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "Globals", "globals.html", [
        [ "All", "globals.html", "globals_dup" ],
        [ "Functions", "globals_func.html", "globals_func" ],
        [ "Variables", "globals_vars.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Enumerator", "globals_eval.html", "globals_eval" ],
        [ "Macros", "globals_defs.html", "globals_defs" ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"LVC__Types_8h.html",
"audio__sink__pcmrtos_8h.html#a84a057191882606dee264c48adeb6b3c",
"decoder_8h.html#ac0c171bf12339214c64a6380bff78d05",
"globals_defs_t.html",
"mem__sink_8h.html#a03e600cb36c8500d5b9eb7f92a13b637",
"streamer_8h.html#a023c32dedd5015febfa0c1985a193057",
"streamer__api_8h.html#ae5dcd38e8148989fa09363a3b9779baca568b1f38786076bac003834d2d34aee9",
"structElementTypeLookup.html#a09881986a80dd864476e3085ffe51c02",
"struct__ElementDecoder.html#a41e210f4b76e8e6d7900e2746e8ebc68"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';