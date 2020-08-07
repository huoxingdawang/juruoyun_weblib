@echo off
xcopy %libs%\jbl jbl\ /E /Y /Q /R
attrib +r jbl\*
attrib -r jbl\jbl*config.h
copy examples\makefile + jbl\makefile + jwl\makefile makefile
xcopy jwl %libs%\jwl\ /E /Y /Q