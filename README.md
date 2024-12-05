# SQLiteOS

SQLiteOS is a database-backed operating system that runs SQLite3 in the Linux kernel and provides database-backed services to user space, such as interprocess communication (IPC) and transactional virtual file system (VFS). Please follow traditional Linux build instructions. Currently the extension is tested on x86 Microsoft-WSL2 ([main](https://github.com/realVinayak/SQLiteOS/tree/main) branch) and x86 Amazon EC2 ([linux-aws](https://github.com/realVinayak/SQLiteOS/tree/linux-aws) branch). Please direct any questions via git issues or email to vinayakjha@ku.edu. This is a hobby project which begun as a course-project for a graduate systems course at University of Kansas. Strongly inspired by [DBOS](https://dbos-project.github.io/).

Also see transactional [extension](https://github.com/realVinayak/mysql-server-isam) of MyISAM, which leverages SQLiteOS (please don't use it for _any_ production database, since the file system is in-memory, and consequently, the database data is also in-memory).

The code is made open-source partially for the spirit of learning, and if you find any improvements, please drop an email or an issue, or, even better, a PR!

Linux kernel
============

There are several guides for kernel developers and users. These guides can
be rendered in a number of formats, like HTML and PDF. Please read
Documentation/admin-guide/README.rst first.

In order to build the documentation, use ``make htmldocs`` or
``make pdfdocs``.  The formatted documentation can also be read online at:

    https://www.kernel.org/doc/html/latest/

There are various text files in the Documentation/ subdirectory,
several of them using the Restructured Text markup notation.

Please read the Documentation/process/changes.rst file, as it contains the
requirements for building and running the kernel, and information about
the problems which may result by upgrading your kernel.
