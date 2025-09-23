
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>






//extern struct _tPM_RESULT	tPmResult[PM_CH_MAX];

extern uint8_t m_BITProcSeq;
extern uint8_t m_SysProcSeq;

extern void PROC_Init(void);
extern void SYSTEM_Process(void);
extern void PostMessage_Proc(void);
