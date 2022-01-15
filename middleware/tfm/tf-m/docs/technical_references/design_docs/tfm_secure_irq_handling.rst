###################
Secure IRQ handling
###################

The Armv8-M Architecture makes it possible to configure interrupts to target
secure state.

TF-M makes it possible for secure partitions to get notified of secure
interrupts.

By default TF-M sets up interrupts to target NS state. To configure an interrupt
to target secure state and assign a handler to it, the manifest of the partition
must be edited.

See the following example:


.. code-block:: yaml

    {
      "name": "...",
      "type": "...",
      "priority": "...",

      ...

      "irqs": [
        {
          "source": "5",
          "signal": "DUAL_TIMER"
        },
        {
          "source": "TFM_IRQ_LINE_TIMER_1",
          "signal": "TIMER_1"
        }
      ],

      ...

    }

To set up a handler in a partition, the ``irqs`` node must be added. A single
secure partition can have handlers registered for multiple IRQs, in this case
the list ``irqs`` has multiple elements in it.

An IRQ handler is defined by the following nodes:

- ``source``: The IRQ number or the name of the IRQ line. With the name of the
  IRQ line, there must be defined a macro in ``tfm_peripherals_def.h`` which is
  substituted to the IRQ line num. The IRQ line nums and sources are defined by
  each platform: for example, they are defined in ``platform_irq.h`` for the
  Musca-S1 platform. When defining new macros in ``tfm_peripherals_def.h``, it
  is important the macro name matches the platform's handler function for that
  IRQ source.
- ``signal``: The name of the signal for this IRQ.

.. important::

  The name of the privileged interrupt handler is derived from the node
  specifying the IRQ line number.

  - In case ``source`` is IRQ number, the name of the handler becomes
    ``void irq_<number>_Handler(void)``.
  - In case ``source`` is defined IRQ macro, the name of the handler becomes
    ``void <macro>_Handler(void)``.

  This is important, because the derived name has to be present in the vector
  table as the handler of the IRQ. The platform startup functions are specified
  in the vector table defined in the platform secure startup file. The user
  should verify the names of the generated handlers match for a given platform
  IRQ.

.. Note::

  ``signal`` and ``source`` are mandatory.

If an IRQ handler is registered, TF-M will:

- Set the IRQ with number or macro to target secure state
- Set the priority of IRQ, the number is platform dependent. Platforms can
  decide the priorities of each IRQ.

.. Note::

  The priority value for IRQ must be smaller than the value of PendSV, which is
  0x80.

TF-M configures the interrupt lines to be disabled by default. Interrupts for a
service can be enabled by the secure service by calling
``void tfm_enable_irq(psa_signal_t irq_signal)``. The function can be called in
the service init function.

Library model
=============

In Library model a function with the name derived from the value of the
``source`` property is generated. This function will be put in the vector table
by the linker (as the handlers in the startup assembly are defined as weak
symbols). The code generated for this function will forward the call to the
function with the name of the value of the ``signal`` property post-fixed with
``_isr``.

.. hint::

  for a signal ``"signal": "DUAL_TIMER"`` the name of the handler function is
  ``DUAL_TIMER_isr``

The signature of the IRQ handler in the partition must be the following:

.. code-block:: c

    void partition_irq_handler(void);

The detailed description on how secure interrupt handling works in the Library
model see
`Secure Partition Interrupt Handling design document <https://developer.trustedfirmware.org/w/tf_m/design/secure_partition_interrupt_handling/>`_.

IPC model
=========

The detailed description on how secure interrupt handling works in the IPC
model, see the
`PSA Firmware Framework and RoT Services specification <https://pages.arm.com/psa-resources-ff.html>`_.

**********************
Implementation details
**********************

Library model implementation
============================

As a result of the function call like behaviour of secure services in library
model, some information that is critical for the SPM to keep track of partition
states, is stored on the stack of the active partitions. When an interrupt
happens, and a handler partition is set to running state, it has access to its
whole stack, and could corrupt the data stacked by the SPM. To prevent this, a
separate Context stack is introduced for each secure partition, that is used by
the SPM to save this information before starting to execute secure partition
code.

A stack frame to this context stack is pushed when the execution in the
partition is interrupted, and when a handler in the partition interrupts another
service. So the maximal stack usage can happen in the following situation:

Consider secure partition 'A'. 'A' is running, and then it is interrupted by
an other partition. Then the lowest priority interrupt of 'A' is triggered.
Then before the handler returns, the partition is interrupted by another
partition's handler. Then before the running handler returns, the second
lowest interrupt of 'A' is triggered. This can go until the highest priority
interrupt of 'A' is triggered, and then this last handler is interrupted. At
this point the context stack looks like this:

.. code-block:: rst

  +------------+
  | [intr_ctx] |
  | [hndl_ctx] |
  | .          |
  | .          |
  | .          |
  | [intr_ctx] |
  | [hndl_ctx] |
  | [intr_ctx] |
  +------------+

  Legend:
    [intr_ctx]: Frame pushed when the partition is interrupted
    [hndl_ctx]: Frame pushed when the partition is handling an interrupt

So the max stack size can be calculated as a function of the IRQ count of 'A':

.. code-block:: c

  max_stack_size = intr_ctx_size + (IRQ_CNT * (intr_ctx_size + hndl_ctx_size))

--------------

*Copyright (c) 2018-2021, Arm Limited. All rights reserved.*
