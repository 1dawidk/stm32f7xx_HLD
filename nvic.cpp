#include "nvic.h"

void Nvic::EnableIRQ(IRQn_Type irq, uint32_t preemptPriority, uint32_t subPriority){
	uint32_t prioritygroup = 0x00;
	
	prioritygroup = NVIC_GetPriorityGrouping();
  NVIC_SetPriority(irq, NVIC_EncodePriority(prioritygroup, preemptPriority, subPriority));
	NVIC_EnableIRQ(irq);
}

void Nvic::SetPriorityGrouping(uint32_t priorityGroup){
	NVIC_SetPriorityGrouping(priorityGroup);
}

void Nvic::SetPriority(IRQn_Type irq, uint32_t preemptPriority, uint32_t subPriority){
	uint32_t prioritygroup = 0x00;
  
  prioritygroup = NVIC_GetPriorityGrouping();
  NVIC_SetPriority(irq, NVIC_EncodePriority(prioritygroup, preemptPriority, subPriority));
}

void Nvic::EnableIRQ(IRQn_Type irq){
	NVIC_EnableIRQ(irq);
}

void Nvic::DisableIRQ(IRQn_Type irq){
	NVIC_DisableIRQ(irq);
}

void Nvic::SetVectorTable(uint32_t flashBaseAddress, uint32_t offset){
	SCB->VTOR = flashBaseAddress | (offset & ((uint32_t)0x1FFFFF80));
}	

