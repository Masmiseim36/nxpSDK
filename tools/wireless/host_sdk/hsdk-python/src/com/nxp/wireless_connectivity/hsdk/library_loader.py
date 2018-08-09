'''
* The Clear BSD License
* Copyright 2014-2015 Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
'''

from ctypes import cdll
import os
import sys

from singleton import singleton


extension = {'linux': '.so', 'linux2': '.so', 'linux3': '.so', 'darwin': '.dylib', 'win': '.dll'}


@singleton
class LibraryLoader(object):

    '''
    Class that loads the native C code libraries.
    '''

    def __init__(self):

        if sys.platform.startswith('linux') or sys.platform == 'darwin':
            try:
                self.lib_dir = os.popen('ldconfig -p | grep fsci').read().split('\n')[0].split('=>')[1].split('libfsci')[0].lstrip().rstrip()
            except:
                sys.exit("[ERROR] The system could not find the Host SDK shared libraries. Please make sure that `ldconfig -p` displays them.")
            self.ext = extension[sys.platform]

            try:
                self.CSpiLibrary = cdll.LoadLibrary(self.lib_dir + 'libspi' + self.ext)
            except:
                print "[WARNING] Could not find libspi.so, SPI support is disabled."
                self.CSpiLibrary = None

            self.CUartLibrary = cdll.LoadLibrary(self.lib_dir + 'libuart' + self.ext)
            self.CPhysicalLibrary = cdll.LoadLibrary(self.lib_dir + 'libphysical' + self.ext)
            self.CSysLibrary = cdll.LoadLibrary(self.lib_dir + 'libsys' + self.ext)
            self.CFramerLibrary = cdll.LoadLibrary(self.lib_dir + 'libframer' + self.ext)
            self.CFsciLibrary = cdll.LoadLibrary(self.lib_dir + 'libfsci' + self.ext)

        elif sys.platform.startswith('win'):
            self.HSDKLibrary = cdll.LoadLibrary(sys.prefix + '\DLLs\HSDK' + extension['win'])
            self.CUartLibrary = self.HSDKLibrary
            self.CSpiLibrary = None  # not available on Windows
            self.CPhysicalLibrary = self.HSDKLibrary
            self.CSysLibrary = self.HSDKLibrary
            self.CFramerLibrary = self.HSDKLibrary
            self.CFsciLibrary = self.HSDKLibrary

        else:
            raise Exception(sys.platform + ' is not supported yet.')
