/****************************************************************************
       ***             ***                      MAXLIB-GRAPHC                  
      ** **           ** **                                                    
     **   **         **   **            THE MAXLIB FOR IMAGE SHOW PROCESS      
    **     **       **     **                                                  
   **       **     **       **              MAX ROURCE ICON MANAGEMENT         
  **         **   **         **                                                
 **           ** **           **              (C) COPYRIGHT 2016 MAX           
**             ***             **                                              
                                                                               
* File Name   : user_icon.c                                                    
* Author      : Mark.Douglas                                                   
* Version     : V0100                                                          
* Date        : 05/25/2016                                                     
* Description : This file for maxlib resource icon managemant                  
*                                                                              
* History     :                                                                
* 2016-05-25  :                                                                
*      <1>.This is created by mark,set version as v0103.                       
*      <2>.Add basic functions.                                                
******************************************************************************/

#include "../application.h"
#include "user_icon.h"
R_ICON_T User_Icon_Table[R_ICON_MAX&0xffff]=
{
   {R_ID_ICON_GAMESNAKEBODY                 ,	(void *)0,0, 14, 14,0x00000000,0x00000000},
   {R_ID_ICON_GAMESNAKEFOOD                 ,	(void *)0,0, 14, 14,0x00000000,0x00000001},
   {R_ID_ICON_GAMESNAKEHEAD_D               ,	(void *)0,0, 14, 14,0x00000000,0x00000002},
   {R_ID_ICON_GAMESNAKEHEAD_L               ,	(void *)0,0, 14, 14,0x00000000,0x00000003},
   {R_ID_ICON_GAMESNAKEHEAD_R               ,	(void *)0,0, 14, 14,0x00000000,0x00000004},
   {R_ID_ICON_GAMESNAKEHEAD_U               ,	(void *)0,0, 14, 14,0x00000000,0x00000005},
   {R_ID_ICON_GAMESNAKEWALL                 ,	(void *)0,0, 14, 14,0x00000000,0x00000006},
   {R_ID_ICON_MENUMUSICPAUSE                ,	(void *)0,0, 24, 24,0x00000000,0x00000007},
   {R_ID_ICON_MENUMUSICPLAY                 ,	(void *)0,0, 24, 24,0x00000000,0x00000008},
   {R_ID_ICON_MTBATTERY0                    ,	(void *)0,0, 32, 32,0x00000000,0x00000009},
   {R_ID_ICON_MTBATTERY1                    ,	(void *)0,0, 32, 32,0x00000000,0x0000000a},
   {R_ID_ICON_MTBATTERY2                    ,	(void *)0,0, 32, 32,0x00000000,0x0000000b},
   {R_ID_ICON_MTBATTERY3                    ,	(void *)0,0, 32, 32,0x00000000,0x0000000c},
   {R_ID_ICON_MTBATTERY4                    ,	(void *)0,0, 32, 32,0x00000000,0x0000000d},
   {R_ID_ICON_MTBATTERY5                    ,	(void *)0,0, 32, 32,0x00000000,0x0000000e},
   {R_ID_ICON_MTNULL                        ,	(void *)0,0, 32, 32,0x00000000,0x0000000f},
   {R_ID_ICON_MTPAUSE                       ,	(void *)0,0, 40, 40,0x00000000,0x00000010},
   {R_ID_ICON_MTPHOTO                       ,	(void *)0,0, 32, 32,0x00000000,0x00000011},
   {R_ID_ICON_MTPHOTO3                      ,	(void *)0,0, 32, 32,0x00000000,0x00000012},
   {R_ID_ICON_MTPHOTO5                      ,	(void *)0,0, 32, 32,0x00000000,0x00000013},
   {R_ID_ICON_MTPHOTOFOCUSRED               ,	(void *)0,0,132,104,0x00000000,0x00000014},
   {R_ID_ICON_MTPHOTOFOCUSYELLOW            ,	(void *)0,0,132,104,0x00000000,0x00000015},
   {R_ID_ICON_MTPHOTOTIME10                 ,	(void *)0,0, 32, 32,0x00000000,0x00000016},
   {R_ID_ICON_MTPHOTOTIME3                  ,	(void *)0,0, 32, 32,0x00000000,0x00000017},
   {R_ID_ICON_MTPHOTOTIME5                  ,	(void *)0,0, 32, 32,0x00000000,0x00000018},
   {R_ID_ICON_MTPHOTOTIME5P3                ,	(void *)0,0, 32, 32,0x00000000,0x00000019},
   {R_ID_ICON_MTPLAY                        ,	(void *)0,0, 32, 32,0x00000000,0x0000001a},
   {R_ID_ICON_MTRECORD                      ,	(void *)0,0, 32, 32,0x00000000,0x0000001b},
   {R_ID_ICON_MTRECORD1080P                 ,	(void *)0,0, 40, 24,0x00000000,0x0000001c},
   {R_ID_ICON_MTRECORD720P                  ,	(void *)0,0, 40, 24,0x00000000,0x0000001d},
   {R_ID_ICON_MTRECORDING                   ,	(void *)0,0, 32, 32,0x00000000,0x0000001e},
   {R_ID_ICON_MTRECORDVGA                   ,	(void *)0,0, 40, 24,0x00000000,0x0000001f},
   {R_ID_ICON_MTSDCNORMAL                   ,	(void *)0,0, 32, 32,0x00000000,0x00000020},
   {R_ID_ICON_MTSDCNULL                     ,	(void *)0,0, 32, 32,0x00000000,0x00000021},
};
