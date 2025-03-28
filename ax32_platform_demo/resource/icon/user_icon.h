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

#ifndef USER_ICON_H 
   #define USER_ICON_H



#define  TRANSPARENT_COLOR    0

enum
{
   R_ID_ICON_GAMESNAKEBODY=R_ID_TYPE_ICON, 
   R_ID_ICON_GAMESNAKEFOOD,
   R_ID_ICON_GAMESNAKEHEAD_D,
   R_ID_ICON_GAMESNAKEHEAD_L,
   R_ID_ICON_GAMESNAKEHEAD_R,
   R_ID_ICON_GAMESNAKEHEAD_U,
   R_ID_ICON_GAMESNAKEWALL,
   R_ID_ICON_MENUMUSICPAUSE,
   R_ID_ICON_MENUMUSICPLAY,
   R_ID_ICON_MTBATTERY0,
   R_ID_ICON_MTBATTERY1,
   R_ID_ICON_MTBATTERY2,
   R_ID_ICON_MTBATTERY3,
   R_ID_ICON_MTBATTERY4,
   R_ID_ICON_MTBATTERY5,
   R_ID_ICON_MTNULL,
   R_ID_ICON_MTPAUSE,
   R_ID_ICON_MTPHOTO,
   R_ID_ICON_MTPHOTO3,
   R_ID_ICON_MTPHOTO5,
   R_ID_ICON_MTPHOTOFOCUSRED,
   R_ID_ICON_MTPHOTOFOCUSYELLOW,
   R_ID_ICON_MTPHOTOTIME10,
   R_ID_ICON_MTPHOTOTIME3,
   R_ID_ICON_MTPHOTOTIME5,
   R_ID_ICON_MTPHOTOTIME5P3,
   R_ID_ICON_MTPLAY,
   R_ID_ICON_MTRECORD,
   R_ID_ICON_MTRECORD1080P,
   R_ID_ICON_MTRECORD720P,
   R_ID_ICON_MTRECORDING,
   R_ID_ICON_MTRECORDVGA,
   R_ID_ICON_MTSDCNORMAL,
   R_ID_ICON_MTSDCNULL,

  R_ICON_MAX
};
extern R_ICON_T User_Icon_Table[];




#endif

