#ifndef _NVIC_H
#define _NVIC_H

#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007U)
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006U)
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005U)
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004U)
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003U)

#include "stm32f7xx.h"

class Nvic{
	public:
		static void EnableIRQ(IRQn_Type irq, uint32_t preemptPriority, uint32_t subPriority);
		static void SetPriorityGrouping(uint32_t priorityGroup);
		static void SetPriority(IRQn_Type irq, uint32_t preemptPriority, uint32_t subPriority);
		static void EnableIRQ(IRQn_Type irq);
		static void DisableIRQ(IRQn_Type irq);
		static void SetVectorTable(uint32_t flashBaseAddress, uint32_t offset);
};

#endif
