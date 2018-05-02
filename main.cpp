/****************************************************************************
 *   FileName    : main.cpp
 *   Description :
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited to re-
distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code shall
constitute any express or implied warranty of any kind, including without limitation, any warranty
of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright
or other third party intellectual property right. No warranty is made, express or implied,
regarding the information's accuracy, completeness, or performance.
In no event shall Telechips be liable for any claim, damages or other liability arising from, out of
or in connection with this source code or the use in the source code.
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
between Telechips and Company.
*
****************************************************************************/
/******************************************************************************
*  Description : This file contains functions and variables for a BT Sample
*******************************************************************************
*
*    yy/mm/dd     ver            descriptions                Author
*	---------	  --------       -----------------           -----------------
*    16/11/22     0.1            created                     xieyonghui
*******************************************************************************/
#include <QCoreApplication>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "BlueToothMidwareDBusRegistant.h"
#include <alsa/asoundlib.h>
#include "VolumeManager.h"
static void Daemonize();

int g_debug = 0;
int _daemonize = 1;
int main(int argc, char *argv[])
{
    int index;

    if (argc > 1)
    {
        for (index = 1; index < argc; index++)
        {
            if (strncmp(argv[index], "--debug", 7) == 0)
            {
                g_debug = 1;
            }
            else if (strncmp(argv[index], "--no-daemon", 11) == 0)
            {
                _daemonize = 0;
            }
        }
    }

    if (_daemonize)
{
        Daemonize();
        fprintf(stderr, "bluetooth daemonized\n");
    }
   QCoreApplication a(argc, argv);
    a.setApplicationVersion("0.0.0.1");
    a.setApplicationName("SmkBlueToothMidware");

    if(!BlueToothMidwareDBusRegistant::getInstance())
        {
       // qFatal("initQtDBus failed!");//And exit()
         fprintf(stderr, "initQtDBus failed! \n");
        return -1;
    }
    else
    {
        fprintf(stderr, "bluetooth initialize ok \n");
    }
    VolumeManager::instance();
    return a.exec();
    }

static void Daemonize()
{
    pid_t pid = 0;

    // create child process
    pid = fork();

    // fork failed
    if (pid < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }

    // parent process
    if (pid > 0)
    {
        // exit parent process for daemonize
        exit(0);
    }

    // umask the file mode
    umask(0);

    // set new session
    if (setsid() < 0)
    {
        fprintf(stderr ,"set new session failed\n");
        exit(1);
    }

    // change the current working directory for safety
    if (chdir("/") < 0)
    {
        fprintf(stderr, "change directory failed\n");
        exit(1);
    }

    // close standard file descriptor
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    //close(STDERR_FILENO);
}
