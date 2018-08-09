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

class FsciParameter(object):

    '''
    Class that maps on parameters present in the XML files describing a protocol.
    '''

    def __init__(self, name, size, dependency=None, selectDict=None, description=None):
        '''
        Constructor for a FsciParameter.

        @param name: the name of the parameter
        @param size: the size in bytes of the parameter
        @param dependency: if the current parameter size depends on another parameter, this is the
                            name of the latter
        @param selectDict: a dictionary used for inferring dependencies between parameters
        @param description: a brief additional description of the parameter, usually not used
        '''
        self.name = name
        self.size = size
        self.dependency = dependency
        self.selectDict = selectDict
        self.description = description
        self.endianness = FsciParameterType.LittleEndianBytes

    def __repr__(self):
        return self.name + ' [' + str(self.size) + ' byte(s)] -> ' + str(self.dependency)

    def __str__(self):
        return self.name + ' [' + str(self.size) + ' byte(s)] -> ' + str(self.dependency)


class FsciParameterType(object):
    LittleEndianBytes = 0
    BigEndianBytes = 1
