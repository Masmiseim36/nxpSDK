# Alexa UX Audio for MCU

This readme is a guide to the AVS sound library and usage in MCU based Alexa applications. It covers the AVS Sound Library, usage of pre-made C array files, and how to add new C array files from AVS Sound Library audio files.

## AVS Sound Library

A guide on how to effectively utilize the sound library can be found online: [Alexa Sound LIbrary](https://developer.amazon.com/en-US/docs/alexa/alexa-voice-service/ux-design-attention.html#alexa-sound-library)

## NXP Generated C Arrays

NXP provies a set of pre-generated C arrays, intended for use with the SLN-ALEXA-IOT Solution.

These C arrays are accompanied with a C header file containing defintions for the sizes and 'extern' declaration of each audio array. This facilitates easy inclusion of the audio C arrays into the end application through single header file.

## Generating C arrays

Requirements:
- POSIX environment
- xxd
- AVS Sound Library

```
user@host:~/<path-to-avs-sound-library-download> $ xxd -i <audio-file>.wav <audio-c-array-file>.c
```
