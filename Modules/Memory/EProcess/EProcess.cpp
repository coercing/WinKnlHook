#include "EProcess.h"

namespace EProcess
{
    PEPROCESS* FindProcessByID(uint32_t target_pid) {

        auto process_list_head = NT.KeCapturePersistentThreadState();
        if (!process_list_head) {
            return nullptr;
        }

        auto linkage_va = reinterpret_cast<uintptr_t>(process_list_head) -
            NT.PsInitialSystemProcess();
        if (!linkage_va) {
            return nullptr;
        }

        for (auto flink = process_list_head->Flink; flink; flink = flink->Flink) {
            if (!NT.MmIsAddressValid(flink)) {
                break;
            }

            auto curr_eprocess = reinterpret_cast<PEPROCESS*>(
                reinterpret_cast<uintptr_t>(flink) - linkage_va
                );

            if (!curr_eprocess)
                continue;

            auto process_id = NT.PsGetProcessId(curr_eprocess);

            if (process_id == target_pid) {
                printf("[+] Found target process: %u at %p\n", process_id, curr_eprocess);
                return curr_eprocess;
            }
        }

        printf("[-] Target PID not found\n");
        return nullptr;
    }

    NTSTATUS GetEProcess(pprocessid request)
    {
        if (!request || !request->ProcessID)
            return STATUS_INVALID_PARAMETER;

        request->MProcess = FindProcessByID(request->ProcessID);

        return request->MProcess ? STATUS_SUCCESS : STATUS_UNSUCCESSFUL;
    }
}