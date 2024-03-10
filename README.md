# WAV-files
This C code is a program for processing WAV files. It reads a WAV file from standard input, checks its header for correctness, and then outputs the modified WAV data to standard output based on the specified processing mode.

The simplest possible version of the WAV file format, which will be used in the context of the assignment, is described below. A WAV file contains, in order:

4 bytes with the characters "RIFF" (initials of Resource Interchange File Format, which is the standard used for the structure of WAV files).
4 bytes, symbolically named SizeOfFile, representing the size of the file. In reality, this is the number of bytes following until the end of the file. Essentially, it is the file size on disk minus 8 bytes, i.e., those occupied by the current field and the previous one ("RIFF"). Note that the representation of unsigned integers is in little-endian order, from the least significant byte to the most significant one. For example, the integer 0xA3870FB9 is represented in order by the bytes 0xB9, 0x0F, 0x87, 0xA3.
4 bytes with the characters "WAVE," indicating the specific file category that follows the RIFF standard.
4 bytes with the characters "fmt " (note the last space character), indicating the start of the format section (format chunk) of the file.
4 bytes for the space occupied by the data of the format section that will follow. In the files used in the assignment, the value of this field will always be 16 (0x00000010). Here, the little-endian order is applied again. That is, this number is stored with the sequence of bytes 0x10, 0x00, 0x00, 0x00.
2 bytes for the WAVE format type. For the assignment, this will always be equal to 1 (0x0001) and will be stored in little-endian order, i.e., with the bytes 0x01, 0x00.
2 bytes, symbolically named MonoStereo, for whether the sound is monophonic (value 0x0001) or stereophonic with two channels (value 0x0002). Pay attention to little-endian, as in all subsequent integer entries.
4 bytes, symbolically named SampleRate, for the sampling rate, i.e., how many values per second of the analog sound function are recorded in the file.
4 bytes, symbolically named BytesPerSec, for the number of bytes per second of sound contained in the file.
2 bytes, symbolically named BlockAlign, for the number of bytes required to record sound information at a given moment for all channels. Note that BytesPerSec = SampleRate × BlockAlign should always be true.
2 bytes, symbolically named BitsPerSample, for the number of bits required to record sound information at a given moment for a single channel. In the assignment, this value will be either 8 (0x0008) or 16 (0x0010). For BitsPerSample = 8, the sound intensity is an unsigned integer in the range [0, 255], while for BitsPerSample = 16, the sound intensity is a signed integer in the range [-32768, 32767]. Note that BlockAlign = BitsPerSample/8 × MonoStereo should always be true.
4 bytes with the characters "data," indicating the start of the data section (data chunk) of the file.
4 bytes, symbolically named SizeOfData, for the space occupied by the data of the data section that will follow.
The digitized sound data in chronological order. In the case of stereophonic sound with two channels, for each moment in time, there is a pair of data, with the first element corresponding to the left channel and the second element to the right channel.
After the end of the sound data, there may be other sections provided for the WAVE category, but we will not address them in the assignment.

Mode 1: The program should check if the read data follows the WAV format described previously and print the relevant sound information. Your program should be able to detect errors in the read sound data and stop at the first error it encounters, printing the appropriate diagnostic message.

Mode 2: Extend your program so that, in this operating mode, in addition to checking the correctness of the input data in terms of compliance with the wav standard, it transfers to the output, using the putchar function, the sound data, having doubled the playback speed without distorting the samples that constitute the digitized sound. It is evident that the output of the program must be a legal sequence of data that constitutes a sound based on the described standard. Even if there are additional bytes after the data section, they should also be transferred to the output, obviously intact.

Mode 3: Implement a similar function to the previous section, where the sound input will be transferred to the output with twice the playback speed. Obviously, the playback time will be halved since the samples constituting the digitized sound should again be transferred unchanged.

Mode 4: Extend your program so that in this operating mode, in addition to checking the correctness of the input data regarding their compliance with the WAV standard, it transfers to the output, with the help of the putchar function, only the data of the left channel of the sound in case the input sound is stereo. If the input sound is monophonic, no error should occur; instead, simply transfer the single channel of the input to the output. Consider which parameters of the input data need to change and how, and which bytes of the data section of the input should be transferred to the output to achieve the desired result. In any case, the output of the program must be a legal sequence of data constituting a sound based on the described standard. Even if there are additional bytes in the input after the data section, they should also be transferred to the output, obviously intact.

Mode 5: Implement a similar functionality to the previous section, where the output will contain the right channel of the input in the case of stereophonic sound at the input or the intact single channel in the case of monophonic sound.

