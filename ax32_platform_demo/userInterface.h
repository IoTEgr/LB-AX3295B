#ifndef __USERINTERFACE_H_
#define __USERINTERFACE_H_


typedef void (*actionType)(u8 *, u8 *, u8);
typedef bool (*actionRun)(void *, u8 *);

typedef struct UserInterface  
{
	u8 cnt;
	u8 action;
	u8 *preFrameBuf;
	actionType actionFunc;
	actionRun run;
} UserInterface;

extern UserInterface changeUserInterface;
extern uint8 jump_process_type;

enum{
	UPPER2BOTTOM,
	BOTTOM2UPPER,
	LEFT2RIGHT,
	RIGHT2LEFT,
	LEFTBOTTOM2RIGHTUPPER,
	LEFTUPPER2RIGHTBOTTOM,
	CIRCLE_INSIDE2OUTSIDE,
	CIRCLE_OUTSIDE2INSIDE,
	SQUARE_INSIDE2OUTSIDE,
	SQUARE_OUTSIDE2INSIDE,
};

enum
{
	JUMP_PROCESS_TYPE_NONE,
	CIRCLE_OPEN,
	CIRCLE_CLOSE,
};

extern bool animationRun(void *objName, u8 *srcBuf);
extern void animationUnInit(UserInterface *objName);

extern void type_function_circle(INT32U buff);



#define ANIMATION_CNT (/*10*/5)


#define ANIMATION(name, action)\
{\
	UserInterface name = {ANIMATION_CNT, action, NULL, NULL, animationRun};\
	animationInit(&name);\
}


#endif
  