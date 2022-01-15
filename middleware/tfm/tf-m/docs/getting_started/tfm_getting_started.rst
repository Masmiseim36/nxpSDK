####################################
Select and set up build environments
####################################

TF-M officially supports a limited set of build environments and setups. In
this context, official support means that the environments listed below
are actively used by team members and active developers, hence users should
be able to recreate the same configurations by following the instructions
described below. In case of problems, the TF-M team provides support
only for these environments, but building in other environments can still be
possible.

The following environments are supported:

.. tabs::

    .. group-tab:: Linux

        1. version supported:

           Ubuntu 18.04 x64+

        2. install dependencies:

        .. code-block:: bash

            sudo apt-get install -y git curl wget build-essential libssl-dev python3 \
            python3-pip cmake make

        3. verify cmake version:

        .. code-block:: bash

            cmake --version

        .. note::

            Please download cmake 3.15 or later version from https://cmake.org/download/.

        4. add CMake path into environment:

        .. code-block:: bash

            export PATH=<CMake path>/bin:$PATH

    .. group-tab:: Windows

        1. version supported:

           Windows 10 x64

        2. install dependecies:

        - Git client latest version (https://git-scm.com/download/win)
        - CMake (`native Windows version <https://cmake.org/download/>`__)
        - GNU make (http://gnuwin32.sourceforge.net/packages/make.htm)
        - Python3 `(native Windows version) <https://www.python.org/downloads/>`__ and
          the pip package manager (from Python 3.4 it's included)

        3. add CMake path into environment:

        .. code-block:: bash

            set PATH=<CMake_Path>\bin;$PATH

###########################
Install python dependencies
###########################

Clone the TF-M source code, and then install the TF-M's additional Python
dependencies.

.. tabs::

    .. group-tab:: Linux

        1. get the TF-M source code:

        .. code-block:: bash

            cd <base folder>
            git clone https://git.trustedfirmware.org/TF-M/trusted-firmware-m.git

        2. TF-M's ``tools/requirements.txt`` file declares additional Python
           dependencies. Install them with ``pip3``:

        .. code-block:: bash

            pip3 install --upgrade pip
            cd trusted-firmware-m
            pip3 install -r tools/requirements.txt

    .. group-tab:: Windows

        1. get the TF-M source code:

        .. code-block:: bash

            cd <base folder>
            git clone https://git.trustedfirmware.org/TF-M/trusted-firmware-m.git

        2. TF-M's ``tools/requirements.txt`` file declares additional Python
           dependencies. Install them with ``pip3``:

        .. code-block:: bash

            cd trusted-firmware-m
            pip3 install -r tools\requirements.txt

###################
Install a toolchain
###################

To compile TF-M code, at least one of the supported compiler toolchains have to
be available in the build environment. The currently supported compiler
versions are:

    - Arm Compiler v6.10.1+

      .. tabs::

          .. group-tab:: Linux

              - Download the standalone packages from `here <https://developer.arm.com/products/software-development-tools/compilers/arm-compiler/downloads/version-6>`__.
              - Add Arm Compiler into environment:

                .. code-block:: bash

                    export PATH=<ARM_CLANG_PATH>/sw/ARMCompiler6.10.1/bin:$PATH
                    export ARM_PRODUCT_PATH=<ARM_CLANG_PATH>/sw/mappings

          .. group-tab:: Windows

              - Download the standalone packages from `here <https://developer.arm.com/products/software-development-tools/compilers/arm-compiler/downloads/version-6>`__.
              - Add Arm Compiler into environment:

                .. code-block:: bash

                    set PATH=<ARM_CLANG_PATH>\sw\ARMCompiler6.10.1\bin;$PATH
                    set ARM_PRODUCT_PATH=<ARM_CLANG_PATH>\sw\mappings

    - GNU Arm compiler v7.3.1+

      .. tabs::

          .. group-tab:: Linux

              - Download the GNU Arm compiler from `here <https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads>`__.
              - Add GNU Arm into environment:

                .. code-block:: bash

                    export PATH=<GNU_ARM_PATH>/bin:$PATH

          .. group-tab:: Windows

              - Download the GNU Arm compiler from `here <https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads>`__.
              - Add GNU Arm into environment:

                .. code-block:: bash

                    export PATH=<GNU_ARM_PATH>\bin;$PATH

      .. note::

          GNU Arm compiler version *10-2020-q4-major* has an issue in CMSE
          support. The bug is reported in `here <https://gcc.gnu.org/bugzilla/show_bug.cgi?id=99157>`__.
          Select other GNU Arm compiler versions instead.

    - IAR Arm compiler v8.42.x, v8.50.x

      .. tabs::

          .. group-tab:: Linux

              - Download IAR build tools from `here <https://www.iar.com/iar-embedded-workbench/build-tools-for-linux/>`__.
              - Add IAR Arm compiler into environment:

                .. code-block:: bash

                    export PATH=<IAR_COMPILER_PATH>/bin:$PATH

          .. group-tab:: Windows

              - Download IAR build tools from `here <https://www.iar.com/iar-embedded-workbench/#!?architecture=Arm>`__.
              - Add IAR Arm compiler into environment:

                .. code-block:: bash

                    export PATH=<IAR_COMPILER_PATH>\bin;$PATH

#############################
Build AN521 regression sample
#############################

Here, we take building TF-M for AN521 platform with regression tests using GCC
as an example:

.. tabs::

    .. group-tab:: Linux

        .. code-block:: bash

            cd trusted-firmware-m
            cmake -S . -B cmake_build -DTFM_PLATFORM=arm/mps2/an521 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DCMAKE_BUILD_TYPE=Debug -DTEST_S=ON -DTEST_NS=ON
            cmake --build cmake_build -- install

        Alternately using traditional cmake syntax

        .. code-block:: bash

            cd trusted-firmware-m
            mkdir cmake_build
            cd cmake_build
            cmake .. -DTFM_PLATFORM=arm/mps2/an521 -DTFM_TOOLCHAIN_FILE=../toolchain_GNUARM.cmake -DTEST_S=ON -DTEST_NS=ON
            make install

    .. group-tab:: Windows

        .. code-block:: bash

            cd trusted-firmware-m
            cmake -G"Unix Makefiles" -S . -B cmake_build -DTFM_PLATFORM=arm/mps2/an521 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DCMAKE_BUILD_TYPE=Debug -DTEST_S=ON -DTEST_NS=ON
            cmake --build cmake_build -- install

        Alternately using traditional cmake syntax

        .. code-block:: bash

            cd trusted-firmware-m
            mkdir cmake_build
            cd cmake_build
            cmake -G"Unix Makefiles" .. -DTFM_PLATFORM=arm/mps2/an521 -DTFM_TOOLCHAIN_FILE=../toolchain_GNUARM.cmake -DTEST_S=ON -DTEST_NS=ON
            make install

###########################
Run AN521 regression sample
###########################

Run the sample code on SSE-200 Fast-Model, using FVP_MPS2_AEMv8M provided by
Arm Development Studio.

.. note::

    Arm Development Studio is not essential to develop TF-M, you can skip this
    section if don't want to try on Arm develop boards.

.. tabs::

    .. group-tab:: Linux

        1. install Arm Development Studio to get the fast-model.

           Download Arm Development Studio from `here <https://developer.arm.com/tools-and-software/embedded/arm-development-studio>`__.

        2. Add ``bl2.axf`` and ``tfm_s_ns_signed.bin`` to symbol files in Debug
           Configuration menu.

        .. code-block:: bash

            <DS_PATH>/sw/models/bin/FVP_MPS2_AEMv8M  \
            --parameter fvp_mps2.platform_type=2 \
            --parameter cpu0.baseline=0 \
            --parameter cpu0.INITVTOR_S=0x10000000 \
            --parameter cpu0.semihosting-enable=0 \
            --parameter fvp_mps2.DISABLE_GATING=0 \
            --parameter fvp_mps2.telnetterminal0.start_telnet=1 \
            --parameter fvp_mps2.telnetterminal1.start_telnet=0 \
            --parameter fvp_mps2.telnetterminal2.start_telnet=0 \
            --parameter fvp_mps2.telnetterminal0.quiet=0 \
            --parameter fvp_mps2.telnetterminal1.quiet=1 \
            --parameter fvp_mps2.telnetterminal2.quiet=1 \
            --application cpu0=<build_dir>/bin/bl2.axf \
            --data cpu0=<build_dir>/bin/tfm_s_ns_signed.bin@0x10080000

    .. group-tab:: Windows

        1. install Arm Development Studio to get the fast-model.

           Download Arm Development Studio from `here <https://developer.arm.com/tools-and-software/embedded/arm-development-studio>`__.

        2. Add ``bl2.axf`` and ``tfm_s_ns_signed.bin`` to symbol files in Debug
           Configuration menu.

        .. code-block:: bash

            <DS_PATH>\sw\models\bin\FVP_MPS2_AEMv8M  \
            --parameter fvp_mps2.platform_type=2 \
            --parameter cpu0.baseline=0 \
            --parameter cpu0.INITVTOR_S=0x10000000 \
            --parameter cpu0.semihosting-enable=0 \
            --parameter fvp_mps2.DISABLE_GATING=0 \
            --parameter fvp_mps2.telnetterminal0.start_telnet=1 \
            --parameter fvp_mps2.telnetterminal1.start_telnet=0 \
            --parameter fvp_mps2.telnetterminal2.start_telnet=0 \
            --parameter fvp_mps2.telnetterminal0.quiet=0 \
            --parameter fvp_mps2.telnetterminal1.quiet=1 \
            --parameter fvp_mps2.telnetterminal2.quiet=1 \
            --application cpu0=<build_dir>/bin/bl2.axf \
            --data cpu0=<build_dir>/bin/tfm_s_ns_signed.bin@0x10080000

After completing the procedure you should see the following messages on the
DAPLink UART (baud 115200 8n1)::

    [INF] Starting bootloader
    [INF] Image 0: magic=good, copy_done=0xff, image_ok=0xff
    [INF] Scratch: magic=bad, copy_done=0x5, image_ok=0x9
    [INF] Boot source: primary slot
    [INF] Swap type: none
    [INF] Bootloader chainload address offset: 0x20000
    [INF] Jumping to the first image slot
    [Sec Thread] Secure image initializing!

    #### Execute test suites for the protected storage service ####
    Running Test Suite PS secure interface tests (TFM_PS_TEST_2XXX)...
    > Executing 'TFM_PS_TEST_2001'
      Description: 'Create interface'
      TEST PASSED!
    > Executing 'TFM_PS_TEST_2002'
      Description: 'Get handle interface (DEPRECATED)'
    This test is DEPRECATED and the test execution was SKIPPED
      TEST PASSED!
    > Executing 'TFM_PS_TEST_2003'
      Description: 'Get handle with null handle pointer (DEPRECATED)'
    This test is DEPRECATED and the test execution was SKIPPED
      TEST PASSED!
    > Executing 'TFM_PS_TEST_2004'
      Description: 'Get attributes interface'
      TEST PASSED!
    > Executing 'TFM_PS_TEST_2005'
      Description: 'Get attributes with null attributes struct pointer'
    ....

##########################
Tool & Dependency overview
##########################

To build the TF-M firmware the following tools are needed:

.. csv-table:: Tool dependencies
   :header: "Name", "Version", "Component"

   "C compiler",,"Firmware"
   "CMake",,
   "GNU Make",,
   "tf-m-tests",CMake handles it,
   "mbed-crypto",CMake handles it,
   "MCUboot",CMake handles it,
   "Python",3.x,"Firmware, User Guide"
   "yaml",,"Firmware"
   "pyasn1",,"Firmware"
   "jinja2",,"Firmware"
   "cryptography",,"Firmware"
   "cbor",,"Firmware"
   "click",,"Firmware"
   "imgtool",,"Firmware"
   "Doxygen",">1.8","Reference manual"
   "Sphinx","=2.0.1","User Guide"
   "sphinxcontrib-plantuml",,"User Guide"
   "sphinxcontrib-svg2pdfconverter",,"User Guide"
   "sphinx-rtd-theme",,"User Guide"
   "sphinx-tabs",,"User Guide"
   "Git",,
   "PlantUML",">v1.2018.11","Reference Manual, User Guide"
   "Graphviz dot",">v2.38.0","Reference manual"
   "Java runtime environment (JRE)",">1.8","Reference Manual, User Guide"
   "LaTex",,"pdf version of Reference Manual and User Guide"
   "PdfLaTex",,"pdf version of Reference Manual and User Guide"
   "librsvg2-bin",, "User Guide"

Dependency chain:

.. uml::

   @startuml
    skinparam state {
      BackgroundColor #92AEE0
      FontColor black
      FontSize 16
      AttributeFontColor black
      AttributeFontSize 16
      BackgroundColor<<pdf>> #A293E2
      BackgroundColor<<doc>> #90DED6
    }
    state fw as "Firmware" : TF-M binary
    state c_comp as "C Compiler" : C99
    state gmake as "GNU make"
    state u_guide as "User Guide" <<doc>>
    state refman as "Reference Manual" <<doc>>
    state rtd_theme as "sphinx-rtd-theme" <<doc>>
    state tabs as "sphinx-tabs" <<doc>>
    state sphnix_puml as "sphinxcontrib-plantuml" <<doc>>
    state sphnix_svg as "sphinxcontrib-svg2pdfconverter" <<doc>>
    state JRE as "JRE" <<doc>> : Java Runtime Environment
    state gwiz as "Graphwiz dot" <<doc>>
    state Sphinx as "Sphinx" <<doc>>
    state m2r as "m2r" <<doc>>
    state PlantUML as "PlantUML" <<doc>>
    state LaTex as "LaTex" <<pdf>>
    state PdfLaTex as "PdfLaTex" <<<<pdf>>>>
    state Doxygen as "Doxygen" <<doc>>
    state librsvg as "librsvg2-bin" <<doc>>

    [*] --> fw
    fw --> c_comp
    fw --> CMake
    CMake --> gmake
    fw --> cryptography
    fw --> pyasn1
    fw --> yaml
    fw --> jinja2
    fw --> cbor
    fw --> click
    fw --> imgtool
    cryptography --> Python3
    pyasn1 --> Python3
    yaml --> Python3
    jinja2 --> Python3
    cbor --> Python3
    click --> Python3
    imgtool --> Python3

    [*] --> u_guide
    u_guide --> Sphinx
    Sphinx --> m2r
    Sphinx --> rtd_theme
    Sphinx --> tabs
    Sphinx --> sphnix_puml
    Sphinx --> sphnix_svg
    m2r --> Python3
    rtd_theme --> Python3
    tabs --> Python3
    sphnix_puml --> Python3
    sphnix_svg --> Python3
    sphnix_svg --> librsvg
    Sphinx --> PlantUML
    PlantUML --> JRE
    PlantUML --> gwiz
    Sphinx --> LaTex
    LaTex --> PdfLaTex

    [*] --> refman
    refman --> Doxygen
    Doxygen --> PlantUML
    Doxygen --> LaTex
    state Legend {
      state x as "For PDF generation only" <<pdf>>
      state y as "For document generation only" <<doc>>
      state z as "Mandatory"
    }

   @enduml

##########
Next steps
##########

Here are some next steps for exploring TF-M:

    - Detailed :doc:`Build instructions </docs/technical_references/instructions/tfm_build_instruction>`.
    - :doc:`IAR Build instructions </docs/technical_references/instructions/tfm_build_instruction_iar>`.
    - Try other :doc:`Samples and Demos </docs/technical_references/instructions/run_tfm_examples_on_arm_platforms>`.
    - :doc:`Documentation generation </docs/technical_references/instructions/documentation_generation>`.

--------------

*Copyright (c) 2017-2021, Arm Limited. All rights reserved.*
