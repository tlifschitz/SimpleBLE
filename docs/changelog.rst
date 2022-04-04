Changelog
=========

All notable changes to this project will be documented in this file.

The format is based on `Keep a Changelog`_, and this project adheres to
`Semantic Versioning`_.

[0.3.0] - 2021-XX-XX
--------------------

**Added**
-  Pairing functionality has been validated on all supported operating systems.
   In the case of Windows and MacOS, the user will be required to interact with
   an operating system popup to pair the device, while on Linux all pairing 
   requests will automatically be accepted, with passcodes ``abc123`` or ``123456``.
-  Unpair command has been added, although the only working implementation
   will be the linux one. Both Windows and MacOS require the user to manually
   unpair a device from the corresponding OS settings page.

**Changed**

-  Updated Linux implementation to use SimpleBluez v0.3.1.
-  Migrated to using safe callbacks from external vendor (kvn::safe_callback).

[0.2.0] - 2021-02-13
--------------------

**Added**

-  (Linux) Support for emulated battery service. *(Thanks ptenbrock!)*

**Fixed**

-  (Windows) Proper cleanup of callbacks during destruction.
-  (Windows) Async timeout reduced to 10 seconds.
-  (Linux) Returned characteristic value would be empty or outdated. *(Thanks ptenbrock!)*
-  (MacOS) Fixed a bunch of memory leaks and enabled automatic reference counting.
-  (MacOS) Fixed race condition.

**Changed**

-  Updated Linux implementation to use SimpleBluez v0.2.1.


[0.1.0] - 2021-12-28
--------------------

**Changed**

-  Referenced specific version of SimpleBluez to avoid breaking changes as those libraries evolve.
-  (Linux) When ``scan_stop`` is called, it is now guaranteed that no more scan results will be received.
-  Updated Linux implementation to use SimpleBluez v0.1.1.

**Fixed**

-  (Linux) Scan will never stop sleeping.


[0.0.2] - 2021-10-09
--------------------

**Added**

-  Safe implementation of ``SimpleBLE::Adapter`` and ``SimpleBLE::Peripheral`` classes.
-  CppCheck and ClangFormat CI checks. *(Thanks Andrey1994!)*
-  C-style API with examples.
-  Access to manufacturer data in the ``SimpleBLE::Peripheral`` class, for Windows and MacOS.

**Fixed**

-  Compilation errors that came up during development. *(Thanks fidoriel!)*
-  WinRT buffer allocation would fail. *(Thanks PatrykSajdok!)*
-  ``SimpleBLE::Adapter`` would fail to stop scanning. *(Thanks PatrykSajdok!)*
-  Switched WinRT initialization to single-threaded.

**Changed**

-  SimpleBluez dependency migrated to OpenBluetoothToolbox.


[0.0.1] - 2021-09-06
--------------------

**Added**

-  Initial definition of the full API.
-  Usage examples of the library.

.. _Keep a Changelog: https://keepachangelog.com/en/1.0.0/
.. _Semantic Versioning: https://semver.org/spec/v2.0.0.html