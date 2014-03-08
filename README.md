DiskArcher-backup-utility
=========================
A backup utility for personal use. Visual C++, MFC, ADO


I started working on it in 2001 or so. It was a shareware and some copies of it were sold.
The highest rating in search engines was being shown on the 1st page (i.e. top 10) for "backup utility".
If you search for 'DiskArcher', there are still thousands of links to it. Although, I have not been supporting a Web site for it for many years now.

I use DiskArcher for my backups and I am proud that it is very stable and reliable.


The source code consists of a number of subprojects, some of them are static libraries.
MArc2 is the main subproject, it represents all the program UI.

As a result of the project build, only one .exe file is produced, no any .dlls or other files are built.
This .exe relies of MS VC redistributables and usual Windows dlls.

As it is a quite old project, currently there is a mix of coding styles. So,
  TODO: - Get rid of the Hungarian notation;
        - Use 2 spaces instead of tabs.