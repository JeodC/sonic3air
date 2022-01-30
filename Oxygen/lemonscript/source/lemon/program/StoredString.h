/*
*	Part of the Oxygen Engine / Sonic 3 A.I.R. software distribution.
*	Copyright (C) 2017-2022 by Eukaryot
*
*	Published under the GNU GPLv3 open source software license, see license.txt
*	or https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#pragma once

#include "lemon/compiler/Token.h"


namespace lemon
{

	class API_EXPORT StoredString
	{
	friend class StringLookup;

	public:
		inline const std::string_view& getString() const { return mStringView; }
		inline uint64 getHash() const					 { return mHash; }

	private:
		std::string_view mStringView;
		uint64 mHash = 0;		// 64-bit hash of the string
	};


	class API_EXPORT StringLookup
	{
	public:
		StringLookup();

		inline size_t size() const { return mNumEntries; }

		void clear();
		const StoredString* getStringByHash(uint64 hash) const;
		const StoredString& getOrAddString(std::string_view str);
		const StoredString& getOrAddString(std::string_view str, uint64 hash);

		void addFromLookup(const StringLookup& other);

		void serialize(VectorBinarySerializer& serializer);

	private:
		static inline const size_t HASH_TABLE_SIZE    = 0x800;
		static inline const uint64 HASH_TABLE_BITMASK = (uint64)(HASH_TABLE_SIZE - 1);

		struct TableEntry
		{
			StoredString mStoredString;
			TableEntry* mNext = nullptr;
		};
		ObjectPool<TableEntry, 64> mEntryPool;
		TableEntry* mTable[HASH_TABLE_SIZE] = { nullptr };	// Using lowest 10 bits of the key as index
		size_t mNumEntries = 0;
		rmx::OneTimeAllocPool mMemoryPool;
	};


	struct API_EXPORT StringRef
	{
		uint64 mHash = 0;
		const StoredString* mStoredString = nullptr;

		inline StringRef() {}
		inline explicit StringRef(uint64 hash) : mHash(hash) {}
		inline StringRef(uint64 hash, const StoredString* storedString) : mHash(hash), mStoredString(storedString) {}

		inline bool isValid() const  { return (nullptr != mStoredString); }
		inline const std::string_view* operator->() const  { return &mStoredString->getString(); }
		inline const std::string_view& operator*() const   { return mStoredString->getString(); }
	};

}
