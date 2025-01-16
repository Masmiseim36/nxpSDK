# mmu_xspi

## Overview
The mmu_xspi example is a demonstration program that uses the MMU driver to
leverage the MMU module's features.

This example demonstrats simple map, region map, and array map.
The simple map means map one physical unit into the virtual address space.
The region map means map a region of physical memory into the virtual address space, and the region size must be unit aligned.
The array map means map an array of physical unit into an array of virtual address space.

## Supported Boards
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/driver_examples/mmu/xspi/example_board_readme.md)
