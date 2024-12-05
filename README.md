# SQLiteOS

SQLiteOS is a database-backed operating system that runs SQLite3 in the Linux kernel and provides database-backed services to user space, such as interprocess communication (IPC) and transactional virtual file system (VFS). Please follow traditional Linux build instructions. Currently the extension is tested on x86 Microsoft-WSL2 ([main](https://github.com/realVinayak/SQLiteOS/tree/main) branch) and x86 Amazon EC2 ([linux-aws](https://github.com/realVinayak/SQLiteOS/tree/linux-aws) branch). Please direct any questions via git issues or email to vinayakjha@ku.edu. This is a hobby project which begun as a course-project for a graduate systems course at University of Kansas. Strongly inspired by [DBOS](https://dbos-project.github.io/).

Also see transactional [extension](https://github.com/realVinayak/mysql-server-isam) of MyISAM, which leverages SQLiteOS (please don't use it for _any_ production database, since the file system is in-memory, and consequently, the database data is also in-memory).

The code is made open-source partially for the spirit of learning, and if you find any improvements, please drop an email or an issue, or, even better, a PR!

# Introduction

The [WSL2-Linux-Kernel][wsl2-kernel] repo contains the kernel source code and
configuration files for the [WSL2][about-wsl2] kernel.

# Reporting Bugs

If you discover an issue relating to WSL or the WSL2 kernel, please report it on
the [WSL GitHub project][wsl-issue]. It is not possible to report issues on the
[WSL2-Linux-Kernel][wsl2-kernel] project.

If you're able to determine that the bug is present in the upstream Linux
kernel, you may want to work directly with the upstream developers. Please note
that there are separate processes for reporting a [normal bug][normal-bug] and
a [security bug][security-bug].

# Feature Requests

Is there a missing feature that you'd like to see? Please request it on the
[WSL GitHub project][wsl-issue].

If you're able and interested in contributing kernel code for your feature
request, we encourage you to [submit the change upstream][submit-patch].

# Build Instructions

Instructions for building an x86_64 WSL2 kernel with an Ubuntu distribution are
as follows:

1. Install the build dependencies:  
   `$ sudo apt install build-essential flex bison dwarves libssl-dev libelf-dev`
2. Build the kernel using the WSL2 kernel configuration:  
   `$ make KCONFIG_CONFIG=Microsoft/config-wsl`

# Install Instructions

Please see the documentation on the [.wslconfig configuration
file][install-inst] for information on using a custom built kernel.

[wsl2-kernel]:  https://github.com/microsoft/WSL2-Linux-Kernel
[about-wsl2]:   https://docs.microsoft.com/en-us/windows/wsl/about#what-is-wsl-2
[wsl-issue]:    https://github.com/microsoft/WSL/issues/new/choose
[normal-bug]:   https://www.kernel.org/doc/html/latest/admin-guide/bug-hunting.html#reporting-the-bug
[security-bug]: https://www.kernel.org/doc/html/latest/admin-guide/security-bugs.html
[submit-patch]: https://www.kernel.org/doc/html/latest/process/submitting-patches.html
[install-inst]: https://docs.microsoft.com/en-us/windows/wsl/wsl-config#configure-global-options-with-wslconfig
