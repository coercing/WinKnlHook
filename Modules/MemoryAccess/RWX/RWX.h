#pragma once

#include "../../../Includes.h"

namespace RWX {
	// Page table entry structures
	typedef union _pml4e
	{
		struct
		{
			std::uint64_t present : 1;                   // Must be 1 if valid
			std::uint64_t read_write : 1;               // Write access control
			std::uint64_t user_supervisor : 1;           // User/supervisor access control
			std::uint64_t page_write_through : 1;        // Write-through caching
			std::uint64_t cached_disable : 1;            // Cache disable
			std::uint64_t accessed : 1;                  // Set when accessed
			std::uint64_t ignored0 : 1;                  // Ignored
			std::uint64_t large_page : 1;               // Reserved (must be 0)
			std::uint64_t ignored1 : 4;                 // Ignored
			std::uint64_t pfn : 36;                     // Physical frame number
			std::uint64_t reserved : 4;                 // Reserved for software
			std::uint64_t ignored2 : 11;                // Ignored
			std::uint64_t no_execute : 1;               // No-execute bit
		} hard;
		std::uint64_t value;
	} pml4e, * ppml4e;

	typedef union _pdpte
	{
		struct
		{
			std::uint64_t present : 1;                   // Must be 1 if valid
			std::uint64_t read_write : 1;               // Write access control
			std::uint64_t user_supervisor : 1;           // User/supervisor access control
			std::uint64_t page_write_through : 1;        // Write-through caching
			std::uint64_t cached_disable : 1;            // Cache disable
			std::uint64_t accessed : 1;                  // Set when accessed
			std::uint64_t dirty : 1;                    // Set when written to (1GB pages)
			std::uint64_t page_size : 1;                // 1=1GB page, 0=points to page directory
			std::uint64_t ignored1 : 4;                 // Ignored
			std::uint64_t pfn : 36;                     // Physical frame number
			std::uint64_t reserved : 4;                 // Reserved for software
			std::uint64_t ignored2 : 11;                // Ignored
			std::uint64_t no_execute : 1;               // No-execute bit
		} hard;
		std::uint64_t value;
	} pdpte, * ppdpte;

	typedef union _pde
	{
		struct
		{
			std::uint64_t present : 1;                   // Must be 1 if valid
			std::uint64_t read_write : 1;               // Write access control
			std::uint64_t user_supervisor : 1;           // User/supervisor access control
			std::uint64_t page_write_through : 1;        // Write-through caching
			std::uint64_t cached_disable : 1;            // Cache disable
			std::uint64_t accessed : 1;                  // Set when accessed
			std::uint64_t dirty : 1;                    // Set when written to (2MB pages)
			std::uint64_t page_size : 1;                // 1=2MB page, 0=points to page table
			std::uint64_t global : 1;                   // Global page (if CR4.PGE=1)
			std::uint64_t ignored1 : 3;                 // Ignored
			std::uint64_t pfn : 36;                     // Physical frame number
			std::uint64_t reserved : 4;                 // Reserved for software
			std::uint64_t ignored2 : 11;                // Ignored
			std::uint64_t no_execute : 1;               // No-execute bit
		} hard;
		std::uint64_t value;
	} pde, * ppde;

	typedef union _pte
	{
		struct
		{
			std::uint64_t present : 1;                   // Must be 1 if valid
			std::uint64_t read_write : 1;               // Write access control
			std::uint64_t user_supervisor : 1;           // User/supervisor access control
			std::uint64_t page_write_through : 1;        // Write-through caching
			std::uint64_t cached_disable : 1;            // Cache disable
			std::uint64_t accessed : 1;                  // Set when accessed
			std::uint64_t dirty : 1;                    // Set when written to
			std::uint64_t pat : 1;                      // Page Attribute Table bit
			std::uint64_t global : 1;                   // Global page
			std::uint64_t ignored1 : 3;                 // Ignored
			std::uint64_t pfn : 36;                     // Physical frame number
			std::uint64_t reserved : 4;                 // Reserved for software
			std::uint64_t ignored2 : 7;                 // Ignored
			std::uint64_t protection_key : 4;           // Protection key
			std::uint64_t no_execute : 1;               // No-execute bit
		} hard;
		std::uint64_t value;
	} pte, * ppte;

	// Page size constants
	constexpr auto page_4kb_size = 0x1000ull;
	constexpr auto page_2mb_size = 0x200000ull;
	constexpr auto page_1gb_size = 0x40000000ull;

	constexpr auto page_shift = 12ull;
	constexpr auto page_2mb_shift = 21ull;
	constexpr auto page_1gb_shift = 30ull;

	constexpr auto page_4kb_mask = 0xFFFull;
	constexpr auto page_2mb_mask = 0x1FFFFFull;
	constexpr auto page_1gb_mask = 0x3FFFFFFFull;

	// Function declarations
	auto ReadMemory(uint64_t destination, PVOID src, SIZE_T size, SIZE_T* copiedSize) -> NTSTATUS;
	auto WritePhysicalAddress(uint64_t target_address, PVOID buffer, SIZE_T size, SIZE_T* bytes_written) -> NTSTATUS;
	auto TranslateLinear(UINT64 dtbase, UINT64 vtaddy) -> uint64_t;
	auto translate(std::uint64_t dtb, std::uint64_t virtual_address, std::uint32_t* out_page_size = nullptr) -> std::uint64_t;

	// Cached PML4 entries
	extern cache cached_pml4e[512];

}