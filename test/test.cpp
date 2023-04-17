#include <format>
#include <iostream>
#include <string>

import libpe;
using namespace libpe;

int main()
{
	libpe::Clibpe pe;
	if (pe.OpenFile(L"C:\\Windows\\notepad.exe") != PEOK) {
		return -1;
	}

	const auto peResRoot = pe.GetResources();
	if (!peResRoot) {
		return -1;
	}

	std::wstring wstrResData; //This wstring will contain all resources by name.
	for (const auto& iterRoot : peResRoot->vecResData) { //Main loop to extract Resources.
		auto ilvlRoot = 0;
		auto pResDirEntry = &iterRoot.stResDirEntry; //ROOT IMAGE_RESOURCE_DIRECTORY_ENTRY
		if (pResDirEntry->NameIsString) {
			wstrResData += std::format(L"Entry: {} [Name: {}]\r\n", ilvlRoot, iterRoot.wstrResName);
		}
		else {
			if (const auto iter = MapResID.find(pResDirEntry->Id); iter != MapResID.end()) {
				wstrResData += std::format(L"Entry: {} [Id: {}, {}]\r\n", ilvlRoot, pResDirEntry->Id, iter->second);
			}
			else {
				wstrResData += std::format(L"Entry: {} [Id: {}]\r\n", ilvlRoot, pResDirEntry->Id);
			}
		}

		if (pResDirEntry->DataIsDirectory) {
			auto ilvl2 = 0;
			auto pstResLvL2 = &iterRoot.stResLvL2;
			for (const auto& iterLvL2 : pstResLvL2->vecResData) {
				pResDirEntry = &iterLvL2.stResDirEntry; //Level 2 IMAGE_RESOURCE_DIRECTORY_ENTRY
				if (pResDirEntry->NameIsString) {
					wstrResData += std::format(L"    Entry: {}, Name: {}\r\n", ilvl2, iterLvL2.wstrResName);
				}
				else {
					wstrResData += std::format(L"    Entry: {}, Id: {}\r\n", ilvl2, pResDirEntry->Id);
				}

				if (pResDirEntry->DataIsDirectory) {
					auto ilvl3 = 0;
					auto pstResLvL3 = &iterLvL2.stResLvL3;
					for (const auto& iterLvL3 : pstResLvL3->vecResData) {
						pResDirEntry = &iterLvL3.stResDirEntry; //Level 3 IMAGE_RESOURCE_DIRECTORY_ENTRY
						if (pResDirEntry->NameIsString) {
							wstrResData += std::format(L"        Entry: {}, Name: {}\r\n", ilvl3, iterLvL3.wstrResName);
						}
						else {
							wstrResData += std::format(L"        Entry: {}, lang: {}\r\n", ilvl3, pResDirEntry->Id);
						}
						++ilvl3;
					}
				}
				++ilvl2;
			}
		}
		++ilvlRoot;
	}
	std::wcout << wstrResData;

	return 0;
}